#include "semantizer.h"
#include "AST.h"
#include "symbol_table.h"
#include "error.h"
#include "parser.h"
#include "symbol.h"

int semantizer(AST_Module* mod)
{
	AST_Decl* decl=mod->decl_seq;
	AST_Stmt* stm=mod->stmt_seq;

	//add variable and constants to the symbol_table
	while(decl)
	{
		if(decl->type==VAR_DECL)
		{
			Variable* var=get_variable(decl->var_decl.var_name);

			if(var)
				add_error("error:line:%d:Vairable %s already declared\n",decl->line,decl->var_decl.var_name);
			else
			{
				Variable* v=add_variable(decl->var_decl.var_name,decl->var_decl.var_type,0,0);
				v->val=(char*)malloc(sizeof(64));
				v->val[0]=0;
			}
		}
		else if(decl->type==CONST_DECL)
		{
			Variable* var=get_variable(decl->const_decl.const_name);

			if(var)
				add_error("error:line:%d:Constant %s already declared\n",decl->line,decl->const_decl.const_name);
			else
			{
				int type=get_exp_type(decl->const_decl.exp);
				add_variable(decl->const_decl.const_name,type,0,1);
			}
		}

		decl=decl->next;
	}


	return semantize_stmt(stm);
}


int semantize_stmt(AST_Stmt* stm)
{

	if(!stm)
		return 1;

	if(stm->type==ASSIGN_STMT)
	{
			Variable* var=get_variable(stm->assign.var_name);

			if(!var)
				add_error("error:line:%d:Undeclared variable %s.\n",stm->line,stm->assign.var_name);
			else
			{
				int t=get_exp_type(stm->assign.exp);

				if(var->type!=t)
					add_error("error:line:%d:variable %s is not of the same type as the expression.\n"
						,stm->line,stm->assign.var_name);

				if(var->constant)
					add_error("error:line:%d:Cannot change constant %s value.\n"
						,stm->line,stm->assign.var_name);
			}
	}
	else if(stm->type==IF_STMT)
	{
		int t=get_exp_type(stm->ifelse.exp);
		if(t!=TOK_BOOL_TYPE)
		{
			add_error("error:line:%d:Expression inside If isn't of type bool.\n"
					,stm->line);
		}

		semantize_stmt(stm->ifelse.ifsmt);
		semantize_stmt(stm->ifelse.elsesmt);

	}
	else if(stm->type==WHILE_STMT)
	{
		int t=get_exp_type(stm->whilee.exp);
		if(t!=TOK_BOOL_TYPE)
		{
			add_error("error:line:%d:Expression inside While isn't of type bool.\n"
					,stm->line);
		}
		semantize_stmt(stm->whilee.smt);

	}
	else if(stm->type==REPEAT_STMT)
	{
		int t=get_exp_type(stm->repeat.exp);
		if(t!=TOK_BOOL_TYPE)
		{
			add_error("error:line:%d:Expression inside While isn't of type bool.\n"
					,stm->line);
		}
		semantize_stmt(stm->repeat.smt);

	}
	else if(stm->type==FOR_STMT)
	{
		semantize_stmt(stm->forr.initstmt);
		get_exp_type(stm->forr.highexp);
		get_exp_type(stm->forr.byexp);
		semantize_stmt(stm->forr.smt);
	}

	semantize_stmt(stm->next);
}

int get_exp_type(AST_Exp* exp)
{
	if(!exp)
		return -1;

	if(exp->type==TOK_INT_CONST)
		return exp->valtype=TOK_INTEGER_TYPE;
	if(exp->type==TOK_REAL_CONST)
		return exp->valtype=TOK_REAL_TYPE;
	if(exp->type==TOK_TRUE)
		return exp->valtype=TOK_BOOL_TYPE;
	if(exp->type==TOK_FALSE)
		return exp->valtype=TOK_BOOL_TYPE;

	if(exp->type==VAR_EXP)
	{
		Variable* var=get_variable(exp->var_name);
			if(!var)
			{
				add_error("error:line:%d:Undeclared variable %s\n",exp->line,exp->var_name);
				return 0;
			}
			else
				return exp->valtype=var->type;
	}

	if(exp->type==OP_EXP)
	{
		int t1=get_exp_type(exp->op.left);
		int t2=get_exp_type(exp->op.right);

		if(exp->op.type=='+' || exp->op.type=='-' || exp->op.type=='/' || exp->op.type=='*')
		{
			if(t1!=t2)
			{
				add_error("error:line:%d:Incompatible type in expression\n",exp->line);
				return exp->valtype=t1;
			}

			return exp->valtype=t1;
		}
		else if(exp->op.type=='&' || exp->op.type==TOK_OR)
		{
			if(t1!=TOK_BOOL_TYPE || t2!=TOK_BOOL_TYPE)
			{
				add_error("error:line:%d:Incompatible type in expression (not bool)\n",exp->line);
				return exp->valtype=TOK_BOOL_TYPE;
			}

			return exp->valtype=TOK_BOOL_TYPE;
		}
		else if(exp->op.type=='<' || exp->op.type==TOK_GREATER_EQ || exp->op.type==TOK_SMALLER_EQ || exp->op.type=='>')
		{
			if(t1==TOK_BOOL_TYPE || t2==TOK_BOOL_TYPE)
			{
				add_error("error:line:%d:Incompatible type in expression (cannot campare bool types)\n",exp->line);
				return exp->valtype=TOK_BOOL_TYPE;
			}

			return exp->valtype=TOK_BOOL_TYPE;
		}
		else if(exp->op.type=='=' || exp->op.type=='#')
		{
			if(t1!=t2)
			{
				add_error("error:line:%d:Incompatible type in expression\n",exp->line);
				return exp->valtype=TOK_BOOL_TYPE;
			}

			return exp->valtype=TOK_BOOL_TYPE;
		}

		return exp->valtype=t1;
	}
}