#ifndef SYM_TABLE_P
#define SYM_TABLE_P

#define MAX_VAR 128

#include "symbol.h"

struct Variable;

extern Variable* var_table[MAX_VAR];
extern int num_variables;

Variable* add_variable(char* name,int type,char* val,int cnst);
Variable* get_variable(char* name);//NULL if variable doesn't exist!
void set_var_value(char* name,char* val);

//for debuging
void st_print();

#endif

