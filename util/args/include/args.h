#ifndef ARGS_H
#define ARGS_H

#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	BIN,
	OCT,
	DEC,
	HEX
} arg_formats_t;

typedef struct {
	int cols;
	int len;
	int skip;
	bool color;
	bool version;
	arg_formats_t format;
	char *input;
} arguments_t;

const static char doc[] = "\n"
					" .d8888b.  888\n"
					"d88P  Y88b 888\n"
					"Y88b.      888\n"
					" \"Y888b.   88888b.   .d88b.  888  888 888  888\n"
					"    \"Y88b. 888 \"88b d8P  Y8b `Y8bd8P\' 888  888\n"
					"      \"888 888  888 88888888   X88K   888  888\n"
					"Y88b  d88P 888  888 Y8b.     .d8\"\"8b. Y88b 888\n"
					" \"Y8888P\"  888  888  \"Y8888  888  888  \"Y88888\n"
					"                                           888\n"
					"                                      Y8b d88P\n"
					"                                       \"Y88P\" \n"
					"\n"
					"_(:3」∠)_ Shexy is a sexy hexdump written in C _(┐「ε:)_\n"
					"\v"
					"Examples:\n"
					"\tshexy -c who_is_rem.txt // Prints the file text with a colorful output.\n"
					"\tshexy -f bin bokita.log // Prints bokita.log in binary format.\n"
					"\tshexy -s 4 la_torta_es_un_chamullo.pdf // Skips the first 4 bytes.\n"
					"\n"
					"Shexy supports the next formats:\n"
					"\t+--------------+----------------------+\n"
					"\t|    format    |       argument       |\n"
					"\t+--------------+----------------------+\n"
					"\t| binary       | b, bin, binary       |\n"
					"\t+--------------+----------------------+\n"
					"\t| octal        | o, oct, octal        |\n"
					"\t+--------------+----------------------+\n"
					"\t| decima       | d, dec, decimal      |\n"
					"\t+--------------+----------------------+\n"
					"\t| hexadecimal  | h, hex, hexadecimal  |\n"
					"\t+--------------+----------------------+\n"
					"\n"
					"MIT License Copyright (c) 2021 Lunlumo-ex-machina";
const static char args_doc[] = "[FILE]";
static struct argp_option options[] = {
	{"color",  'c', 0,        0, "Display data with color."},
	{"format", 'f', "FORMAT", 0, "Set output format (binary, octal, decimal or hexadecimal)."},
	{"length", 'n', "LENGTH", 0, "Only read N bytes from input."},
	{"columns",'l', "COLS",   0, "Number of columns per line."},
	{"skip",   's', "SKIP",   0, "Skip the first N bytes from input."},
	{0}
}; 


error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = {options, parse_opt, args_doc, doc};

#endif // ARGS_H
