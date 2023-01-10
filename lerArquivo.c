#include <stdio.h>
#include <string.h>
#include <dirent.h>

char* getArquivo(int arquivo);
void lerAquivo(char *arquivo);

int main() {

    lerAquivo(getArquivo(7));

    return 0;
}

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
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        printf("%c", ch);
    }

    // close the file
    fclose(fp);
}

