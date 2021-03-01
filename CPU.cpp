#include "commands.h"
#include "stack.h"
const int INIT_SIZE = 10;

struct CPU_struct{
    double* machine_code = NULL;
    int command_pointer = 0;
    my_stack* Stack = NULL;
    double* segment = (double*) calloc(4, sizeof(double));
    double* AX = segment;                      //аккумулятор тут переменные для сравнения
    double* BX = segment + 1 * sizeof(double); //база
    double* CX = segment + 2 * sizeof(double); //счетчик
    double* DX = segment + 3 * sizeof(double);
};

void processor(struct CPU_struct * CPU, int num_of_comands){

    double fir_num = 0, sec_num = 0;
    CPU->Stack = stack_create(num_of_comands, sizeof(double), 0);

    while (CPU->command_pointer < num_of_comands){

        printf("|command pointer = %i|\n", CPU->command_pointer);
        switch ((int)CPU->machine_code[CPU->command_pointer]){

        case CPU_PUSH:
            printf("push was called com.poin = %i\n", CPU->command_pointer);
            assert( push(CPU->Stack, CPU->machine_code[CPU->command_pointer + 1]) == 0 );
            CPU->command_pointer += 2;
            break;

        case CPU_IN:
            printf("in was called com.poin = %i\n", CPU->command_pointer);
            scanf("%lf", &fir_num);
            assert( push(CPU->Stack, fir_num) == 0 );
            CPU->command_pointer++;
            break;

        case CPU_ADD:
            printf("add was called com.poin = %i\n", CPU->command_pointer);
            sec_num = *(double*)pop(CPU->Stack);
            fir_num = *(double*)pop(CPU->Stack);
            assert( push(CPU->Stack, fir_num + sec_num) == 0 );
            CPU->command_pointer++;
            break;

        case CPU_SUB:
            printf("sub was called com.poin = %i\n", CPU->command_pointer);
            sec_num = *(double*)pop(CPU->Stack);
            fir_num = *(double*)pop(CPU->Stack);
            assert( push(CPU->Stack, fir_num - sec_num) == 0 );
            CPU->command_pointer++;
            break;

        case CPU_DIV:
            printf("div was called com.poin = %i\n", CPU->command_pointer);
            sec_num = *(double*)pop(CPU->Stack);
            fir_num = *(double*)pop(CPU->Stack);
            assert( push(CPU->Stack, fir_num / sec_num) == 0 );
            CPU->command_pointer++;
            break;

        case CPU_MUL:
            printf("mul was called com.poin = %i\n", CPU->command_pointer);
            sec_num = *(double*)pop(CPU->Stack);
            fir_num = *(double*)pop(CPU->Stack);
            assert( push(CPU->Stack, fir_num * sec_num) == 0 );
            CPU->command_pointer++;
            break;

        case CPU_FSQRT:
            printf("fsqrt was called com.poin = %i\n", CPU->command_pointer);
            sec_num = *(double*)pop(CPU->Stack);
            fir_num = *(double*)pop(CPU->Stack);
            assert( push(CPU->Stack, sqrt(fir_num * sec_num)) == 0 );
            CPU->command_pointer++;
            break;

        case CPU_JL:
            printf("jl was called com.poin = %i\n", CPU->command_pointer);
            if (*CPU->AX < 0){
                CPU->command_pointer += CPU->machine_code[ CPU->command_pointer + 1] + 2;
            } else{
                CPU->command_pointer +=2;
            }
            break;

        case CPU_JG:
            printf("jg was called () com.poin = %i\n", CPU->command_pointer);
            if (*CPU->AX > 0){
                CPU->command_pointer = CPU->machine_code[ CPU->command_pointer + 1];
            } else{
                CPU->command_pointer +=2;
            }
            break;

        case CPU_JE:
            printf("je was called com.poin = %i\n", CPU->command_pointer);
            if (*CPU->AX == 0){
                CPU->command_pointer += CPU->machine_code[ CPU->command_pointer + 1] + 2;
            } else{
                CPU->command_pointer +=2;
            }
            break;

        case CPU_JN:
            printf("jn was called com.poin = %i\n", CPU->command_pointer);
            CPU->command_pointer = CPU->machine_code[ CPU->command_pointer + 1] ;
            break;

        case CPU_CMP:
            printf("cmp was called com.poin = %i\n", CPU->command_pointer);
            sec_num = *(double*)pop(CPU->Stack);
            fir_num = *(double*)pop(CPU->Stack);
            if (fir_num > sec_num){
                *CPU->AX = 1;
            } else{
                if (fir_num == sec_num){
                    *CPU->AX = 0;
                } else{
                    *CPU->AX = -1;
                }
            }

            CPU->command_pointer++;
            break;

        case CPU_PUSH_DX:
            printf("push_dx was called com.poin = %i\n", CPU->command_pointer);
            assert( push(CPU->Stack, *CPU->DX) == 0 );
            CPU->command_pointer++;
            break;

        case CPU_POP_DX:
            printf("pop_dx was called com.poin = %i\n", CPU->command_pointer);
            *CPU->DX = *(double*)pop(CPU->Stack);
            CPU->command_pointer++;
            break;

        case CPU_OUT:
            printf("out was called com.poin = %i\n", CPU->command_pointer);
            printf("%lf", *(double*)pop(CPU->Stack));
            CPU->command_pointer++;
            break;

        case CPU_HLT:
            printf("hlt was called com.poin = %i\n", CPU->command_pointer);
            exit(0);
            break;

        case END:
            printf("end was called com.poin = %i\n", CPU->command_pointer);
            CPU->command_pointer = num_of_comands;
            break;
        }
    }

}


int main(){
    struct CPU_struct CPU;
    int size_of_file = 0;
    FILE* input_file = fopen("machine_commands.bin", "r");

    if (input_file == NULL)
    {
        printf("Can't open file\n");
    }

    fread(&size_of_file, sizeof(int), 1, input_file);

    CPU.machine_code = (double*) calloc(size_of_file, sizeof(double));

    fread(CPU.machine_code, sizeof(double), size_of_file, input_file);
    fclose(input_file);

    processor(&CPU, size_of_file);
}
