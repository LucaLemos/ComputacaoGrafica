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
/*
void rasterizar(float P1x, float P1y, float P2x, float P2y, float P3x, float P3y) {
    float ponto1X = P1x;
    float ponto1Y = P1y;
    float ponto2X = P2x;
    float ponto2Y = P2y;
    float ponto3X = P3x;
    float ponto3Y = P3y;

    if (ponto1Y > ponto3Y) {
        float tmp = ponto3Y;
        ponto3Y = ponto1Y;
        ponto1Y = tmp;

        tmp = ponto3X;
        ponto3X = ponto1X;
        ponto1X = tmp;
    }
    if (ponto1Y > ponto2Y) {
        float tmp = ponto2Y;
        ponto2Y = ponto1Y;
        ponto1Y = tmp;

        tmp = ponto2X;
        ponto2X = ponto1X;
        ponto1X = tmp;
    }
    if (ponto2Y > ponto3Y) {
        float tmp = ponto3Y;
        ponto3Y = ponto2Y;
        ponto2Y = tmp;

        tmp = ponto3X;
        ponto3X = ponto2X;
        ponto2X = tmp;
    }

    float pontoCima[2] = {ponto1X, ponto1Y};
    float pontoMedio[2] = {ponto2X, ponto2Y};
    float pontoBaixo[2] = {ponto3X, ponto3Y};

    printf("pontoCima X: %f pontoCima Y: %f\n", pontoCima[0], pontoCima[1]);
    printf("pontoMedio X: %f pontoMedio Y: %f\n", pontoMedio[0], pontoMedio[1]);
    printf("pontoBaixo X: %f pontoBaixo Y: %f\n", pontoBaixo[0], pontoBaixo[1]);

    float Yscan = pontoCima[1];
    float Xmin = pontoCima[0];
    float Xmax = pontoCima[0];

    float a1;
    float a2;

    float temp = (pontoMedio[0] - pontoCima[0]);
    if (temp == 0) {
        a1 = 0;
    }else {
        a1 = (pontoMedio[1] - pontoCima[1])/(pontoMedio[0] - pontoCima[0]); 
        if (!a1 == 0) {
            a1 = (1/a1);
        }
    }
    temp = (pontoBaixo[0] - pontoCima[0]);
    if (temp == 0) {
        a2 = 0;
    }else {
        a2 = (pontoBaixo[1] - pontoCima[1])/(pontoBaixo[0] - pontoCima[0]);
        if (!a2 == 0) {
            a2 = (1/a2);
        }
    }
    printf("a1: %f a2: %f\n\n", a1, a2);
    printf("pontoMedio: %f pontoMedio: %f\n\n", pontoMedio[0], pontoMedio[0]);

    if(pontoMedio[0] > pontoBaixo[0]) {
        float troca = a1;
        a1 = a2;
        a2 = troca;
    }

    float Xmin_;
    float Xmax_;

    printf("a1: %f a2: %f\n\n", a1, a2);

    while (Yscan <= pontoMedio[1]) {
        Xmin_ = (Xmin + a1);
        Xmax_ = (Xmax + a2);

        //printf("Xmin: %f Xmax: %f\n\n", Xmin, Xmax);

        Yscan++;
        Xmin = Xmin_;
        Xmax = Xmax_;
    }
    Xmin = (Xmin - a1);
    Xmax = (Xmax - a2);

    printf("Xmin: %f Xmax: %f\n", Xmin, Xmax);
    printf("pontoMedio[0]: %f Xmax: %f\n", pontoMedio[0], Xmax);
    float pontoD[2];
    if((int)pontoMedio[0] > (int)pontoBaixo[0]) {
        pontoD[0] = Xmin; 
        pontoD[1] = pontoMedio[1];
    }else {
        pontoD[0] = Xmax; 
        pontoD[1] = pontoMedio[1];
    }

    printf("pontoD X: %f pontoD Y: %f\n", pontoD[0], pontoD[1]);

    Yscan = pontoBaixo[1];
    Xmin = pontoBaixo[0];
    Xmax = pontoBaixo[0];

    temp = (pontoMedio[0] - pontoBaixo[0]);
    if (temp == 0) {
        a1 = 0;
    }else {
        a1 = (pontoMedio[1] - pontoBaixo[1])/(pontoMedio[0] - pontoBaixo[0]);
        if (!a1 == 0) {
            a1 = (1/a1);
        } 
    }
    temp = (pontoD[0] - pontoBaixo[0]);
    printf("temp: %f a2: %f\n\n", a1, a2);
    if (temp == 0) {
        a2 = 0;
    }else {
        printf("temp: %f a2: %f\n\n", a1, a2);
        a2 = (pontoD[1] - pontoBaixo[1])/(pontoD[0] - pontoBaixo[0]); 
        printf("a1: %f a2: %f\n\n", a1, a2);
        if (!a2 == 0) {
            a2 = (1/a2);
        } 
        printf("a1: %f a2: %f\n\n", a1, a2);
    }

    if(pontoMedio[0] > pontoD[0]) {
        float troca = a1;
        a1 = a2;
        a2 = troca;
    }

    
    printf("Yscan: %f pontoMedio: %f\n\n", Yscan, pontoMedio[1]);
    while (Yscan >= pontoMedio[1]) {
        Xmin_ = (Xmin - a1);
        Xmax_ = (Xmax - a2);

        //printf("Yscan: %f\n", Yscan);
        //printf("2: Xmin: %f Xmax: %f\n\n", Xmin, Xmax);

        Yscan--;
        Xmin = Xmin_;
        Xmax = Xmax_;
    }
}

int main() {
    //rasterizar(200, 300, 400, 5, 600, 100);
    rasterizar(20, 20, 100, 10, 20, 100);
}
*/