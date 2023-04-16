#include "dhcp.h"
#include "../nic/rtl8139.h"
#include "../../memory/pagingHandler.h"
#include "../../../lib/memory.h"
#include "../../../lib/printf.h"
#include "../../../lib/convert.h"

static const uint8_t emptyIP[IPv4_LENGTH] = {0, 0, 0, 0};

bool connectToRouter(){
    printf("%CDHCP DISCOVER...\n", CYAN, DEFAULT_COLOR);
    if(DHCPDiscover()){
        printf("%CDHCP OFFER RECEIVED!\n", CYAN, DEFAULT_COLOR);
    }
    else{
        printf("%C\tError!\n", RED, DEFAULT_COLOR);
        printf("%C\tCouldn't find router\n", RED, DEFAULT_COLOR);
        return false;
    }
    

    printf("%CDHCP REQUEST...\n", CYAN, DEFAULT_COLOR);
    if(DHCPRequest()){
        printf("%CDHCP ACK RECEIVED!\n", CYAN, DEFAULT_COLOR);
        return true;
    }

    printf("%C\tError!\n", RED, DEFAULT_COLOR);
    printf("%C\tError while receiving IP from router\n", RED, DEFAULT_COLOR);
    return false;
}

static void initDHCPPacket(DHCPPacket* packet, uint8_t MAC[MAC_LENGTH], uint8_t serverIP[IPv4_LENGTH]){    
    packet->operation = REQUEST_OPCODE;
    packet->HWType = HW_TYPE_ETHERNET;
    packet->HWAddrLen = MAC_LENGTH;
    packet->hops = 0x0; //no hops yet
    packet->id = DHCP_MAGIC_ID;
    packet->seconds = 0x0;
    packet->flags = 0x0;
    memset(0, packet->clientIP, IPv4_LENGTH);
    memset(0, packet->yourIP, IPv4_LENGTH);
    memset(serverIP, packet->serverIP, IPv4_LENGTH);
    memset(0, packet->gatewayIP, IPv4_LENGTH);
    memset(0, packet->reserverd, DHCP_PADDING_SIZE);
    memset(MAC, packet->clientMAC, MAC_LENGTH);
    packet->magicCookie = DHCP_MAGIC_COOKIE;

    packet->options.packetTypeCode = DHCP_MESSAGE_TYPE;
    packet->options.packetTypeLength = DHCP_MESSAGE_TYPE_LENGTH;
    packet->options.packetType = DHCP_DISCOVER_TYPE;
    packet->options.requestListCode = DHCP_REQUEST_LIST_CODE;
    packet->options.requestListLength = DHCP_REQUEST_LIST_LENGTH;
    packet->options.requestList = DHCP_REQUEST_LIST;

    packet->options.endmark = DHCP_END;
}

static bool DHCPWaitForReply(DhcpPacketType replyType){
    for(int i = 0; i < DHCP_TYMEOUT_MS; i++){
        if(DHCPState == replyType)
            return true;
        
        //FIXME: sleep 1 ms
        //for(int i=0; i<30000; i++);
    }

    return false;
}

