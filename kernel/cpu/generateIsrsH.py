from generateIsrsAsm import NUMBER_OF_ISRS

GENERATED_FILE_PATH = 'kernel/cpu/isrs.h'

file_beginning = '''// AUTO GENERATED FILE

#pragma once

#include <stdint.h>

'''


with open(GENERATED_FILE_PATH, 'w') as generated_file:
    generated_file.write(file_beginning)

    #define isrs
    for isr_number in range(NUMBER_OF_ISRS):
        generated_file.write(f'extern uint32_t* isr{isr_number}();\n')

    #write getter
    generated_file.write('''
uint32_t* getIsr(uint8_t isrNumber){
    switch(isrNumber){''')

    for isr_number in range(NUMBER_OF_ISRS):
        generated_file.write(f'''
    case {isr_number}:
        return (uint32_t*) isr{isr_number};
        break;
''')

    generated_file.write('''   
    default:
        return (uint32_t*) 0;
        break;
   }
}
''')
