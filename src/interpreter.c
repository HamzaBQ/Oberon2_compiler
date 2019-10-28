#include "AST.h"


void interpret_decl(AST_Decl* decl)
{
	if(decl->type==CONST_DECL)
	{
		interpret_exp(decl->const_decl.exp);
		set_var_value(decl->const_decl.const_name,decl->const_decl.exp.value);
	}
}


//@MEM:wasting memory using malloc everytime!
void interpret_exp(AST_Exp*)
{
	if(exp->type==TOK_INT_CONST || exp->type==TOK_REAL_CONST)
	{
		exp->value=exp->literal;
	}
	else if(exp->type==VAR_EXP)
	{
		exp->value=get_variable(exp->var_name)->val;
	}
	else if(exp->type==OP_EXP)
	{
		interpret_exp(exp->op.left);
		interpret_exp(exp->op.right);
		//+
		if(exp->op.type=='+')
		{
			if(exp->valtype==TOK_INTEGER_TYPE)
			{
				int a=atoi(exp->op.left->value);
				int b=atoi(exp->op.right->value);

				char* final=(char*)malloc(sizeof(20));

				sprintf(final,"%d",a+b);

				exp->value=final;
			}
			if(exp->valtype==TOK_REAL_TYPE)
			{
				double a=atof(exp->op.left->value);
				double b=atof(exp->op.right->value);

				char* final=(char*)malloc(sizeof(20));

				sprintf(final,"%f",a+b);

				exp->value=final;
			}
		}
		//-

		if(exp->op.type=='-')
		{
			if(exp->valtype==TOK_INTEGER_TYPE)
			{
				int a=atoi(exp->op.left->value);
				int b=atoi(exp->op.right->value);

				char* final=(char*)malloc(sizeof(20));

				sprintf(final,"%d",a-b);

				exp->value=final;
			}
			if(exp->valtype==TOK_REAL_TYPE)
			{
				double a=atof(exp->op.left->value);
				double b=atof(exp->op.right->value);

				char* final=(char*)malloc(sizeof(20));

				sprintf(final,"%f",a-b);

				exp->value=final;
			}
		}

		//*
		if(exp->op.type=='*')
		{
			if(exp->valtype==TOK_INTEGER_TYPE)
			{
				int a=atoi(exp->op.left->value);
				int b=atoi(exp->op.right->value);

				char* final=(char*)malloc(sizeof(20));

				sprintf(final,"%d",a*b);

				exp->value=final;
			}
			if(exp->valtype==TOK_REAL_TYPE)
			{
				double a=atof(exp->op.left->value);
				double b=atof(exp->op.right->value);

				char* final=(char*)malloc(sizeof(20));

				sprintf(final,"%f",a*b);

				exp->value=final;
			}
		}

		///
		if(exp->op.type=='/')
		{
			if(exp->valtype==TOK_INTEGER_TYPE)
			{
				int a=atoi(exp->op.left->value);
				int b=atoi(exp->op.right->value);

				char* final=(char*)malloc(sizeof(20));

				sprintf(final,"%f",a/b);

				exp->value=final;
			}
			if(exp->valtype==TOK_REAL_TYPE)
			{
				double a=atof(exp->op.left->value);
				double b=atof(exp->op.right->value);

				char* final=(char*)malloc(sizeof(20));

				sprintf(final,"%f",a/b);

				exp->value=final;
			}
		}
	}
}