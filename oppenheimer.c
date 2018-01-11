#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *expand(char *file, unsigned int fileSize){
    /*allocate maximum possible size of fileSize * 2 + 9*/
    char *newFile = (char *) malloc(((sizeof(char) * fileSize) * 2) + 9);
    /*insert header*/
    memmove(newFile, "OP1AVUXO", 8);

    int charIndex = 8;
    while(*file){
        newFile[charIndex++] = *file;
        /*if the value of the char is even then add it again + 12 (makes files more expanded)*/
        if((int) *file % 2 == 0){
            newFile[charIndex++] = *file+12;
        }
        file++;
    }

    return newFile;
}

char *compress(char *file, unsigned int fileSize){
    /*check for header*/
    if(strncmp(file, "OP1AVUXO", 8)){
        fprintf(stderr, "ERROR: Please supply a valid Oppenheimer file.\n");
    }

    /*maximum file size of fileSize * 2 +1 (no header)*/
    char *newFile = (char *) malloc(((sizeof(char) * fileSize) * 2) + 1);

    file += 8; /*skip header*/
    int charIndex = 0;
    while(*file){
        /*check if current byte is even, if so: ignore it. Could be an expanded byte after all, can't be sure.*/
        if((int) *file % 2 == 0){
            file++;
        } else {
            newFile[charIndex] = *file;
            charIndex++;
            file++;
        }
    }
    
    return newFile;
}

int main(int argc, char **argv){
    if(argc < 3){
        fprintf(stderr, "ERROR: Oppenheimer requires a file to expand and an operation\n");
        printf("Usage: oppenheimer <-e | -c> <filename>\n");
        exit(1);
    }

    const char *filename = argv[2];
    const char *operation = argv[1];


    FILE *file;
    unsigned int fileSize;

    /*get file size*/
    file = fopen(filename, "rb");
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    rewind(file);

    /*allocate memory for file size*/
    char *fileBuffer = (char *) malloc(sizeof(char) * (fileSize + 1));
    /*read into buffer*/
    fread(fileBuffer, fileSize, 1, file);
    fclose(file);

    /*pre-defined so the compiler will stop whining*/
    char *newFile;
    if(!strcmp(operation, "-c")){
        newFile = compress(fileBuffer, fileSize);
    } else if(!strcmp(operation, "-e")){
        newFile = expand(fileBuffer, fileSize);
    } else {
        fprintf(stderr, "ERROR: %s is not a valid operation!\n", operation);
        exit(1);
    }

    printf("%s\n", newFile);

    /*cleanup*/
    free(fileBuffer);
    free(newFile);
}
