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
        fprintf(stderr, "Too few arguments\nUsage: ./eeptool <FILE> <DATA>\n");
        return 1;
    } else {
        if(argc > ARGS_COUNT) {
            fprintf(stderr, "Too many arguments\nUsage: ./eeptool <FILE> <DATA>\n");
            return 1;
        } else {
            int length = strlen(DATA);
            char *arg_data;
            arg_data = DATA;
            for(int i = 0; i < length; i++) {
                if(!isdigit(*arg_data)) {
                    fprintf(stderr, "Data contains illegal character: %c\n", *arg_data);
                    return 1;
                } else {
                    arg_data++;
                }
            }
            if(length > MAX_DIGITS) {
                fprintf(stderr, "Too many digits, maximum is MAX_DIGITS\n");
                return 1;
            } else { //Daten sind korrekt
                FILE *file;
                file = fopen(FILENAME, "wb");
                if(file == NULL) {
                    fprintf(stderr, "Could not acess %s\n", FILENAME);
                    return 1;
                } else {
                    int digit;
                    char *arg_digit;
                    arg_digit = DATA;
                    for(int i = 0; i < length; i++) {
                        digit = *arg_digit - '0'; //ASCII --> Int
                        fputc(digit, file);
                        arg_digit++;
                    }
                }
            }
        }
    }
    return 0;
}