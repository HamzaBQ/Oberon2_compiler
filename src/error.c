#include "error.h"
#include <stdarg.h>

Error error_table[MAX_ERROR];
int num_errors=0;

void add_error(char* fmt,...)
{
	va_list args;
  	char buffer[512];
 
  	va_start(args, fmt);
  	vsnprintf(buffer, sizeof(buffer), fmt, args);
  	va_end(args);

  	strcpy(error_table[num_errors].msg,buffer);
  	num_errors++;
}

void print_errors()
{
	int i;
	for(i=0;i<num_errors;i++)
	{
		printf("%s",error_table[i].msg);
	}
}