#ifndef UTILS_H
#define	UTILS_H

#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstring>

using namespace std;

class Utils {

	public:
		static bool isValidHexChar(char c);
		static bool isValidBinChar(char c);
		static bool isValidHexString(unsigned char *string);
		static bool isValidBinString(unsigned char *string);
		static unsigned char *removeSpaces(const char *string);

};

#endif /* UTILS_H */
