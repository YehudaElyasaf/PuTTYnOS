NUMBER_OF_IRQS = 16
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

    generated_file.write('''   
    default:
        return (uint32_t*) 0;
        break;
   }
}
''')
