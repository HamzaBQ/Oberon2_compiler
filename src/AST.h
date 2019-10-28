#ifndef AST_P
#define AST_P

enum {INT_LIT_EXP,DOUBLE_LIT_EXP,OP_EXP,VAR_EXP};
enum {OP_PLUS,OP_MINUS,OP_MUL,OP_DIV};//relation.addop.mulop
enum {ASSIGN_STMT,IF_STMT,WHILE_STMT,FOR_STMT,REPEAT_STMT};
enum {VAR_DECL,CONST_DECL,TYPE_DECL,PROC_DECL};

typedef struct AST_Exp
{
	int type;
	int line;
	int term_sign;
	char* value;//used for ast interpreter to store the value of the expression
	int valtype;
	union{
		char* literal;
		struct{int type; struct AST_Exp* left; struct AST_Exp* right;} op;
		char* var_name;
	};
} AST_Exp;

typedef struct AST_Stmt
{
	int type;
	int line;
	struct AST_Stmt* next;
	union{
		struct{char* var_name;AST_Exp* exp;}assign;
		struct{AST_Exp* exp;struct AST_Stmt* ifsmt;struct AST_Stmt* elsesmt} ifelse;
		struct{AST_Exp* exp;struct AST_Stmt* smt;} whilee;
		struct{AST_Exp* exp;struct AST_Stmt* smt;} repeat;
		struct{struct AST_Stmt* initstmt;AST_Exp* highexp;AST_Exp* byexp;struct AST_Stmt* smt;} forr;
	};
} AST_Stmt;

typedef struct AST_Decl
{
	int type;
	int line;
	struct AST_Decl* next;
	union{
		struct {char* var_name;int var_type} var_decl;
		struct {char* const_name;AST_Exp* exp} const_decl;
	}
} AST_Decl;

typedef struct AST_Module
{
	char* name;
	AST_Decl* decl_seq;
	AST_Stmt* stmt_seq;
} AST_Module;

AST_Exp* new_var_exp(char* name);
AST_Exp* new_literal_exp(int type,char* val);
AST_Exp* new_op_exp(int type,AST_Exp* left,AST_Exp* right);

AST_Stmt* new_assign(char* var,AST_Exp* exp);
AST_Stmt* new_if_stmt(AST_Exp* exp,AST_Stmt* ifsmt,AST_Stmt* elsesmt);
AST_Stmt* new_while_stmt(AST_Exp* exp,AST_Stmt* smt);
AST_Stmt* new_for_stmt(AST_Stmt* initstmt,AST_Exp* highexp,AST_Exp* byexp,AST_Stmt* smt);


AST_Decl* new_var_decl(char* name,int var_type);
AST_Decl* new_const_decl(char* name,AST_Exp* exp);

AST_Module* new_module(char* name,AST_Decl* decl,AST_Stmt* stmt);
//debuging

void print_module(AST_Module* mod);
void print_decl(AST_Decl* decl);
void print_stmt(AST_Stmt* smt);
void print_exp(AST_Exp* exp);

#endif