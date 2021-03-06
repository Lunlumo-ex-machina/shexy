#include "shexy.h"

const char *argp_program_version = "0.1";
const char *argp_program_bug_address = "https://github.com/Lunlumo-ex-machina/shexy/issues";

/* Reemplace all non-printable character with a dot */
static inline char8_t sanitize(char8_t ch) {
	return isprint(ch)? ch: '.';
}

/* Print n white spaces */
static inline void print_spaces(int n) {
	printf("%*s", n, "");
}

/* print_bin, print_oct, print_dec and print_hex are user to print the data
 * in diferent formats*/
static inline void print_bin(char8_t ch) {
	for (int i = 0; i < 8; i++) {
		(ch & 1)? printf("1"): printf("0");
		ch >>= 1;
	}
}

static inline void print_oct(char8_t ch) {
	printf("%03o", ch);
}

static inline void print_dec(char8_t ch) {
	printf("%03d", ch);
}

static inline void print_hex(char8_t ch) {
	printf("%02X", ch);
}

/* Generate a colorful output */
void print_color(arg_formats_t format, char8_t *buf, int buf_len, size_t sz, size_t offset) {
	static color_char_t color; // Structure to store the color
	int mid = buf_len / 2;
	bool iseven = !(buf_len % 2);
	static unsigned int utf8_state[2] = {0, 0}; // Stores the number of bytes of the UTF-8 encodeing. {hex (or other format), ascii}
	int padding;
	void (*fun)(char8_t); // Calls a print function

	switch (format) {
		case BIN:
			fun = print_bin;
			padding = 9;
			break;
		case OCT:
			fun = print_oct;
			padding = 4;
			break;
		case DEC:
			fun = print_dec;
			padding = 4;
			break;
		default:
			fun = print_hex;
			padding = 3;
			break;
	}

	printf("%08zX| ", offset);

	for (int i = 0; i < sz; i++) {
		color = colorof(buf[i]);
		if (iseven && i == mid) {
			print_spaces(1);
		}

		/* Sets the foreground and the background colors */
		printf("\x1B[38;5;%dm\x1B[48;5;%dm", color.foreground, color.background);
		fun(buf[i]);

		/* Return to the default color */
		printf("\x1B[m");
		print_spaces(1);
	}

	print_spaces((buf_len - sz) * padding);
	if (iseven && sz <= mid) {
		print_spaces(1);
	}
	printf("|");
	for (int i = 0; i < sz; i++) {
		color = colorof(buf[i]);
		printf("\x1B[38;5;%dm\x1B[48;5;%dm%c\x1B[m", color.foreground, color.background, sanitize(buf[i]));
	}
	print_spaces(buf_len - sz);
	printf("|\n");
}

void print_bw(arg_formats_t format, char8_t *buf, int buf_len, size_t sz, size_t offset) {
	bool iseven = !(buf_len % 2);
	int padding;
	int mid = buf_len / 2;
	void (*fun)(char8_t);

	switch (format) {
		case BIN:
			fun = print_bin;
			padding = 9;
			break;
		case OCT:
			fun = print_oct;
			padding = 4;
			break;
		case DEC:
			fun = print_dec;
			padding = 4;
			break;
		default:
			fun = print_hex;
			padding = 3;
			break;
	}

	printf("%08zX| ", offset);

	for (int i = 0; i < sz; i++) {
		if (iseven && i == mid) {
			printf(" ");
		}
		fun(buf[i]);
		print_spaces(1);
	}
	if (iseven && sz <= mid) {
		print_spaces(1);
	}
	print_spaces((buf_len - sz) * padding);
	printf("|");
	for (int i = 0; i < sz; i++) {
		printf("%c", sanitize(buf[i]));
	}
	print_spaces((buf_len - sz));
	printf("|\n");
}

int main(int argc, char **argv) {

	/* Default arguments */
	arguments_t arguments;
	arguments.color = false; // Color mode
	arguments.format = HEX; // Format of the output
	arguments.len = 0; // Number of bytes to read
	arguments.cols = 16; // Number of colums to display per line
	arguments.skip = 0; // Number of bytes to skip
	arguments.input = "-"; // The input to read.

	argp_parse(&argp, argc, argv, ARGP_NO_HELP, 0, &arguments);

	if (arguments.cols < 1 || arguments.len < 0 || arguments.skip < 0) {
		return 1;
	}

	int buf_len;
	if (arguments.cols != 16) {
		buf_len = arguments.cols;
	} else {
	 	buf_len = (arguments.format == BIN)? 8: 16;
	}

	/* Checks if the input is a filepath otherwise treats it as a string */
	FILE *in;
	if (!strcmp(arguments.input, "-")) {
		in = stdin;
	} else {
		if (!(in = fopen(arguments.input, "r"))) {
			in = fmemopen(arguments.input, strlen(arguments.input), "r");
		}
	}

	if (arguments.skip) {
		fseek(in, arguments.skip, SEEK_SET);
	}

	void (*fun)(arg_formats_t, char8_t *, int, size_t, size_t) = arguments.color? print_color: print_bw;

	char8_t buf[buf_len];
	size_t bytes_read; // Number of bytes read
	size_t offset = 0; // Number of the actual offset
	int bytes_to_read = (!arguments.len || arguments.len >= buf_len)? buf_len: arguments.len; // Number of bytes to read
	while ((bytes_read = fread(buf, 1, bytes_to_read, in))) {
		fun(arguments.format, buf, buf_len, bytes_read, offset);
		if (bytes_to_read < buf_len) {
			break;
		}
		memset(buf, 0, bytes_read);
		offset += buf_len;
		if (arguments.len && (offset + buf_len) > arguments.len) {
			bytes_to_read = arguments.len - offset;
		}
	}

	fclose(in);
	return 0;
}
