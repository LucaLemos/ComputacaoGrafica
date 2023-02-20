#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <math.h>

char* getArquivo(int arquivo) {
    DIR *folder;
    struct dirent *entry;
    int files = 0;

    folder = opendir("objetos");
    if(folder == NULL) {
        perror("Diretorio nao existe");
    }

    while( (entry=readdir(folder)) ) {
        files++;
        if (arquivo == files) {
            return entry->d_name;
        }
    }

    closedir(folder);

    return NULL;
}

void lerAquivo(char *arquivo) {
    char filename[100] = "objetos/";
    strcat(filename, arquivo);

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: nao foi possivel ler o aquivo %s", filename);
    }

    // read one character at a time and
    // display it to the output
    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];

    while (fgets(buffer, MAX_LENGTH, fp)) {
        printf("%s", buffer);
    }

    // close the file
    fclose(fp);
}

void separarDados(char *nome, float dados[]) {
    char string[100] = "";
    strcat(string, nome);

    int contador = 0;

    char * token = strtok(string, " ");

    while( token != NULL ) {
        dados[contador] = strtod(token,NULL);
        //printf( " %s\n", token ); //printing each token
        token = strtok(NULL, " ");
        contador++;
    }
}

void carregaMalha(int arquivo, float teste[3][3][10000]) {
    int contador = 0;
    int contador1 = 0;
    int contador2 = 0;
    float dados[3];

    char filename[100] = "objetos/";
    strcat(filename, getArquivo(arquivo));

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: nao foi possivel ler o aquivo %s", filename);
    }

    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];

    while (fgets(buffer, MAX_LENGTH, fp)) {
        separarDados(buffer, dados);

        //printf("(%f %f %f)\n", dados[0], dados[1], dados[2]);

        float data1 = dados[0];
        float data2 = dados[1];
        
        if (contador == 0) {
            teste[0][0][0] = data1;
            teste[0][1][0] = data2;

        }else if (contador > 0 && contador <= teste[0][0][0]) {
            teste[1][0][contador1] = dados[0];
            teste[1][1][contador1] = dados[1];
            teste[1][2][contador1] = dados[2];

            contador1++;
        }else if (contador > teste[0][0][0]) {
            teste[2][0][contador2] = dados[0];
            teste[2][1][contador2] = dados[1];
            teste[2][2][contador2] = dados[2];

            contador2++;
        }

        contador++;
    }
    fclose(fp);
}

void carregaCamera(float teste[3][6]) {
    int contador = 0;
    float dados[10];

    char filename[100] = "parametros/camera.txt";

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error: nao foi possivel ler o aquivo %s", filename);
    }

    const unsigned MAX_LENGTH = 256;
    char buffer[MAX_LENGTH];

    while (fgets(buffer, MAX_LENGTH, fp)) {
        separarDados(buffer, dados);

        //printf("(%f %f %f)\n", dados[0], dados[1], dados[2]);

        teste[0][contador] = dados[2];
        teste[1][contador] = dados[3];
        teste[2][contador] = dados[4];

        contador++;
    }
    fclose(fp);
}
