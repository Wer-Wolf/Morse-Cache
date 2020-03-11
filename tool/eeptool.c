#include <ctype.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define ARGS_COUNT_READ 3
#define ARGS_COUNT_WRITE 4
#define ARGS_COUNT_MIN ARGS_COUNT_READ
#define ACTION argv[1]
#define FILENAME argv[2]
#define DATA argv[3]
#define MAX_DIGITS 61
#define MAX_DATA_LENGHT 62
#define DATA_END 0x00

#define DATA_MIN 0x30 //"0"
#define DATA_MAX 0x39 //"9"

#define is_illegal_data(data) ((data) > DATA_MAX || (data) < DATA_MIN)

/*... <FILE> <READ/WRITE> <DATA>*/

int main(int argc, char *argv[]) {
    printf("eeptool v1.1\n\n");
    if(argc < ARGS_COUNT_MIN) {
        fprintf(stderr, "ERROR: Too few arguments\nUsage: ./eeptool <read/write> <file> <data>\n");
        return 1;
    } else {
        if(strcmp(ACTION, "read") == 0) {
            if(argc == ARGS_COUNT_READ) { //Argumente korrekt
                FILE *file;
                file = fopen(FILENAME, "r");
                if(file == NULL) {
                    fprintf(stderr, "ERROR: Could not read %s\n", FILENAME);
                    return 1;
                } else {
                    printf("%s sucessfully opened\n", FILENAME);
                    printf("Content of %s: ", FILENAME);
                    int file_content = 0;
                    char byte = 0;
                    int digit_count = 0;
                    int i = 0;
                    for(i = 0; i <= MAX_DIGITS; i++) {
                        if((file_content = fgetc(file)) == EOF) { //Ende der Datei
                            break;
                        } else {
                            if(is_illegal_data(file_content)) {
                                if(file_content == DATA_END) {
                                    break;
                                } else {
                                    byte = '?'; //Unknown character
                                }
                            } else {
                                byte = file_content;
                                digit_count++;
                            }
                            printf("%c", byte);
                        }
                    }
                    fclose(file);
                    printf("\nNumber of digits: %d\n", digit_count);
                    if(i > digit_count) {
                        printf("\nWARNING: The eeprom image contains %d possibly damaged data entrys,\nplease consider creating a new eeprom image with 'write'\n\n", i - digit_count);
                    }
                    printf("Finished\n");
                }
            } else {
                fprintf(stderr, "ERROR: Incorrect using of action 'read'\nUsage: ./eeptool read <file>\n");
                return 1;
            }
        } else {
            if(strcmp(ACTION, "write") == 0) {
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
                        file = fopen(FILENAME, "w");
                        if(file == NULL) {
                            fprintf(stderr, "ERROR: Could not access %s\n", FILENAME);
                            return 1;
                        } else {
                            printf("Writing data to %s\n", FILENAME);
                            char digit;
                            for(int i = 0; i < length; i++) {
                                digit = *(DATA + i); //Get char
                                fputc(digit, file);
                                printf(".");
                            }
                            for(int i = 0; i < MAX_DATA_LENGHT - length; i++) {
                                fputc(DATA_END, file);
                                printf("_");
                            }
                            printf("\n");
                            fclose(file);
                            printf("Finished\n");
                        }
                    }
                } else {
                    fprintf(stderr, "ERROR: Incorrect using of action 'write'\nUsage: ./eeptool write <file> <data>\n");
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