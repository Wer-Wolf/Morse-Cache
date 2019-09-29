#include <ctype.h>
#include <string.h>
#include <stdio.h>

#define ARGS_COUNT 3
#define FILENAME argv[1]
#define DATA argv[2]
#define MAX_DIGITS 62

/*... <FILE> <DATA>*/

int main(int argc, char *argv[]) {
    if(argc < ARGS_COUNT) {
        fprintf(stderr, "ERROR: Too few arguments\nUsage: ./eeptool <FILE> <DATA>\n");
        return 1;
    } else {
        if(argc > ARGS_COUNT) {
            fprintf(stderr, "ERROR: Too many arguments\nUsage: ./eeptool <FILE> <DATA>\n");
            return 1;
        } else { //Argumente korrekt
            if(strlen(DATA) > MAX_DIGITS) {
                fprintf(stderr, "ERROR: Too much data, maximum is %d\n", MAX_DIGITS);
                return 1;
            } else {
                int length = strlen(DATA);
                for(int i = 0; i < length; i++) {
                    if(!isdigit(*(DATA + i))) {
                        fprintf(stderr, "ERROR: Data contains illegal character: %c\n", *(DATA + i));
                        return 1;
                    }
                }
                //Daten sind korrekt
                printf("Data sucessfully loaded: %s\nNummber of digits: %d\n", DATA, length);
                FILE *file;
                file = fopen(FILENAME, "wb");
                if(file == NULL) {
                    fprintf(stderr, "ERROR: Could not access %s\n", FILENAME);
                    return 1;
                } else {
                    printf("Writing data to %s\n", FILENAME);
                    int digit;
                    for(int i = 0; i < length; i++) {
                        digit = *(DATA + i) - '0'; //ASCII --> Int
                        fputc(digit, file);
                        printf(".");
                    }
                    printf("\n");
                    fclose(file);
                    printf("Finished\n");
                }
            }
        }
    }
    return 0;
}