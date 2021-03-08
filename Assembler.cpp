
#include "commands.h"

typedef struct array_of_var{

    char** name;
    int* pointer;
    int number_of_var;
} type_array_with_deffin_of_var;

typedef struct array_of_undef_var{

    char** name;
    double** var_point;
    int number_of_var;
} type_array_with_undef_var;

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
        //printf("in name '%s'\n", tmp);
    } else{

        printf("this isn't a link\n");
        exit(1);
    }

}

int check_dublicated_name(char* word, char** name, int number_of_links){

    int i = 0;

    //printf("----dafuq in check-(number of links = %i)--\n", number_of_links);

    while ( ( strcmp(word, name[i]) != 0 ) && ( i < number_of_links) ){

        //printf("     strcmp = %i with word = %s and mas = %s\n", strcmp(word, name[i]), word, name[i]);
        i++;
    }

    if (i == (number_of_links - 1) ) {

        return 1;
    } else{
        printf("in chaeck i == %i num of inks = %i\n", i, number_of_links);
        printf("redeclaration of '%s'\n", word);
        return 0;
    }
}

int find_register(char* word) {

    if ( (length(word) == 3) && (word[1] == '_') ) {

        if ( (word[0] == 'B') && (word[2] >= 'A') && (word[2] <= 'C') ) {

            return PUBL_B_A + (word[2] - 'A');
        }

        if ( (word[0] == 'W') && (word[2] >= 'A') && (word[2] <= 'D') ) {

            return PUBL_W_A + (word[2] - 'A');
        }

        if ( (word[0] == 'D') && (word[2] >= 'A') && (word[2] <= 'E') ) {

            return PUBL_DW_A + (word[2] - 'A');

        }

        if (word[0] == 'P') {

            if (word[2] == 'F') {

                return PRIV_F;
            }

            if (word[2] == 'R') {

                return PRIV_R;
            }

        }

    } else {

        printf("invalid register name [%s]\n", word);
        printf("Братан, ты регистры попутал\n");
        return -1;
    }

        printf("invalid register name [%s]\n", word);
        printf("Братан, ты регистры НЕ попутал\n");
        return -1;
}

void add_var(type_array_with_deffin_of_var* array_with_deffin_of_var, char* name_of_var, int data_pointer) {

    (*array_with_deffin_of_var).number_of_var++;

    (*array_with_deffin_of_var).name = (char**) realloc((*array_with_deffin_of_var).name, sizeof(char*) * (*array_with_deffin_of_var).number_of_var);
    (*array_with_deffin_of_var).name[(*array_with_deffin_of_var).number_of_var - 1] = name_of_var;
    assert ( check_dublicated_name(name_of_var, (*array_with_deffin_of_var).name, (*array_with_deffin_of_var).number_of_var) );

    (*array_with_deffin_of_var).pointer = (int*) realloc((*array_with_deffin_of_var).pointer, sizeof(int) * (*array_with_deffin_of_var).number_of_var);
    (*array_with_deffin_of_var).pointer[(*array_with_deffin_of_var).number_of_var - 1] = data_pointer;
}

void add_undef_var(type_array_with_undef_var* array_with_undef_var, char* name_of_undef_var, int code_pointer, double* machine_code){

    (*array_with_undef_var).number_of_var++;
    (*array_with_undef_var).name = (char**) realloc( (*array_with_undef_var).name, sizeof(char*) * (*array_with_undef_var).number_of_var);
    (*array_with_undef_var).name[(*array_with_undef_var).number_of_var - 1] = name_of_undef_var;

    (*array_with_undef_var).var_point = (double**) realloc( (*array_with_undef_var).var_point, sizeof(double*) * (*array_with_undef_var).number_of_var);
    (*array_with_undef_var).var_point[ (*array_with_undef_var).number_of_var - 1] = &machine_code[code_pointer];

}

void add_jmp(array_jmps* jmps_array, char* link_name, int code_pointer, double* machine_code){

    (*jmps_array).number_of_jmps++;
    (*jmps_array).name = (char**) realloc( (*jmps_array).name, sizeof(char*) * (*jmps_array).number_of_jmps);
    (*jmps_array).name[(*jmps_array).number_of_jmps - 1] = link_name;

    (*jmps_array).jmp_point = (double**) realloc( (*jmps_array).jmp_point, sizeof(double*) * (*jmps_array).number_of_jmps);
    (*jmps_array).jmp_point[(*jmps_array).number_of_jmps - 1] = &machine_code[code_pointer];

}

//void add_jmp(array_jmps* jmps_array, )

