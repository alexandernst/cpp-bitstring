#include "Utils.h"
using namespace std;

bool Utils::isValidBinString(const string& str) {
	for(string::size_type i = 0; i < str.size(); ++i) {
		if(str[i] != '0' && str[i] != '1') {
			return false;
		}
	}

	return true;
}

bool Utils::isValidHexString(const string& str) {
	size_t len = str.size();

	if(len % 2 != 0) return false;

	for(string::size_type i = 0; i < len; ++i) {
		if(str[i] < 65 || str[i] > 70) {
			if(!isdigit(str[i])) {
				return false;
			}
		}
	}

	return true;
}

const string Utils::removeSpaces(const string& str) {
	stringstream s;

	for(string::size_type i = 0; i < str.size(); ++i) {
		if(str[i] != ' '){
			s << str[i];
		}
	}

	return s.str();
}

string Utils::randomString(size_t len){
	stringstream ss;
	static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> uni(0, sizeof(alphanum) - 1);

	for (size_t i = 0; i < len; ++i) {
		ss << alphanum[uni(rng)];
	}

	return ss.str();
}

string Utils::trim(const string& str){
	string s = str;

	s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
	s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());

	return s;
}
