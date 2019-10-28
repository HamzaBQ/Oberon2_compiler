#ifndef SEMANTIZER_P
#define SEMANTIZER_P

#include "AST.h"

struct AST_Module;
struct AST_Exp;

int semantizer(AST_Module*);
int semantize_stmt(AST_Stmt* smt);
int get_exp_type(AST_Exp*);

#endif SEMANTIZER_P

///assignement just on vars
//add bool type support