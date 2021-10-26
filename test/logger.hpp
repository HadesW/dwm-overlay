#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

// #define DISABLE_LOG

#if defined(DISABLE_LOG)
# define log(...)
#else
# define log(format,...) logger::log_print( __FUNCTION__, format, __VA_ARGS__)
#endif

class logger
{
public:
	static int log_print(const char* function, const char* format, ...)
	{
		//
		va_list args;
		va_start(args, format);

		char msg[200] = { 0 };
		vsprintf_s(msg, format, args);

		va_end(args);

		//
		char buffer[256] = { 0 };
		sprintf_s(buffer, "[dwm-a] fn:%s msg:%s\n", function, msg);

		//
		return printf(buffer);
	}

private:

};