static bool DHCPDiscover(){
    return false;
    /*
    DHCPPacket packet;
    initDHCPPacket(&packet, getMac(), emptyIP);
    DHCPState = DHCP_DISCOVER_TYPE;
    
    //TODO: send to UDP
    return DHCPWaitForReply(DHCP_OFFER_TYPE);
    */
}
static bool DHCPRequest(){
    DHCPPacket packet;
    initDHCPPacket(&packet, getMac(), getDefaultGatewayIPv4());
    DHCPState = DHCP_REQUEST_TYPE;

    //TODO: send to UDP
    return DHCPWaitForReply(DHCP_ACK_TYPE);
}
/*

void DHCPRecv(DHCPPacket* packet){
    if(packet->operation!=REPLY_OPCODE){
        printf("%CWrong opcode %d.\n", RED, DEFAULT_COLOR, packet->operation);
        printf("%CExpected %d (DHCP Reply).\n", RED, DEFAULT_COLOR, REPLY_OPCODE);
        return;
    }
    if(packet->id != DHCP_MAGIC_ID){
        printf("%CWrong packet ID %d.\n", RED, DEFAULT_COLOR, packet->id);
        printf("%CExpected %d.\n", RED, DEFAULT_COLOR, DHCP_MAGIC_ID);
        return;
    }
    if(!memeq(packet->clientMAC, getMac(), MAC_LENGTH)){
        char* macBuffer[20];
        MACtos(packet->clientMAC, macBuffer);
        printf("%C%s is not my MAC.\n", RED, DEFAULT_COLOR, macBuffer);

        MACtos(getMac(), macBuffer);
        printf("%CMy MAC is %s.\n", RED, DEFAULT_COLOR, macBuffer);
        return;
    }
    if(packet->magicCookie != DHCP_MAGIC_COOKIE){
        printf("%CWrong coockie %h.\n", RED, DEFAULT_COLOR, packet->magicCookie);
        printf("%CDHCP cookie is %h (DHCP Reply).\n", RED, DEFAULT_COLOR, DHCP_MAGIC_COOKIE);
        return;
    }

    uint8_t* options = (uint8_t*)(&(packet->options));
    if(*options == DHCP_MESSAGE_TYPE){
        options++; //points to option length
        options++; //points to message type

        if(DHCPState == DHCP_DISCOVER_TYPE){
            if(*options == DHCP_OFFER_TYPE){
                DHCPGetOffer(packet, options + 1); //+1 - points to next option
            }
            else{
                printf("%CWrong message type %x.\n", RED, DEFAULT_COLOR, *options);
                printf("%CExpected %x (DHCP Offer).\n", RED, DEFAULT_COLOR, DHCP_OFFER_TYPE);
            }
        }
        else if(DHCPState == DHCP_REQUEST_TYPE){
            if(*options == DHCP_ACK_TYPE){
                DHCPGetAck(packet, options + 1); //+1 - points to next option
            }
            else{
                printf("%CWrong message type %x.\n", RED, DEFAULT_COLOR, *options);
                printf("%CExpected %x (DHCP Ack).\n", RED, DEFAULT_COLOR, DHCP_ACK_TYPE);
            }
        }
        else{
            printf("%CCurrent DHCP state (%x) doesn't allow replies.\n", RED, DEFAULT_COLOR, *options);
        }
    }
}

static void DHCPGetOffer(DHCPPacket* packet, uint8_t* options){
    DHCPState = DHCP_OFFER_TYPE;
}

static void DHCPGetAck(DHCPPacket* packet, uint8_t* pOptions){
    char* conversionBuffer[20];

    memcpy(packet->serverIP, defaultGatewayIPv4, IPv4_LENGTH);
    setIPv4(packet->yourIP);

    while(*pOptions != DHCP_END){
        int option = *pOptions;
        pOptions++;
        int optionLength = *pOptions;
        uint8_t* pValue = ++pOptions;

        switch (option)
        {
        case DHCP_OPTION_SUBNET_MASK:
            //for now, ignore subnet mask
            break;
        case DHCP_OPTION_ROUTER:
            memcpy(pValue, defaultGatewayIPv4, IPv4_LENGTH);
            break;

        case DHCP_OPTION_DNS_SERVERS:
            //TODO: set dns servers list
            break;

        case DHCP_OPTION_DOMAIN_NAME:
            //who cares?
            break;

        case DHCP_OPTION_IP_LEASE_TIME_SEC:
            //TODO: set expiration time
            break;
        
        default:
            printf("%CDHCP option %d is not supported.\n", RED, DEFAULT_COLOR, option);
            break;
        }

        pOptions += optionLength; //point to next option
    }

    DHCPState = DHCP_ACK_TYPE;
}
    */
