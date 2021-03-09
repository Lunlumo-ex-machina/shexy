#include "color.h"

color_char_t colorof(unsigned char ch) {
	color_char_t color;
	if (ch < 0x20) {
		color = ch == 0? (color_char_t){BRIGHT_YELLOW, BLACK}: (color_char_t){BRIGHT_GREEN, BLACK};
	} else if (ch >= 0x7F) {
		color = ch == 0xFF? (color_char_t){BRIGHT_RED, BLACK}: (color_char_t){BRIGHT_MAGENTA, BLACK};
	} else {
		color = (color_char_t){BRIGHT_BLUE, BLACK};
	}
	return color;
}
