#ifndef ARGS_H
#define ARGS_H

#include <argp.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct argp argp_t;

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

error_t parse_opt(int key, char *arg, struct argp_state *state);

#endif // ARGS_H
