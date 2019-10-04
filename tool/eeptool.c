#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define ARGS_COUNT_READ 3
#define ARGS_COUNT_WRITE 4
#define ARGS_COUNT_MIN ARGS_COUNT_READ
#define FILENAME argv[1]
#define ACTION argv[2]
#define DATA argv[3]
#define MAX_DIGITS 62

/*... <FILE> <READ/WRITE> <DATA>*/

int main(int argc, char *argv[]) {
    if(argc < ARGS_COUNT_MIN) {
        fprintf(stderr, "ERROR: Too few arguments\nUsage: ./eeptool <FILE> <READ/WRITE> <DATA>\n");
        return 1;
    } else {
        if(strcmp(ACTION, "READ") == 0) {
            if(argc == ARGS_COUNT_READ) { //Argumente korrekt
                FILE *file;
                file = fopen(FILENAME, "rb");
                if(file == NULL) {
                    fprintf(stderr, "ERROR: Could not read %s\n", FILENAME);
                    return 1;
                } else {
                    printf("%s sucessfully opened\n", FILENAME);
                    printf("Content of %s: ", FILENAME);
                    int i;
                    char digit;
                    for(i = 0; i <= MAX_DIGITS; i++) {
                        digit = fgetc(file);
                        if(digit == EOF) { //Ende der Datei
                            fclose(file);
                            printf("\nNumber of digits: %d\nFinished\n", i);
                            return 0;
                        } else {
                            digit = digit +  '0'; //Nach char umwandeln
                            printf("%c", digit);
                        }
                    }
                }
            } else {
                fprintf(stderr, "ERROR: Incorrect using of action READ\nUsage: ./eeptool <FILE> <READ>\n");
                return 1;
            }
        } else {
            if(strcmp(ACTION, "WRITE") == 0) {
                if(argc == ARGS_COUNT_WRITE) { //Argumente korrekt
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
                        printf("Data sucessfully loaded: %s\nNumber of digits: %d\n", DATA, length);
                        FILE *file;
                        file = fopen(FILENAME, "wb");
                        if(file == NULL) {
                            fprintf(stderr, "ERROR: Could not access %s\n", FILENAME);
                            return 1;
                        } else {
                            printf("Writing data to %s\n", FILENAME);
                            uint8_t digit;
                            for(int i = 0; i < length; i++) {
                                digit = *(DATA + i) - '0'; //ASCII --> uint8_t
                                fputc(digit, file);
                                printf(".");
                            }
                            printf("\n");
                            fclose(file);
                            printf("Finished\n");
                        }
                    }
                } else {
                    fprintf(stderr, "ERROR: Incorrect using of action WRITE\nUsage: ./eeptool <FILE> <WRITE> <DATA>\n");
                    return 1;
                }
            } else {
                fprintf(stderr, "ERROR: Unknown action %s\n", ACTION);
                return 1;
            }
        }
    }
    return 0;
}