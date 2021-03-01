
#include "commands.h"

typedef struct array_of_links{

    char** name;
    int* command_number;
    int number_of_links;
} array_links;

typedef struct array_of_jmps{

    char** name;
    double** jmp_point;
    int number_of_jmps;
} array_jmps;

char* fill_buffer(char name_of_input_file[MAXLEN], int* word_counter) {
    int i = 0;

    struct stat statistika;
    stat(name_of_input_file, &statistika);

    char* buffer = (char*) calloc(statistika.st_size + 1, sizeof(char));

    if (buffer == NULL) {

        printf("trobles with memory\n");
        return NULL;
    }
    FILE* p_input = fopen(name_of_input_file, "r");

    if (p_input == NULL) {
        return NULL;
    }

    char symbol = fgetc(p_input);

    while (symbol != EOF) {

        if ((symbol != '\n') && (symbol != '\t')) {

            if (symbol == ' ') {
                (*word_counter)++;
            }

            buffer[i] = symbol;
            i++;
        }
        else {

            buffer[i] = ' ';
            i++;
            (*word_counter)++;
        }

        symbol = fgetc(p_input);
    }
    (*word_counter)++;

    return buffer;
}


int length(char* line){

    int i = 0;

    while (line[i] != '\0'){

        i++;
    }

    return i;
}

void name_of_link(char* tmp){

    int i = 0, len = length(tmp);

    if (tmp[len - 1] == ':'){

        tmp[len - 1] = '\0';
        printf("in name '%s'\n", tmp);
    } else{

        printf("this isn't a link\n");
        exit(1);
    }

}

int check_dublicated_name(char* word, array_links links_array){

    int i = 0;

    while ( ( strcmp(word, links_array.name[i]) != 0 ) && ( i < links_array.number_of_links) ){

        i++;
    }

    if (i != links_array.number_of_links) {

        return 1;
    } else{
        printf("in chaeck i == %i num of inks = %i\n", i, links_array.number_of_links);
        printf("redeclaration of '%s'\n", word);
        return 0;
    }
}


