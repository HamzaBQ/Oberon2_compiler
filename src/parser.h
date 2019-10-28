#ifndef PARSER_P
#define PARSER_P

#include <stdio.h>
#include "AST.h"

enum {TOK_IF=-100,/*-99*/TOK_IMPORT,/*-98*/TOK_IN,/*-97*/TOK_IS,/*-96*/TOK_LOOP,/*-95*/TOK_MOD,/*-94*/TOK_NIL,/*-93*/TOK_OR,
	/*-92*/TOK_POINTER,/*-91*/TOK_PROCEDURE,/*-90*/TOK_RECORD,/*-89*/TOK_REPEAT,/*-88*/TOK_RETURN,/*-87*/TOK_THEN,/*-86*/TOK_MODULE,
	/*-85*/TOK_BEGIN,/*-84*/TOK_ARRAY,/*-83*/TOK_CASE,/*-82*/TOK_CONST,/*-81*/TOK_DIV,/*-80*/TOK_DO,/*-79*/TOK_ELSE,/*-78*/TOK_ELSIF,
	/*-77*/TOK_END,/*-76*/TOK_BY,/*-75*/TOK_FOR,/*-74*/TOK_EXIT,/*-73*/TOK_WITH,/*-72*/TOK_WHILE,/*-71*/TOK_VAR,/*-70*/TOK_TYPE,
	/*-69*/TOK_UNTIL,/*-68*/TOK_TO,/*-67*/TOK_CHAR_TYPE,/*-66*/TOK_INTEGER_TYPE,/*-65*/TOK_REAL_TYPE,/*-64*/TOK_BOOL_TYPE,
	/*-63*/TOK_SHORTINT_TYPE,/*-62*/TOK_LONGINT_TYPE,/*-61*/TOK_LONGREAL_TYPE,/*-60*/TOK_SET_TYPE,/*-59*/TOK_ASSIGN,
	/*-58*/TOK_GREATER_EQ,/*-57*/TOK_SMALLER_EQ,/*-56*/TOK_IDENT,/*-55*/TOK_INT_CONST,/*-54*/TOK_HEX_CONST,/*-53*/TOK_REAL_CONST,
	/*-52*/TOK_CHAR_CONST,/*-51*/TOK_STRING_CONST,/*-50*/TOK_OF,TOK_TRUE,TOK_FALSE
};

struct FILE;
struct AST_Module;
struct AST_Decl;
struct AST_Stmt;
struct AST_EXP;

extern int cur_line;
extern int cur_column;
extern int token;
extern int read_next;
extern char* yytext;
extern FILE* yyin;
		
int yylex();

int next_token();
void take_token_back();
void error(const char* err);


//grammar
int module(AST_Module**);
int import_list();

int ident_list();
int const_decl(AST_Decl**);
int var_decl(AST_Decl**);
int type_decl();
int forward_decl();
int proc_decl();

int const_decl_aux(AST_Decl**);
int type_decl_aux();
int var_decl_aux(AST_Decl**);


int formal_params();
int fp_section();

int type();

int decl_seq(AST_Decl**);
int decl_seq_aux(AST_Decl**);
int proc_forward_decl_aux();
int stmt_seq(AST_Stmt**);

//used to test if token t is a First of the production rule!
int is_mulop(int t);
int is_addop(int t);
int is_firstof_expr(int t);


int stmt(AST_Stmt**);
int expr_list();
int expr(AST_Exp**);
int const_expr(AST_Exp**);
int simple_expr(AST_Exp**);
int relation();
int term(AST_Exp**);
int mulop();
int addop();
int factor(AST_Exp**);

//kamal

int designator();
int qualident();
int ident_def();
int case_expr();
int case_labels();
int field_list();
int receiver();
int guard();
int element();


#endif