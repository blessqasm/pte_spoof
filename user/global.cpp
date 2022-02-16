#include "./global.h"


void log(const char* format, ...) {
	char msg[1024] = "";
	va_list vaArg;
	va_start(vaArg, format);
	const int n = _vsnprintf(msg, sizeof(msg) / sizeof(char), format, vaArg);
	va_end(vaArg);
	msg[n] = '\n';

	std::cout << msg;
}