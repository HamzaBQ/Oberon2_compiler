#ifndef INTERPRETER_P
#define INTERPRETER_P

struct AST_Decl;
struct AST_Module;
struct AST_Exp;
struct AST_Stmt;


void interpret_decl(AST_Decl*);
void interpret_module(AST_Module*);
void interpret_exp(AST_Exp*);
void interpret_stmt(AST_Stmt*);


#endif