#include "dhcp.h"
#include "../../../lib/memory.h"
#include "../../memory/pagingHandler.h"

uint8_t* getDefaultGatewayIPv4(){
    return defaultGatewayIPv4;
}

static uint8_t emptyIP[IPv4_LENGTH] = {0, 0, 0, 0};

static DHCPPacket* newDhcpPacket(uint8_t MAC[MAC_LENGTH], uint8_t serverIP[IPv4_LENGTH]){
    DHCPPacket* packet = allocPage();
    
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

    return packet;
}

void DHCPDiscover(uint8_t MAC[MAC_LENGTH]){
    DHCPPacket* packet = newDhcpPacket(MAC, emptyIP);
    
    DHCPDiscoverOptions* options = ((char*)packet) + sizeof(packet); //locate in the end of the packet
    options->packetTypeCode = DHCP_MESSAGE_TYPE;
    options->packetTypeLength = DHCP_MESSAGE_TYPE_LENGTH;
    options->packetType = DHCP_DISCOVER_TYPE;
    options->requestListCode = DHCP_REQUEST_LIST_CODE;
    options->requestListLength = DHCP_REQUEST_LIST_LENGTH;
    options->requestList = DHCP_REQUEST_LIST;

    options->endmark = DHCP_END;

    //TODO: send to UDP

    deallocPage(packet);
}


void DHCPRequest(uint8_t MAC[MAC_LENGTH]){
    DHCPPacket* packet = newDhcpPacket(MAC, getDefaultGatewayIPv4());
    
    DHCPRequestOptions* options = ((char*)packet) + sizeof(packet);
    options->packetTypeCode = DHCP_MESSAGE_TYPE;
    options->packetTypeLength = DHCP_MESSAGE_TYPE_LENGTH;
    options->packetType = DHCP_REQUEST_TYPE;

    options->serverIdentifierCode = DHCP_SERVER_IDENTIFIER_CODE;
    options->serverIdentifierLength = DHCP_SERVER_IDENTIFIER_LENGTH;
    options->serverIdentifier = getDefaultGatewayIPv4();

    options->endmark = DHCP_END;

    //TODO: send to UDP

    deallocPage(packet);
}

void DHCPRecv(){

}
