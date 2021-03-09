#include "utf8.h"

int utf8_len(unsigned char ch) {
	if (ch < 0x80) {
		return 1;
	} else if (ch < 0xE0) {
		return 2;
	} else if (ch < 0xF0) {
		return 3;
	} else if (ch < 0xF5) {
		return 4;
	} else {
		return 1;
	}
}
