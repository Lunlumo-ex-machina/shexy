#include "args.h"

error_t parse_opt(int key, char *arg, struct argp_state *state) {
	arguments_t *arguments = state->input;

	switch (key) {
		case 'h':
			argp_state_help(state, stdout, ARGP_HELP_LONG | ARGP_HELP_DOC | ARGP_HELP_BUG_ADDR | ARGP_HELP_EXIT_OK);
			break;
		case OPT_USG:
			argp_usage(state);
			break;
		case 'c':
			arguments->color = true;
			break;
		case 'f':
			if (!strcmp("b", arg) || !strcmp("bin", arg) || !strcmp("binary", arg)) {
				arguments->format = BIN;
			} else if (!strcmp("o", arg) || !strcmp("oct", arg) || !strcmp("octal", arg)) {
				arguments->format = OCT;
			} else if (!strcmp("d", arg) || !strcmp("dec", arg) || !strcmp("decimal", arg)) {
				arguments->format = DEC;
			} else if (!strcmp("h", arg) || !strcmp("hex", arg) || !strcmp("hexadecimal", arg)) {
				arguments->format = HEX;
			} else {
				argp_usage(state);
			}
			break;
		case 'l':
			arguments->cols = strtol(arg, NULL, 10);
			break;
		case 'n':
			arguments->len = strtol(arg, NULL, 10);
			break;
		case 's':
			arguments->skip = strtol(arg, NULL, 10);
			break;
		case ARGP_KEY_HELP_HEADER:
			printf("hola");
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num > 0) {
				argp_usage(state);
			}
			arguments->input = arg;
			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

