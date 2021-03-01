#include "commands.h"

void disassembler(double* machine_code, int num_of_comands, FILE* output_file){

    int command_pointer = 0;

    while (command_pointer < num_of_comands){

        switch ((int)machine_code[command_pointer]){
        case CPU_PUSH:
            fprintf(output_file, "PUSH %lf\n", machine_code[command_pointer + 1]);
            command_pointer += 2;
            break;

        case CPU_IN:
            fprintf(output_file, "IN\n");
            command_pointer++;
            break;

        case CPU_ADD:
            fprintf(output_file, "ADD\n");
            command_pointer++;
            break;

        case CPU_SUB:
            fprintf(output_file, "SUB\n");
            command_pointer++;
            break;

        case CPU_DIV:
            fprintf(output_file, "DIV\n");
            command_pointer++;
            break;

        case CPU_MUL:
            fprintf(output_file, "MUL\n");
            command_pointer++;
            break;

        case CPU_FSQRT:
            fprintf(output_file, "FSQRT\n");
            command_pointer++;
            break;

        case CPU_OUT:
            fprintf(output_file, "OUT\n");
            command_pointer++;
            break;

        case CPU_HLT:
            fprintf(output_file, "HLT\n");
            command_pointer++;
            break;

        case END:
            fprintf(output_file, "END\n");
            command_pointer++;
            break;
        }
    }
}

int main(){
    int size_of_file = 0;
    FILE* input_file = fopen("machine_commands.bin", "r");

    if (input_file == NULL)
    {
        printf("Can't open file\n");
    }

    fread(&size_of_file, sizeof(int), 1, input_file);
    double* machine_code = (double*) calloc(size_of_file, sizeof(double));
    fread(machine_code, sizeof(double), size_of_file, input_file);
    fclose(input_file);

    FILE* output_file = fopen("output.txt", "w");
    disassembler(machine_code, size_of_file, output_file);
    fclose(output_file);
}
