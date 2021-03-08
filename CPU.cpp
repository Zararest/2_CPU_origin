#include "commands.h"
#include "stack.h"
const int INIT_SIZE = 10;
const int MEMORY_SIZE = 500;



struct CPU_struct{
    double* machine_code = NULL;
    int command_pointer = 0;
    my_stack* Stack = NULL;

    unsigned char* memory = (unsigned char*) calloc(MEMORY_SIZE, sizeof(unsigned char));

    /*unsigned char* segment = (unsigned char*) calloc(32, sizeof(unsigned char));
    unsigned char* B_A = segment;
    unsigned char* B_B = segment + 1 * sizeof(unsigned char);
    unsigned char* B_C = segment + 2 * sizeof(unsigned char);

    unsigned char* W_A = B_C + 1;
    unsigned char* W_B = W_A + 1 * 2 * sizeof(unsigned char);
    unsigned char* W_C = W_A + 2 * 2 * sizeof(unsigned char);
    unsigned char* W_D = W_A + 3 * 2 * sizeof(unsigned char);

    unsigned char* DW_A = W_D + 2;
    unsigned char* DW_B = DW_B + 1 * 4 * sizeof(unsigned char);
    unsigned char* DW_C = DW_B + 2 * 4 * sizeof(unsigned char);
    unsigned char* DW_D = DW_B + 3 * 4 * sizeof(unsigned char);
    unsigned char* DW_E = DW_B + 4 * 4 * sizeof(unsigned char);*/

    char* byte_segment = (char*) calloc(3, sizeof(char));
    short int* word_segment = (short int*) calloc(4, sizeof(short int));
    int* double_word_segment = (int*) calloc(5, sizeof(int));

    double* segment_private = (double*) calloc(2, sizeof(double));

};

