#include "AST.h"
#include <stdlib.h>
#include "utils.h"

AST_Exp* new_var_exp(char* name)
{
	AST_Exp* exp=(AST_Exp*) malloc(sizeof(AST_Exp));
	exp->type=VAR_EXP;
	exp->var_name=name;
	exp->term_sign=1;

	return exp;
}

AST_Exp* new_literal_exp(int type,char* val)
{
	AST_Exp* exp=(AST_Exp*) malloc(sizeof(AST_Exp));
	exp->type=type;
	exp->literal=val;
	exp->term_sign=1;

	return exp;
}


AST_Exp* new_op_exp(int type,AST_Exp* left,AST_Exp* right)
{
	AST_Exp* exp=(AST_Exp*) malloc(sizeof(AST_Exp));
	exp->type=OP_EXP;
	exp->op.type=type;
	exp->op.left=left;
	exp->op.right=right;
	exp->term_sign=1;

	return exp;
}

AST_Stmt* new_assign(char* var,AST_Exp* exp)
{
	AST_Stmt* stmt=(AST_Stmt*) malloc(sizeof(AST_Stmt));

	stmt->type=ASSIGN_STMT;
	stmt->next=0;

	stmt->assign.var_name=var;
	stmt->assign.exp=exp;

	return stmt;
}

AST_Stmt* new_if_stmt(AST_Exp* exp,AST_Stmt* ifsmt,AST_Stmt* elsesmt)
{
	AST_Stmt* stmt=(AST_Stmt*) malloc(sizeof(AST_Stmt));
	stmt->type=IF_STMT;
	stmt->next=0;

	stmt->ifelse.exp=exp;
	stmt->ifelse.ifsmt=ifsmt;
	stmt->ifelse.elsesmt=elsesmt;

	return stmt;
}

AST_Stmt* new_while_stmt(AST_Exp* exp,AST_Stmt* smt)
{
	AST_Stmt* stmt=(AST_Stmt*) malloc(sizeof(AST_Stmt));
	stmt->type=WHILE_STMT;
	stmt->next=0;

	stmt->whilee.exp=exp;
	stmt->whilee.smt=smt;

	return stmt;
}

AST_Stmt* new_repeat_stmt(AST_Exp* exp,AST_Stmt* smt)
{
	AST_Stmt* stmt=(AST_Stmt*) malloc(sizeof(AST_Stmt));
	stmt->type=REPEAT_STMT;
	stmt->next=0;

	stmt->repeat.exp=exp;
	stmt->repeat.smt=smt;

	return stmt;
}

AST_Stmt* new_for_stmt(AST_Stmt* initstmt,AST_Exp* highexp,AST_Exp* byexp,AST_Stmt* smt)
{
	AST_Stmt* stmt=(AST_Stmt*) malloc(sizeof(AST_Stmt));
	stmt->type=FOR_STMT;
	stmt->next=0;

	stmt->forr.initstmt=initstmt;
	stmt->forr.highexp=highexp;
	stmt->forr.byexp=byexp;
	stmt->forr.smt=smt;

	return stmt;
}

AST_Decl* new_var_decl(char* name,int var_type)
{
	AST_Decl* decl=(AST_Decl*) malloc(sizeof(AST_Decl));

	decl->type=VAR_DECL;
	decl->next=0;

	decl->var_decl.var_name=name;
	decl->var_decl.var_type=var_type;

	return decl;
}
AST_Decl* new_const_decl(char* name,AST_Exp* exp)
{
	AST_Decl* decl=(AST_Decl*) malloc(sizeof(AST_Decl));

	decl->type=CONST_DECL;
	decl->next=0;

	decl->const_decl.const_name=name;
	decl->const_decl.exp=exp;

	return decl;
}

AST_Module* new_module(char* name,AST_Decl* decl,AST_Stmt* stmt)
{
	AST_Module* mod=(AST_Module*) malloc(sizeof(AST_Module));

	mod->name=name;
	mod->decl_seq=decl;
	mod->stmt_seq=stmt;

	return mod;
}














void print_module(AST_Module* mod)
{
	printf("+MODULE : %s\n",mod->name);
	print_decl(mod->decl_seq);
	print_stmt(mod->stmt_seq);
}

void print_decl(AST_Decl* decl)
{
	if(decl==0)
	{
		printf("-DECL NULL\n");
		return;
	}
	db_tree_lvl+=5;
	db_tab();
	printf("-DECL :\n");
	db_tree_lvl+=2;
	db_tab();
	

	if(decl->type==VAR_DECL)
		printf("-type: %d Variable -- %s\n",decl->var_decl.var_type,decl->var_decl.var_name);
	else if(decl->type==CONST_DECL)
	{
		printf("-type: Constant -- %s\n",decl->const_decl.const_name);
		print_exp(decl->const_decl.exp);
	}
	db_tree_lvl-=7;
	
		print_decl(decl->next);
}

void print_stmt(AST_Stmt* smt)
{
	if(!smt)
		return;

	db_tree_lvl+=5;
	db_tab();
	printf("-STMT :\n");
	db_tree_lvl+=2;
	db_tab();

	if(smt->type==ASSIGN_STMT)
	{
		printf("-lvalue: %s\n",smt->assign.var_name);
		db_tab();
		printf("-rvalue: \n");

		print_exp(smt->assign.exp);
	}
	else if(smt->type==IF_STMT)
	{
		printf("-IF_EXP:\n");
		db_tab();
		print_exp(smt->ifelse.exp);
		db_tab();
		printf("-DO\n");
		db_tab();
		print_stmt(smt->ifelse.ifsmt);

		if(smt->ifelse.elsesmt)
		{
			db_tab();
			printf("-ELSE_DO\n");
			db_tab();
			print_stmt(smt->ifelse.elsesmt);
		}
	}

	db_tree_lvl-=7;
	if(smt->next)
		print_stmt(smt->next);
}
void print_exp(AST_Exp* exp)
{
	db_tree_lvl+=5;
	db_tab();
	printf("-EXP :\n");
	db_tree_lvl+=2;
	db_tab();

	if(exp->type==VAR_EXP)
	{
		printf("-Variable: %s\n",exp->var_name);
	}
	else if(exp->type==OP_EXP)
	{
		printf("-op: %c\n",exp->op.type);
		db_tab();
		printf("-left & right :\n");
		print_exp(exp->op.left);
		print_exp(exp->op.right);
	}
	else
		printf("-literal: %s\n",exp->literal);

	db_tree_lvl-=7;
}