double* assembler(char name_of_input_file[MAXLEN], int* num_of_comands) {

    int  code_pointer = 0, i = 0, j = 0;

    char* buffer = fill_buffer(name_of_input_file, num_of_comands);
    char delim[4] = " ";
    char* word = strtok(buffer, delim);
    double* machine_code = (double*)calloc(*num_of_comands + 2, sizeof(double)); //в массиве машинных команд содержится количество СЛОВ

    array_links links_array;
    array_jmps jmps_array;

    jmps_array.number_of_jmps = 0;
    jmps_array.jmp_point = NULL;
    jmps_array.name = NULL;

    links_array.number_of_links = 0;
    links_array.name = NULL;
    links_array.command_number = NULL;

    while (NULL != word) {

        if (word[0] == '@') {

            name_of_link(word);
            word = word + 1;
            links_array.number_of_links++;

            links_array.name = (char**) realloc(links_array.name, sizeof(char*) * links_array.number_of_links);
            links_array.name[links_array.number_of_links - 1] = word;
            assert ( check_dublicated_name(word, links_array) );

            printf("added word '%s' number %i\n", word, links_array.number_of_links - 1);

            links_array.command_number = (int*) realloc(links_array.command_number, sizeof(int) * links_array.number_of_links);
            links_array.command_number[links_array.number_of_links - 1] = code_pointer;
        }

        if (strcmp(word, "PUSH") == 0) {

            machine_code[code_pointer] = CPU_PUSH;
            code_pointer++;

            word = strtok(NULL, delim);
            machine_code[code_pointer] = strtod(word, NULL);
            code_pointer++;
        }

        if (strcmp(word, "IN") == 0) {

            machine_code[code_pointer] = CPU_IN;
            code_pointer++;
        }

        if (strcmp(word, "ADD") == 0) {

            machine_code[code_pointer] = CPU_ADD;
            code_pointer++;
        }

        if (strcmp(word, "SUB") == 0) {

            machine_code[code_pointer] = CPU_SUB;
            code_pointer++;
        }

        if (strcmp(word, "DIV") == 0) {

            machine_code[code_pointer] = CPU_DIV;
            code_pointer++;
        }

        if (strcmp(word, "MUL") == 0) {

            machine_code[code_pointer] = CPU_MUL;
            code_pointer++;
        }

        if (strcmp(word, "FSQRT") == 0) {

            machine_code[code_pointer] = CPU_FSQRT;
            code_pointer++;
        }

        if (strcmp(word, "PUSH_DX") == 0) {

            machine_code[code_pointer] = CPU_PUSH_DX;
            code_pointer++;
        }

        if (strcmp(word, "POP_DX") == 0) {

            machine_code[code_pointer] = CPU_POP_DX;
            code_pointer++;
        }

        if (strcmp(word, "JL") == 0) {

            machine_code[code_pointer] = CPU_JL;
            code_pointer++;

            word = strtok(NULL, delim);

            jmps_array.number_of_jmps++;
            jmps_array.name = (char**) realloc(jmps_array.name, sizeof(char*) * jmps_array.number_of_jmps);
            jmps_array.name[jmps_array.number_of_jmps - 1] = word;

            jmps_array.jmp_point = (double**) realloc(jmps_array.jmp_point, sizeof(double*) * jmps_array.number_of_jmps);
            jmps_array.jmp_point[jmps_array.number_of_jmps - 1] = &machine_code[code_pointer];
            code_pointer++;


        }

        if (strcmp(word, "JG") == 0) {

            machine_code[code_pointer] = CPU_JG;
            code_pointer++;

            word = strtok(NULL, delim);

            jmps_array.number_of_jmps++;
            jmps_array.name = (char**) realloc(jmps_array.name, sizeof(char*) * jmps_array.number_of_jmps);
            jmps_array.name[jmps_array.number_of_jmps - 1] = word;

            jmps_array.jmp_point = (double**) realloc(jmps_array.jmp_point, sizeof(double*) * jmps_array.number_of_jmps);
            jmps_array.jmp_point[jmps_array.number_of_jmps - 1] = &machine_code[code_pointer];
            code_pointer++;
        }

        if (strcmp(word, "JE") == 0) {

            machine_code[code_pointer] = CPU_JE;
            code_pointer++;

            word = strtok(NULL, delim);

            jmps_array.number_of_jmps++;
            jmps_array.name = (char**) realloc(jmps_array.name, sizeof(char*) * jmps_array.number_of_jmps);
            jmps_array.name[jmps_array.number_of_jmps - 1] = word;

            jmps_array.jmp_point = (double**) realloc(jmps_array.jmp_point, sizeof(double*) * jmps_array.number_of_jmps);
            jmps_array.jmp_point[jmps_array.number_of_jmps - 1] = &machine_code[code_pointer];
            code_pointer++;
        }

        if (strcmp(word, "JN") == 0) {

            machine_code[code_pointer] = CPU_JN;
            code_pointer++;

            word = strtok(NULL, delim);

            jmps_array.number_of_jmps++;
            jmps_array.name = (char**) realloc(jmps_array.name, sizeof(char*) * jmps_array.number_of_jmps);
            jmps_array.name[jmps_array.number_of_jmps - 1] = word;

            jmps_array.jmp_point = (double**) realloc(jmps_array.jmp_point, sizeof(double*) * jmps_array.number_of_jmps);
            jmps_array.jmp_point[jmps_array.number_of_jmps - 1] = &machine_code[code_pointer];
            code_pointer++;
        }

        if (strcmp(word, "CMP") == 0) {

            machine_code[code_pointer] = CPU_CMP;
            code_pointer++;
        }

        if (strcmp(word, "OUT") == 0) {

            machine_code[code_pointer] = CPU_OUT;
            code_pointer++;
        }

        if (strcmp(word, "HLT") == 0) {

            machine_code[code_pointer] = CPU_HLT;
            code_pointer++;
        }

        if (strcmp(word, "END") == 0) {

            machine_code[code_pointer] = END;
            code_pointer++;
        }

        word = strtok(NULL, delim);
    }
    *num_of_comands = code_pointer;


    for (i = 0; i < jmps_array.number_of_jmps; i++){

        j = 0;
        while ( ( j < links_array.number_of_links ) && ( strcmp( jmps_array.name[i], links_array.name[j] ) != 0 ) ){


            printf("in while jmps = '%s' links = '%s'\n", jmps_array.name[i], links_array.name[j] );
            j++;
        }

        if (j == links_array.number_of_links){

            printf("in assembl j == %i\n", j);
            printf("first in mas = '%s'\n", links_array.name[0]);
            printf("strcmp(links, word) = %i\n", strcmp(links_array.name[0], jmps_array.name[i] ) );
            printf("can't find link '%s'\n", jmps_array.name[i]);
            exit(4);
        } else{

            *(jmps_array.jmp_point[i]) = links_array.command_number[j];
        }
    }

    return machine_code;
}

int main() {

    char name_of_input_file[MAXLEN] = "input.txt";
    int num_of_comands = 0;
    double* machine_code = assembler(name_of_input_file, &num_of_comands);

    FILE* output_file = fopen("machine_commands.bin", "w");

    fwrite(&num_of_comands, sizeof(int), 1, output_file);
    fwrite(machine_code, sizeof(double), num_of_comands, output_file);

    free(machine_code);
    fclose(output_file);
}
