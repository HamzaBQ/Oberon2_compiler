#ifndef SYMBOL_P
#define SYMBOL_P

typedef struct Variable
{
	int type;
	char* name;//gets allocated by the parser
	char* val;//gets allocated by the parser
	int constant;
} Variable;

#endif