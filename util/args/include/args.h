#ifndef ARGS_H
#define ARGS_H

#define OPT_USG 1

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
					"\n"
					"Usage:\n"
					"  shexy [OPTIONS...] [FILE]\n"
					"\n"
					"Options:\n"
					"\v"
					"Examples:\n"
					"  shexy -c who_is_rem.txt // Prints the file text with a colorful output.\n"
					"  shexy -f bin bokita.log // Prints bokita.log in binary format.\n"
					"  shexy -s 4 la_torta_es_un_chamullo.pdf // Skips the first 4 bytes.\n"
					"\n"
					"Shexy supports the next formats:\n"
					"  +--------------+----------------------+\n"
					"  |    format    |       argument       |\n"
					"  +--------------+----------------------+\n"
					"  | binary       | b, bin, binary       |\n"
					"  +--------------+----------------------+\n"
					"  | octal        | o, oct, octal        |\n"
					"  +--------------+----------------------+\n"
					"  | decima       | d, dec, decimal      |\n"
					"  +--------------+----------------------+\n"
					"  | hexadecimal  | h, hex, hexadecimal  |\n"
					"  +--------------+----------------------+\n"
					"\n"
					"MIT License Copyright (c) 2021 Lunlumo";
const static char arg_doc[] = "[FILE]";
static struct argp_option options[] = {
	{"help",       'h',    0,        0, "Display the help page.",-1},
	{"usage",  OPT_USG,    0,        0, "Display usage.", -1},
	{"version",    'v',    0,        0, "Display version.", -1},
	{"color",      'c',    0,        0, "Display data with color."},
	{"format",     'f',    "FORMAT", 0, "Set the output format (binary, octal, decimal or hexadecimal)."},
	{"length",     'n',    "LENGTH", 0, "Only read N bytes from input."},
	{"columns",    'l',    "COLS",   0, "Number of columns per line."},
	{"skip",       's',    "SKIP",   0, "Skip the first N bytes from input."},
	{0}
}; 

error_t parse_opt(int key, char *arg, struct argp_state *state);

static struct argp argp = {options, parse_opt, arg_doc, doc};

#endif // ARGS_H