double* assembler(char name_of_input_file[MAXLEN], int* num_of_comands) {

    setlocale( LC_ALL,"Russian" );

    int  code_pointer = 0, data_pointer = 0, i = 0, j = 0, not_a_command = 0, var_was_def = 0, right_registr = 0; //надо сделать переменные только номером ячейки в оперативной памяти и при операциях смотреть на тип

    char* buffer = fill_buffer(name_of_input_file, num_of_comands);
    char delim[4] = " ";
    char* word = strtok(buffer, delim);
    char* name_of_var = NULL;

    double* machine_code = (double*) calloc(*num_of_comands + 2, sizeof(double)); //в массиве машинных команд содержится количество СЛОВ

    type_array_with_deffin_of_var array_with_deffin_of_var;
    type_array_with_undef_var array_with_undef_var;
    array_links links_array;
    array_jmps jmps_array;

    array_with_deffin_of_var.number_of_var = 0;
    array_with_deffin_of_var.name = NULL;
    array_with_deffin_of_var.pointer = NULL;

    array_with_undef_var.number_of_var = 0;
    array_with_undef_var.name = NULL;
    array_with_undef_var.var_point = NULL;

    jmps_array.number_of_jmps = 0;
    jmps_array.jmp_point = NULL;
    jmps_array.name = NULL;

    links_array.number_of_links = 0;
    links_array.name = NULL;
    links_array.command_number = NULL;

    while (NULL != word) {

        not_a_command = 1;

        printf("\n dump machine code = %i word = %s\n", code_pointer, word);

        if (word[0] == '@') {

            name_of_link(word);
            word = word + 1;
            links_array.number_of_links++;

            links_array.name = (char**) realloc(links_array.name, sizeof(char*) * links_array.number_of_links);
            links_array.name[links_array.number_of_links - 1] = word;
            assert ( check_dublicated_name(word, links_array.name, links_array.number_of_links) );

            printf("added word '%s' number %i\n", word, links_array.number_of_links - 1);
            //printf("words pointer = %i\n", word);

            links_array.command_number = (int*) realloc(links_array.command_number, sizeof(int) * links_array.number_of_links);
            links_array.command_number[links_array.number_of_links - 1] = code_pointer;

            not_a_command = 0;
        }

        if (strcmp(word, "MOV") == 0) {

            code_pointer++;

            word = strtok(NULL, delim);

            if (find_register(word) != -1){

                machine_code[code_pointer] = find_register(word);
                code_pointer++;

                word = strtok(NULL, delim);

                if ( ( (word[0] >= '0') && (word[0] <= '9') ) || ((word[1] >= '0') && (word[1] <= '9')) ) {

                    machine_code[code_pointer] = strtod(word, NULL);
                    machine_code[code_pointer - 2] = CPU_MOV_RD;
                    printf("in mov RD%lf\n", machine_code[code_pointer]);
                    code_pointer++;

                } else {

                    add_undef_var(&array_with_undef_var, word, code_pointer, machine_code);

                    machine_code[code_pointer - 2] = CPU_MOV_RV;
                    code_pointer++;

                    word = strtok(NULL, delim);
                    if ( (word[0] >= '0') && (word[0] <= '9') ){

                        machine_code[code_pointer] = strtod(word, NULL);//добавлене модификатора
                    } else {
                        machine_code[code_pointer] = 0;
                    }
                    code_pointer++;

                }

            } else {

                machine_code[code_pointer - 1] = CPU_MOV_VR;

                add_undef_var(&array_with_undef_var, word, code_pointer, machine_code);
                code_pointer++;

                word = strtok(NULL, delim);
                if ( (word[0] >= '0') && (word[0] <= '9') ){

                    machine_code[code_pointer] = strtod(word, NULL);//добавлене модификатора
                } else {
                    machine_code[code_pointer] = 0;
                }

                code_pointer++;

                assert( find_register(word) != -1);
                machine_code[code_pointer] = find_register(word);
                code_pointer++;

            }

        not_a_command = 0;
        }

        if (strcmp(word, "PUSH") == 0) {

            machine_code[code_pointer] = CPU_PUSH;
            code_pointer++;

            word = strtok(NULL, delim);

            assert( find_register(word) != -1);
            machine_code[code_pointer] = find_register(word);
            code_pointer++;

            not_a_command = 0;
        }

        if (strcmp(word, "IN") == 0) {

            machine_code[code_pointer] = CPU_IN;
            code_pointer++;

            word = strtok(NULL, delim);

            assert( find_register(word) != -1);
            machine_code[code_pointer] = find_register(word);
            code_pointer++;

            not_a_command = 0;
        }

        if (strcmp(word, "ADD") == 0) {

            machine_code[code_pointer] = CPU_ADD;
            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "SUB") == 0) {

            machine_code[code_pointer] = CPU_SUB;
            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "DIV") == 0) {

            machine_code[code_pointer] = CPU_DIV;
            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "MUL") == 0) {

            machine_code[code_pointer] = CPU_MUL;
            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "FSQRT") == 0) {

            machine_code[code_pointer] = CPU_FSQRT;
            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "POP") == 0) {

            machine_code[code_pointer] = CPU_POP;
            code_pointer++;

            word = strtok(NULL, delim);

            assert( find_register(word) != -1);
            machine_code[code_pointer] = find_register(word);
            code_pointer++;

            not_a_command = 0;
        }

        if (strcmp(word, "JL") == 0) {
            printf("jl code pointer = %i\n", code_pointer);
            machine_code[code_pointer] = CPU_JL;
            code_pointer++;

            word = strtok(NULL, delim);

            add_jmp(&jmps_array, word, code_pointer, machine_code);

            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "JG") == 0) {

            machine_code[code_pointer] = CPU_JG;
            code_pointer++;

            word = strtok(NULL, delim);

            add_jmp(&jmps_array, word, code_pointer, machine_code);

            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "JE") == 0) {

            machine_code[code_pointer] = CPU_JE;
            code_pointer++;

            word = strtok(NULL, delim);

            add_jmp(&jmps_array, word, code_pointer, machine_code);

            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "JN") == 0) {

            machine_code[code_pointer] = CPU_JN;
            code_pointer++;

            word = strtok(NULL, delim);

            add_jmp(&jmps_array, word, code_pointer, machine_code);

            not_a_command = 0;
        }

        if (strcmp(word, "CMP") == 0) {

            machine_code[code_pointer] = CPU_CMP;
            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "OUT") == 0) {

            machine_code[code_pointer] = CPU_OUT;
            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "HLT") == 0) {

            machine_code[code_pointer] = CPU_HLT;
            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "END") == 0) {

            machine_code[code_pointer] = END;
            code_pointer++;
            not_a_command = 0;
        }

        if (strcmp(word, "NOP") == 0) {

            machine_code[code_pointer] = NOP;
            code_pointer++;
            not_a_command = 0;
        }

        //мне лень делать норм отступы и для перестраховки пока будет так
        if (strcmp(word, "-") == 0) {

            machine_code[code_pointer] = NOP;
            code_pointer++;
            not_a_command = 0;
        }
        //потом надо добавить возможность комментов

        if (not_a_command == 1) {

            printf("dafuq in 506 line word = |%s|\n", word);
            name_of_var = word;

            word = strtok(NULL, delim);
            var_was_def = 0;

            if (strcmp(word, "DB") == 0) { //byte

                var_was_def = 1;

                add_var(&array_with_deffin_of_var, name_of_var, data_pointer);

                data_pointer += 1;
            }

            if (strcmp(word, "DW") == 0) { //word 2 bytes

                var_was_def = 1;

                add_var(&array_with_deffin_of_var, name_of_var, data_pointer);

                data_pointer += 2;
            }

            if (strcmp(word, "DD") == 0) { //double word 4 bytes

                var_was_def = 1;

                add_var(&array_with_deffin_of_var, name_of_var, data_pointer);

                data_pointer += 4;
            }

            if (strcmp(word, "DMAS") == 0) {

                var_was_def = 1;

                add_var(&array_with_deffin_of_var, name_of_var, data_pointer);

                word = strtok(NULL, delim);
                data_pointer += strtod(word, NULL);
            }

            if (var_was_def == 0){

                printf("strange command |%s|\n", word);
                printf("command pointer = %i\n", code_pointer);



                printf("Я знаю много мудаков, но таких, охуенно тупых, как ты, не видал ещё ни разу\n");
                exit(7);
            }

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

    for (i = 0; i < array_with_undef_var.number_of_var; i++) {

        j = 0;
        while ( (j < array_with_deffin_of_var.number_of_var) && (strcmp( array_with_undef_var.name[i], array_with_deffin_of_var.name[j] ) != 0 ) ) {

            j++;
        }

        if (j == array_with_deffin_of_var.number_of_var) {

            printf("Can't find deffin of var |%s|", array_with_undef_var.name[i]);
            exit(10);
        } else
        {
            *(array_with_undef_var.var_point[i]) = array_with_deffin_of_var.pointer[j];
        }
    }


    printf("machine code[68] = %lf and 69 = %lf\n", machine_code[68], machine_code[69]);
    /*for (int k = 0; k <= code_pointer; k++){
        printf("|%lf|", machine_code[k]);
    }*/

    return machine_code;
}

int main() {

    char name_of_input_file[MAXLEN] = "input.txt";
    int num_of_comands = 0;
    double* machine_code = assembler(name_of_input_file, &num_of_comands);

    FILE* output_file = fopen("machine_commands.bin", "wb");

    if (output_file == NULL){

        printf("HELP ME C IS TRYING TO FUCK ME\n");
    }

    printf("printf num_of_comands %i\n", num_of_comands);
    fwrite(&num_of_comands, sizeof(int), 1, output_file);
    fwrite(machine_code, sizeof(double), num_of_comands, output_file);

    free(machine_code);
    fclose(output_file);


    FILE* input_file = fopen("machine_commands.bin", "rb");

    if (input_file == NULL)
    {
        printf("Can't open file\n");
    }

    int size_of_file = 0;
    double tmp = -1;
    fread(&size_of_file, sizeof(int), 1, input_file);

    for (int i = 0; i < 5; i++){

        fread(&tmp, sizeof(double), 1, input_file);
        printf("{%lf}", tmp);
    }
    //CPU.machine_code = (double*) calloc(size_of_file, sizeof(double));
    printf("in open size = %i\n", size_of_file);

    //fread(CPU.machine_code, sizeof(double), size_of_file, input_file);

    fclose(input_file);

}
