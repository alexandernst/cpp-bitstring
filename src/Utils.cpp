#include "Utils.h"
using namespace std;

bool Utils::isValidBinString(unsigned char *string) {
	bool valid = true;
	size_t len = strlen((const char *) string);

	for(size_t i = 0; i < len; i++) {
		if(string[i] != '0' && string[i] != '1') {
			valid = false;
			break;
		}
	}

	return valid;
}

bool Utils::isValidHexString(unsigned char *string) {
	bool valid = true;
	size_t len = strlen((const char *) string);

	if(len % 2 != 0) return false;

	for(size_t i = 0; i < len; i++) {
		if( (string[i] < 65 && string[i] > 70) && !isdigit(string[i]) ) {
			valid = false;
			break;
		}
	}

	return valid;
}

unsigned char *Utils::removeSpaces(const char *string) {
	size_t len = strlen(string);

	unsigned char *s = (unsigned char *) calloc(1, len + 1);
	unsigned char *write = s;
	const char *read = string;

	do {
		if (*read != ' ')
			*write++ = *read;
	} while (*read++);

	len = strlen((const char *) s);
	unsigned char *s2 = (unsigned char *) calloc(1, len + 1);
	memcpy(s2, s, len);
	free(s);

	return s2;
}