void processor(struct CPU_struct * CPU, int num_of_comands){

    double fir_num = 0, sec_num = 0;
    int reg_len = 0, i = 0, reg_pointer = 0, var_addr = 0;
    CPU->Stack = stack_create(num_of_comands, sizeof(double), 0);

    while (CPU->command_pointer < num_of_comands){
        printf("D_A = %i\n", *CPU->DW_A);

        switch ((int)CPU->machine_code[CPU->command_pointer]){

        case CPU_MOV_RD:
            printf("MOV_RD was called \n");
            reg_len = ((int)(CPU->machine_code[CPU->command_pointer + 1] / 10)) % 10;

            switch (reg_len) {

            case 1:
                //reg_pointer = (int)CPU->B_A + ( ( (int)CPU->machine_code[CPU->command_pointer + 1] ) % 10) - (int)CPU->segment;
                CPU->byte_segment[ (( (int)CPU->machine_code[CPU->command_pointer + 1] ) % 10) ] = (char) CPU->machine_code[CPU->command_pointer + 2];
                break;

            case 2:
                /*reg_pointer = (int)CPU->W_A + ( (int) CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                *(CPU->segment + reg_pointer) = (short int) CPU->machine_code[CPU->command_pointer + 2];*/
                CPU->word_segment[ (( (int)CPU->machine_code[CPU->command_pointer + 1] ) % 10) ] = (short int) CPU->machine_code[CPU->command_pointer + 2];//надо все нахер переделать
                break;

            case 4:
                CPU->double_word_segment[ (( (int)CPU->machine_code[CPU->command_pointer + 1] ) % 10) ] = (int) CPU->machine_code[CPU->command_pointer + 2];
                break;

            case 5:
                CPU->segment_private[ (( (int)CPU->machine_code[CPU->command_pointer + 1] ) % 10) ] = (double) CPU->machine_code[CPU->command_pointer + 2];
                break;
            }

            CPU->command_pointer += 3;
            break;

        case CPU_MOV_RV:

            reg_len = ((int)(CPU->machine_code[CPU->command_pointer + 1] / 10)) % 10;
            printf("MOV_RV was called and reglen = %i\n", reg_len);

            switch (reg_len) {

            case 1:
                var_addr = (int)CPU->machine_code[CPU->command_pointer + 2] + (int)CPU->machine_code[CPU->command_pointer + 3];
                CPU->byte_segment[ (( (int)CPU->machine_code[CPU->command_pointer + 1] ) % 10) ] = *((char*) CPU->memory + var_addr);
                break;

            case 2:
                var_addr = (int)CPU->machine_code[CPU->command_pointer + 2] + (int)CPU->machine_code[CPU->command_pointer + 3];
                CPU->word_segment[ (( (int)CPU->machine_code[CPU->command_pointer + 1] ) % 10) ] = *((short int*) CPU->memory + var_addr);
                break;

            case 4:
                var_addr = (int)CPU->machine_code[CPU->command_pointer + 2] + (int)CPU->machine_code[CPU->command_pointer + 3];
                CPU->double_word_segment[ (( (int)CPU->machine_code[CPU->command_pointer + 1] ) % 10) ] = *((int*) CPU->memory + var_addr);
                break;

            case 5:
                var_addr = (int)CPU->machine_code[CPU->command_pointer + 2] + (int)CPU->machine_code[CPU->command_pointer + 3];
                CPU->segment_private[ (( (int)CPU->machine_code[CPU->command_pointer + 1] ) % 10) ] =  *((double*) CPU->memory + var_addr);//???????
                break;
            }

            CPU->command_pointer += 4;
            break;

        case CPU_MOV_VR:
            //printf("MOV_VR was called \n");
            reg_len = ((int)(CPU->machine_code[CPU->command_pointer + 3] / 10)) % 10;
            printf("MOV_VR was called and reglen = %i\n", reg_len);

            switch (reg_len) {

            case 1:
                var_addr = (int)CPU->machine_code[CPU->command_pointer + 1] + (int)CPU->machine_code[CPU->command_pointer + 2];
                *( (char*) (CPU->memory + var_addr)) = (char)CPU->byte_segment[ ( (int)CPU->machine_code[CPU->command_pointer + 3] ) % 10 ];
                break;

            case 2:
                var_addr = (int)CPU->machine_code[CPU->command_pointer + 1] + (int)CPU->machine_code[CPU->command_pointer + 2];
                *( (short int*) (CPU->memory + var_addr)) = *((short int*) (CPU->segment + reg_pointer));
                break;

            case 4:
                var_addr = (int)CPU->machine_code[CPU->command_pointer + 1] + (int)CPU->machine_code[CPU->command_pointer + 2];
                *( (int*) (CPU->memory + var_addr)) = *((int*) (CPU->segment + reg_pointer));
                /*for (i = 0; i < 50; i++){
                    printf("|%i|", CPU->segment[i]);
                }
                printf("\n in mov vr var = %i and reg = %i\n", CPU->memory[ (int)CPU->machine_code[CPU->command_pointer + 1] + (int)CPU->machine_code[CPU->command_pointer + 2] ], *((int*) CPU->segment + reg_pointer));*/
                break;

            case 5:
                reg_pointer = (int)CPU->P_F + 8 * ((int)CPU->machine_code[CPU->command_pointer + 3] % 10) - (int)CPU->segment;
                CPU->memory[ (int)CPU->machine_code[CPU->command_pointer + 1] + (int)CPU->machine_code[CPU->command_pointer + 2] ] = *((double*) CPU->segment + reg_pointer);
                break;
            }

            CPU->command_pointer += 4;
            break;

        //

        case CPU_PUSH:
            printf("push was called com.poin = %i\n", CPU->command_pointer);

            reg_len = ((int)(CPU->machine_code[CPU->command_pointer + 1] / 10)) % 10;

            switch (reg_len) {

            case 1:
                reg_pointer = (int)CPU->B_A + ((int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                fir_num = *( (char*) CPU->segment + reg_pointer);
                break;

            case 2:
                reg_pointer = (int)CPU->W_A + ((int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                fir_num = *( (short int*) CPU->segment + reg_pointer);

                break;

            case 4:
                //printf("in push comand = %lf\n", CPU->machine_code[CPU->command_pointer + 1] );
                reg_pointer = (int)CPU->DW_A + 4 * ((int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                fir_num = *( (int*) (CPU->segment + reg_pointer) );

                //fir_num = *( (int*) (CPU->DW_A + ((int)CPU->machine_code[CPU->command_pointer + 1] % 10)) );
                //printf("in push segment = |%i| reg_pointer = |%i| \n", CPU->segment, ((int)CPU->machine_code[CPU->command_pointer + 1] % 10));
                //printf("-----%lf-----\n", fir_num);
                break;

            case 5:
                reg_pointer = (int)CPU->P_F + ((int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                fir_num = *( (double*) CPU->segment + reg_pointer);
                break;
            }

            assert( push(CPU->Stack, fir_num) == 0 );

            CPU->command_pointer += 2;
            break;

        case CPU_IN:
            printf("in was called com.poin = %i\n", CPU->command_pointer);

            scanf("%lf", &fir_num);

            reg_len = ((int)(CPU->machine_code[CPU->command_pointer + 1] / 10)) % 10;

            switch (reg_len) {

            case 1:
                reg_pointer = (int)CPU->B_A + ( (int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                *(CPU->segment + reg_pointer) = (char) fir_num;
                break;

            case 2:
                reg_pointer = (int)CPU->W_A + ( (int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                *(CPU->segment + reg_pointer) = (short int) fir_num;
                break;

            case 4:
                reg_pointer = (int)CPU->DW_A + ( (int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                *(CPU->segment + reg_pointer) = (int) fir_num;
                break;

            case 5:
                reg_pointer = (int)CPU->P_F + ( (int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                *(CPU->segment + reg_pointer) = fir_num;
                break;
            }

            CPU->command_pointer += 2;
            break;

        case NOP:
            CPU->command_pointer++;
            break;

        case CPU_ADD:
            printf("add was called com.poin = %i\n", CPU->command_pointer);
            sec_num = *(double*)pop(CPU->Stack);
            fir_num = *(double*)pop(CPU->Stack);
            printf("in add sum = %lf\n", fir_num + sec_num);
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

            if (*CPU->P_F < 0){
                printf("go to %i\n", CPU->machine_code[ CPU->command_pointer + 1]);
                CPU->command_pointer = CPU->machine_code[ CPU->command_pointer + 1];
            } else{

                CPU->command_pointer +=2;
            }
            break;

        case CPU_JG:
            printf("jg was called () com.poin = %i\n", CPU->command_pointer);
            if (*CPU->P_F > 0){

                CPU->command_pointer = CPU->machine_code[ CPU->command_pointer + 1];
            } else{
                printf("what\n");
                CPU->command_pointer +=2;
            }
            break;

        case CPU_JE:
            printf("je was called com.poin = %i\n", CPU->command_pointer);
            if (*CPU->P_F == 0){

                CPU->command_pointer = CPU->machine_code[ CPU->command_pointer + 1];
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

                *CPU->P_F = 1;
            }
                printf("why we still here");
            if (fir_num == sec_num){

                *CPU->P_F = 0;
            }

            if (fir_num < sec_num){
                ("in cmp im here\n");
                *CPU->P_F = -1;
            }

            printf("------in cmp fir = %lf sec = %lf\n", fir_num, sec_num);
            printf("in cmp reg = %lf\n", *CPU->P_F);
            CPU->command_pointer++;
            break;

        case CPU_POP:
            printf("pop was called command pointer = %i\n", CPU->command_pointer);

            fir_num = *(double*)pop(CPU->Stack);

                reg_len = (int)CPU->machine_code[CPU->command_pointer + 1] / 10 % 10;

                switch (reg_len) {

                case 1:
                    reg_pointer = (int)CPU->B_A + ( (int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                    *(CPU->segment + reg_pointer) = (char) fir_num;
                    break;

                case 2:
                    reg_pointer = (int)CPU->W_A + ( (int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                    *(CPU->segment + reg_pointer) = (short int) fir_num;
                    break;

                case 4:
                    reg_pointer = (int)CPU->DW_A + ( (int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                    *(CPU->segment + reg_pointer) = (int) fir_num;
                    break;

                case 5:
                    reg_pointer = (int)CPU->P_F + ( (int)CPU->machine_code[CPU->command_pointer + 1] % 10) - (int)CPU->segment;
                    *(CPU->segment + reg_pointer) = fir_num;
                }
            printf("in pop data = %lf\n", *(CPU->segment + reg_pointer));
            CPU->command_pointer += 2;
            break;

        case CPU_OUT:
            printf("out was called com.poin = %i\n", CPU->command_pointer);
            printf("---------------{%lf}", *(double*)pop(CPU->Stack));
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
    FILE* input_file = fopen("machine_commands.bin", "rb");

    if (input_file == NULL)
    {
        printf("Can't open file\n");
    }

    fread(&size_of_file, sizeof(int), 1, input_file);

    CPU.machine_code = (double*) calloc(size_of_file, sizeof(double));
    printf("size = %i\n", size_of_file);

    fread(CPU.machine_code, sizeof(double), size_of_file, input_file);

    fclose(input_file);

    processor(&CPU, size_of_file);
}
