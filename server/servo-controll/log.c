#include "log.h"
#include "defines.h"

#include <stdio.h>
#include <stdarg.h>

#ifdef GPIO_DEBUG
void doLog(const char * format, ...){
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}
#else
void doLog(const char * format, ...){

}
#endif
