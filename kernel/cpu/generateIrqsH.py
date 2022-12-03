from generateIrqsAsm import NUMBER_OF_IRQS, SYSCALL_IRQ_NUMBER

GENERATED_FILE_PATH = 'kernel/cpu/irqs.h'

file_beginning = '''// AUTO GENERATED FILE

#pragma once

#include <stdint.h>

'''

with open(GENERATED_FILE_PATH, 'w') as generated_file:
    generated_file.write(file_beginning)

    #define irqs
    for irq_number in range(NUMBER_OF_IRQS):
        generated_file.write(f'extern uint32_t* irq{irq_number}();\n')
    
    generated_file.write(f'\nextern uint32_t* irq{SYSCALL_IRQ_NUMBER}(); //syscall irq\n')

    #write getter
    generated_file.write('''
uint32_t* getIrq(uint8_t irqNumber){
    switch(irqNumber){''')

    for irq_number in range(NUMBER_OF_IRQS):
        generated_file.write(f'''
    case {irq_number}:
        return (uint32_t*) irq{irq_number};
        break;
''')

    #syscall irq
    generated_file.write(f'''

    //syscall irq
    case {SYSCALL_IRQ_NUMBER}:
        return (uint32_t*) irq{SYSCALL_IRQ_NUMBER};
        break;
''')

    generated_file.write('''   
    default:
        return (uint32_t*) 0;
        break;
   }
}
''')
