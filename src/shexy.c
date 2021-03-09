#include "shexy.h"

const char *argp_program_version = "0.1";
const char *argp_program_bug_address = "nowhere";
static char doc[] = "Shexy is a data-viewer written in C.\
					 \vExamples:\
					 \nshexy -c who_is_rem.txt // Prints the file text with a colorful output.\
					 \nshexy -f bin bokita.log // Prints bokita.log in binary format.\
					 \nshexy -s 4 la_torta_es_un_chamullo.pdf // Skips the first 4 bytes.\
					 \n\
					 \nYou can use the next formats:\
					 \nbinary: b, bin, binary\
					 \noctal: o, oct, octal\
					 \ndecimal: d, dec, decimal\
					 \nhexadecimal: h, hex, hexadecimal";
static char args_doc[] = "[FILE]...";
static struct argp_option options[] = {
	{"color",  'c', 0,        0, "Display data with color."},
	{"format", 'f', "FORMAT", 0, "Set output format (binary, octal, decimal or hexadecimal)."},
	{"length", 'n', "LENGTH", 0, "Only read N bytes from input."},
	{"columns",'l', "COLS",   0, "Number of columns per line."},
	{"skip",   's', "SKIP",   0, "Skip the first N bytes from input."},
	{0}
}; 

argp_t argp = {options, parse_opt, args_doc, doc};

static inline char8_t sanitize(char8_t ch) {
	return isprint(ch)? ch: '.';
}

static inline void print_spaces(int n) {
	printf("%*s", n, "");
}

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

void print_color(arg_formats_t format, char8_t *buf, int buf_len, size_t sz, size_t offset) {
	static color_char_t color;
	int mid = buf_len / 2;
	bool iseven = !(buf_len % 2);
	static unsigned int utf8_state[2] = {0, 0};
	int padding;
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

	for (int i = 0; i < sz; i++, utf8_state[0]--) {
		if (!utf8_state[0]) {
			utf8_state[0] = utf8_len(buf[i]);
			color = colorof(buf[i]);
		}
		if (iseven && i == mid) {
			printf(" ");
		}

		printf("\x1B[38;5;%dm\x1B[48;5;%dm", color.foreground, color.background);
		fun(buf[i]);
		printf("\x1B[m");
		print_spaces(1);
	}
	print_spaces((buf_len - sz) * padding);
	if (iseven && sz < mid) {
		print_spaces(1);
	}
	printf("|");
	for (int i = 0; i < sz; i++, utf8_state[1]--) {
		if (!utf8_state[1]) {
			utf8_state[1] = utf8_len(buf[i]);
			color = colorof(buf[i]);
		}
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
	if (iseven && sz < mid) {
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

	arguments_t arguments;
	arguments.color = false;
	arguments.format = HEX;
	arguments.len = 0;
	arguments.cols = 16;
	arguments.skip = 0;
	arguments.input = "-";

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	int buf_len;
	if (arguments.cols != 16) {
		buf_len = arguments.cols;
	} else {
	 	buf_len = (arguments.format == BIN)? 8: 16;
	}

	FILE *in = fopen(arguments.input, "rb");
	if (!in) {
		in = fmemopen(arguments.input, strlen(arguments.input), "r");
	} 
	if (arguments.skip) {
		fseek(in, arguments.skip, SEEK_SET);
	}

	void (*fun)(arg_formats_t, char8_t *, int, size_t, size_t) = arguments.color? print_color: print_bw;

	char8_t buf[buf_len];
	size_t bytes_read;
	size_t offset = 0;
	int bytes_to_read = (!arguments.len || arguments.len >= buf_len)? buf_len: arguments.len;
	int chunks = arguments.len / buf_len;
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
