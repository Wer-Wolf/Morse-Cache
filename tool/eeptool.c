#include <argp.h>
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../lib/morse.h"
#include "../util/address.h"

#define MAX_DIGITS (DATA_END_ADDRESS - DATA_START_ADDRESS)
#define MAX_DATA_LENGTH (MAX_DIGITS + 1)

const char *argp_program_version = "Version 3.0 (" __DATE__ " " __TIME__ ")";
const char *argp_program_bug_address = "www.github.com/Wer-Wolf/Morse-Cache/issues";

struct arguments {
	enum {WRITE, READ} action;
	char *numbers;
	char *filename;
};

struct argp_option options[] = {
	{"write", 'w', "numbers", 0, "Convert provided number sequence into a EEPROM image"},
	{0}
};

bool is_nummeric(char *string) {
	size_t number_count = 0;
	size_t len = strlen(string);

	for(size_t i = 0; i <= len; i++) {
		if(!isdigit(*(string + i))) {
			break;
		} else {
			number_count++;
		}
	}
	if(number_count != len || len == 0) {
		return false;
	} else {
		return true;
	}
}
			

static int parse_opt(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;
	
	switch(key) {
		case 'w':
			if(arguments->action == WRITE) { //Nur einmal -w
				argp_failure(state, 1, 0, "Too many arguments.");
			} else if (!is_nummeric(arg)) {
				argp_failure(state, 1, 0, "Only numbers are supported.");
			} else if (strlen(arg) > MAX_DIGITS) {
				argp_failure(state, 1, 0, "Too many numbers (max is %d).", MAX_DIGITS);
			} else {
				arguments->action = WRITE;
				arguments->numbers = arg;
			}
			break;
		case ARGP_KEY_ARG:
			if(state->arg_num >= 1) {
				//argp_failure (state, 1, 0, "Too many arguments");
				argp_usage(state);
			}
			arguments->filename = arg;
			break;
		case ARGP_KEY_END:
			if(state->arg_num < 1) {
				//argp_failure (state, 1, 0, "Too few arguments");
				argp_usage(state);
			}
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

void read_from_eeprom(FILE *file) {
	int digits_count = 0;
	int unknown_count = 0;
	bool end_detected = false;

	for(int i = 0; i <= MAX_DATA_LENGTH; i++) {
		int character = fgetc(file);
		if(character == EOF) {
			break;
		}
		if(is_illegal_data(character)) {
			if(character == END_OF_DATA) {
				end_detected = true;
				break;
			} else {
				printf("?"); //Unbekanntes Zeichen
				unknown_count++;
			}
		} else {
			printf("%c", character);
			digits_count++;
		}
		}
	printf("\nDigits: %d\n", digits_count);
	if(unknown_count > 0) {
		printf("Damaged: %d\n", unknown_count);
	}
	if(end_detected == false) {
		printf("Limiter missing: true\n");
	}
	if(unknown_count || !end_detected) {
		printf("WARNING: The EEPROM image contains possibly damaged data entrys and/or is missing the limiter.\n");
		printf("\nPlease consider creating a new EEPROM image.\n");
	}
}

void write_to_eeprom(char *content, FILE *file) {
	int length = strlen(content);
	
	for(int i = 0; i < length; i++) {
		fputc(*(content + i), file);
		printf(".");
	}
	for(int i = 0; i < MAX_DATA_LENGTH - length; i++) {
		fputc(END_OF_DATA, file);
		printf("_");
	}
	printf("\n");
}

int main(int argc, char **argv) {
	struct argp parser = {options, parse_opt, "File", "Read/Create EEPROM images for the Morse-Cache."};
	struct arguments arguments = {READ, 0, 0};
	FILE *file = 0;

	if(argp_parse(&parser, argc, argv, 0, 0, &arguments)) {
		exit(EXIT_FAILURE);
	}
	file = fopen(arguments.filename, arguments.action == READ ? "r": "w");
	if(!file) {
		perror(arguments.filename);
		exit(EXIT_FAILURE);
	}
	if(arguments.action == READ) {
		printf("Content of %s: ", arguments.filename);
		read_from_eeprom(file);
	} else {
		printf("Writing data to %s...\n", arguments.filename);
		write_to_eeprom(arguments.numbers, file);
	}
	printf("\nFinished\n");
	fclose(file);

	exit(EXIT_SUCCESS);
}
