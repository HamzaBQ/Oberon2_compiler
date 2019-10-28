#include "parser.h"
#include <stdio.h>
#include <stdarg.h>

int token;
int read_next=1;//set to 0 equivalent to take_token_back()
int cur_line=1;
int cur_column=1;
int  iii=0;

char* get_symbol_name()
{
	char *str=(char*)malloc(sizeof(strlen(yytext))+1);
	strcpy(str,yytext);

	return str;
}

int next_token()
{
	if(read_next)
	{
		token=yylex();
	}
	else
	{
		read_next=1;
	}

	//printf("%d %s \n",token,yytext);

	return token;
}

void error(const char* err)
{
	fprintf(stderr,"line:%d column:%d syntax error: %s\n",cur_line,cur_column,err);

	//if debug_mode
	/*if(token>0)
		fprintf(stderr,"last token read : '%c'\n",token);
	else
		fprintf(stderr,"last token read : '%d'\n",token);*/


}

int module(AST_Module** module)
{
	char* module_name;
	AST_Decl* decl=0;
	AST_Stmt* stmt=0;

	if(next_token()!=TOK_MODULE)
	{
		error("expected 'MODULE'");
		return 0;
	}
	if(next_token()!=TOK_IDENT)
	{
		error("module name missing");
		return 0;
	}

	//get module name;
	module_name=get_symbol_name();

	if(next_token()!=';')
	{
		error("expected ';'");
		//@MEM: release module_name memory
		return 0;
	}
	
	/*if(next_token()==TOK_IMPORT)
	{
		if(!import_list())
			return 0;
	}
	else
		read_next=0;*/

	if(!decl_seq(&decl))
		return 0;


	

	if(next_token()==TOK_BEGIN)
	{
		if(!stmt_seq(&stmt))
			return 0;
	}
	else
		read_next=0;

	if(next_token()!=TOK_END)
	{
		error("expected 'END'");
		return 0;
	}
	if(next_token()!=TOK_IDENT)
	{
		error("module name missing");
		return 0;
	}
	if(next_token()!='.')
	{
		error("expected '.'");
		return 0;
	}

	*module=new_module(module_name,decl,stmt);

	return 1;
}
/*
int import_list()
{
	if(next_token()!=TOK_IDENT)
	{
		error("imported module name missing");
		return 0;
	}

	if(next_token()==',')
	{
		int res= ident_list();

		if(next_token()!=';')
		{
			error("expected ';'");
			return 0;
		}
		return res;
	}

	if(token==';')
		return 1;

	error("expected ';'");

	return 0;
}*/
/*
int ident_list()
{
	if(next_token()!=TOK_IDENT)
	{
		error("expected an identifier");
		return 0;
	}
	if(next_token()==',')
		return ident_list();
	
	read_next=0;
	return 1;
}*/

int decl_seq(AST_Decl** decl)
{	
	next_token();
	if(token==TOK_CONST || token==TOK_VAR || token==TOK_TYPE)
	{
		read_next=0;
		if(!decl_seq_aux(decl))
		{
			return 0;
		}
	}
	else
		read_next=0;

	//this is why we are using decl_seq_aux!!-->
	/*if(next_token()==TOK_PROCEDURE)
	{
		read_next=0;
		return proc_forward_decl_aux();
	}
	else
		read_next=0;*/

	return 1;
}


int decl_seq_aux(AST_Decl** decl)
{
	AST_Decl *temp=0;
	if(next_token()==TOK_CONST)
	{
		if(!const_decl_aux(&temp))
			return 0;
		if(!temp)
			return 1;

		*decl=temp;
		//get to the end of the list;
		while(temp->next)temp=temp->next;
		decl_seq_aux(&(temp->next));


		return 1;
	}
	else if(token==TOK_VAR)
	{
		if(!var_decl_aux(&temp))
			return 0;
		if(!temp)
			return 1;

		*decl=temp;

		//get to the end of the list;
		while(temp->next)temp=temp->next;
		decl_seq_aux(&(temp->next));

		return 1;
	}
	/*else if(token==TOK_TYPE)
	{
		int res=type_decl_aux();

		return res && decl_seq_aux();
	}*/
	else
	{
		read_next=0;
		return 1;
	}
}

int const_decl(AST_Decl** decl)
{
	char* const_name;
	AST_Exp* exp=0;
	if(next_token()!=TOK_IDENT)
	{
		error("expected an identifier");
		return 0;
	}

	//@TODO:add constant to symbol_table;
	//get_constant name
	const_name=get_symbol_name();

	if(next_token()!='=')
	{
		error("expected '='");
		return 0;
	}
	if(!expr(&exp))
		return 0;

	*decl=new_const_decl(const_name,exp);
	(*decl)->line=cur_line;

	return 1;
}

int var_decl(AST_Decl** decl)
{
	char* var_name;
	int vartype;
	//int res=ident_list();
	if(next_token()!=TOK_IDENT)
	{
		error("expected an identifier");
		return 0;
	}
	var_name=get_symbol_name();
	
	if(next_token()!=':')
	{
		error("expected ':'");
		return 0;
	}

	if(!(vartype=type()))
		return 0;

	*decl=new_var_decl(var_name,vartype);

	(*decl)->line=cur_line;

	//@TODO: add variable to symbol table
	return 1 ;
}
/*
int type_decl()
{
	if(next_token()!=TOK_IDENT)
	{
		error("expected an identifier");
		return 0;
	}
	if(next_token()!='=')
	{
		error("expected '='");
		return 0;
	}

	return type();
}

int forward_decl()
{
	if(next_token()!=TOK_IDENT)
	{
		error("expected an identifier");
		return 0;
	}

	return formal_params();
}*/

int const_decl_aux(AST_Decl** decl)
{
	if(next_token()!=TOK_IDENT)
	{
		read_next=0;
		return 1;
	}
	else
		read_next=0;

	if(!const_decl(decl))
		return 0;

	if(next_token()!=';')
	{
			error("expected ';'");
			return 0;
	}

	return const_decl_aux(&((*decl)->next));
}

int var_decl_aux(AST_Decl** decl)
{
	if(next_token()!=TOK_IDENT)
	{
		read_next=0;
		return 1;
	}
	else
		read_next=0;

	if(!var_decl(decl))
		return 0;

	if(next_token()!=';')
	{
			error("expected ';'");
			return 0;
	}

	return var_decl_aux(&((*decl)->next));
}
/*
int type_decl_aux()
{
	if(next_token()!=TOK_IDENT)
	{
		read_next=0;
		return 1;
	}
	else
		read_next=0;

	int res=type_decl();
	if(next_token()!=';')
	{
			error("expected ';'");
			return 0;
	}
	return res &&  type_decl_aux();
}*/
/*
int proc_decl()
{
	if(next_token()!=TOK_IDENT)
	{
		error("expected an identifier");
		return 0;
	}

	if(next_token()=='(')
	{
		read_next=0;
		if(!formal_params())
			return 0;
	}
	else
		read_next=0;

	if(next_token()!=';')
	{
		error("expected ';'");
		return 0;
	}	

	if(!decl_seq())
		return 0;

	if(next_token()==TOK_BEGIN)
	{
		if(!stmt_seq())
			return 0;
	}
	else
		read_next=0;

	if(next_token()!=TOK_END)
	{
		error("expected 'END'");
		return 0;
	}
	if(next_token()!=TOK_IDENT)
	{
		error("module procedure missing");
		return 0;
	}

	if(next_token()==';')
	{
		return 1;
	}
	else
		read_next=0;

	return 1;
}

int formal_params()
{
	if(next_token()!='(')
	{
		error("expected '('");
			return 0;
	}

	if(next_token()!=')')
	{
		read_next=0;
		if(!fp_section_aux())
			return 0;
	}
	else
		read_next=0;

	if(next_token()!=')')
	{
		error("expected ')'");
			return 0;
	}

	if(next_token()==':')
	{
		if(next_token()!=TOK_TYPE)
		{
			error("expected a type");
			return 0;
		}
	}
	else
		read_next=0;

	return 1;
}

int fp_section_aux()
{
	if(!fp_section())
		return 0;

	if(next_token()==';')
		return fp_section_aux();

	read_next=0;
	return 1;
}

int fp_section()
{
	if(next_token()!=TOK_VAR)
		read_next=0;

	if(!ident_list())
		return 0;

	if(next_token()!=':')
	{
		error("expected ':'");
		return 0;
	}

	return type();
}
*/
int type()
{
	// no support for records yet!
	/*if(next_token()==TOK_ARRAY)
	{
		next_token();
		if(token!=TOK_INT_CONST && token!=TOK_HEX_CONST)
		{
			error("expected an integer constant");
			return 0;
		}
		if(next_token()!=TOK_OF)
		{
			error("expected 'OF'");
			return 0;
		}

		return type();
	}
	/*
	else if(token==TOK_RECORD)
	{

	}
	else if(token==TOK_IDENT)
	{

	}
	else if(token==TOK_POINTER)
	{
		if(next_token()!=TOK_TO)
		{
			error("expected 'TO'");
			return 0;
		}

		return type();
	}
	else if(token==TOK_PROCEDURE)
	{
		return formal_params();
	}
	else if(token==TOK_CHAR_TYPE || token==TOK_INTEGER_TYPE || token==TOK_REAL_TYPE || token==TOK_BOOL_TYPE
		 || token==TOK_SHORTINT_TYPE || token==TOK_LONGINT_TYPE || token==TOK_LONGREAL_TYPE)
	{
		return 1;
	}
	else
	{
		error("expected a type");
		return 0;
	}*/

	if(next_token()==TOK_INTEGER_TYPE || token==TOK_REAL_TYPE || token==TOK_BOOL_TYPE)
	{
		return token;
	}
	else
	{
		error("expected a type");
		return 0;
	}
}
/*
int proc_forward_decl_aux()
{
	if(next_token()!=TOK_PROCEDURE)
	{
		read_next=0;
		return 1;
	}

	if(next_token()=='^')
	{
		int res=forward_decl();
		if(next_token()!=';')
		{
			error("expected ';'");
			return 0;
		}
		return res;
	}
	else
		read_next=0;

	return (proc_decl() && proc_forward_decl_aux()); 
}
*/
int stmt_seq(AST_Stmt** smt)
{
	if(!stmt(smt))
		return 0;

	if(next_token()==';')
		return stmt_seq(&((*smt)->next));
	else
		read_next=0;

	return 1;
}

int stmt(AST_Stmt** smt)
{
	next_token();
	if(token==TOK_IDENT)
	{
		char* var_name=get_symbol_name();
		next_token();
		if(token==TOK_ASSIGN)//assignement
		{
			AST_Exp* exp=0;
			if(!expr(&exp))
				return 0;

			*smt=new_assign(var_name,exp);
			(*smt)->line=cur_line;
			return 1;
		}
		/*else if(token=='(')//function call
		{
			if(!expr_list())
				return 0;

			if(next_token()!=')')
			{
				error("expected ')'");
				return 0;
			}

			return 1;
		}*/
		else 
			read_next=0;
		
		return 1;
	}
	else if(token==TOK_IF)
	{
		AST_Exp* exp=0;
		AST_Stmt* ifsmt=0;
		AST_Stmt* elsesmt=0;

		if(!expr(&exp))
		{
			return 0;
		}

		if(next_token()!=TOK_THEN)
		{
			error("expected 'THEN'");
			return 0;
		}

		if(!stmt_seq(&ifsmt))
			return 0;

		/*if(next_token()==TOK_ELSIF)
		{
			read_next=0;
			if(!elsif_aux())
				return 0;
		}
		else
			read_next=0;*/

		if(next_token()==TOK_ELSE)
		{
			if(!stmt_seq(&elsesmt))
				return 0;
		}
		else
			read_next=0;

		if(next_token()!=TOK_END)
		{
			error("expected 'END'");
			return 0;
		}

		*smt=new_if_stmt(exp,ifsmt,elsesmt);
		(*smt)->line=cur_line;
		return 1;
	}
	else if(token==TOK_WHILE)
	{
		AST_Exp* exp=0;
		AST_Stmt* wsmt=0;

		if(!expr(&exp))
			return 0;

		if(next_token()!=TOK_DO)
		{
			error("expected 'DO'");
			return 0;
		}

		if(!stmt_seq(&wsmt))
			return 0;

		if(next_token()!=TOK_END)
		{
			error("expected 'END'");
			return 0;
		}

		*smt=new_while_stmt(exp,wsmt);
		(*smt)->line=cur_line;
		return 1;
	}
	else if(token==TOK_REPEAT)
	{
		AST_Exp* exp=0;
		AST_Stmt* wsmt=0;

		if(!stmt_seq(&wsmt))
			return 0;

		if(next_token()!=TOK_UNTIL)
		{
			error("expected 'UNTIL'");
			return 0;
		}

		if(!expr(&exp))
			return 0;

		*smt=new_repeat_stmt(exp,wsmt);
		(*smt)->line=cur_line;
		return 1;
	}
	else if(token==TOK_FOR)
	{
		AST_Exp *lowexp=0,*highexp=0,*byexp=0;
		AST_Stmt *wsmt=0,*inismt=0;
		char* it_name=0;

		if(next_token()!=TOK_IDENT)
		{
			error("expected an identifier");
			return 0;
		}
		it_name=get_symbol_name();

		if(next_token()!=TOK_ASSIGN)
		{
			error("expected ':='");
			return 0;
		}

		if(!expr(&lowexp))
			return 0;

		inismt=new_assign(it_name,lowexp);

		if(next_token()!=TOK_TO)
		{
			error("expected 'TO'");
			return 0;
		}

		if(!expr(&highexp))
			return 0;

		if(next_token()==TOK_BY)
		{
			if(!const_expr(&byexp))
				return 0;
		}
		else
			read_next=0;


		if(next_token()!=TOK_DO)
		{
			error("expected 'DO'");
			return 0;
		}

		if(!stmt_seq(&wsmt))
			return 0;

		if(next_token()!=TOK_END)
		{
			error("expected 'END'");
			return 0;
		}


		*smt=new_for_stmt(inismt,highexp,byexp,wsmt);
		(*smt)->line=cur_line;
		return 1;
	}
	/*else if(token==TOK_LOOP)
	{
		if(!stmt_seq())
			return 0;

		if(next_token()!=TOK_END)
		{
			error("expected 'END'");
			return 0;
		}

		return 1;
	}
	else if(token==TOK_EXIT)
		return 1;
	else if(token==TOK_RETURN)
	{
		if(is_firstof_expr(next_token()))
		{
			read_next=0;
			if(!expr())
				return 0;
		}
		else 
			read_next=0;

		return 1;
	}*/
}
/*
int elsif_aux()
{
	//not very useful!!...just for clarity.
	if(next_token()!=TOK_ELSIF)
		return 0;

	if(!expr())
		return 0;

	if(next_token()!=TOK_THEN)
	{
			error("expected 'THEN'");
			return 0;
	}
	if(!stmt_seq())
		return 0;


	if(next_token()==TOK_ELSIF)
	{
		read_next=0;
		if(!elsif_aux())
			return 0;
	}
	else
		read_next=0;
}

int expr_list()
{
	if(!expr())
		return 0;

	if(next_token()==',')
	{
		if(!expr_list())
			return 0;
	}
	else
		read_next=0;


	return 1;
}
*/

int const_expr(AST_Exp** exp)
{
	return expr(exp);
}

int expr(AST_Exp** exp)
{
	AST_Exp* simplexp1=0;
	AST_Exp* simplexp2=0;

	if(!simple_expr(&simplexp1))
		return 0;

	if(is_relation(next_token()))
	{
		int reltype=token;

		if(!simple_expr(&simplexp2))
			return 0;

		*exp=new_op_exp(reltype,simplexp1,simplexp2);
		(*exp)->line=cur_line;

	}
	else
	{
		*exp=simplexp1;
		(*exp)->line=cur_line;
		read_next=0;
	}
	return 1;
}

int simple_expr(AST_Exp** exp)
{
	AST_Exp* termexp1=0;
	AST_Exp* termexp2=0;
	int sign=1;

	next_token();
	if(token=='-')
	{
		sign=-1;
	}
	else if(token!='+')
		read_next=0;

	if(!term(&termexp1))
		return 0;

	termexp1->term_sign=sign;

	if(is_addop(next_token()))
	{
		int optype=token;
		if(!simple_expr_aux(&termexp2))
			return 0;

		*exp=new_op_exp(optype,termexp1,termexp2);
		(*exp)->line=cur_line;

	}
	else
	{
		*exp=termexp1;
		(*exp)->line=cur_line;

		read_next=0;
	}
	return 1;
}

int simple_expr_aux(AST_Exp** exp)
{

	AST_Exp *termexp1=0,*termexp2=0;

	if(!term(&termexp1))
		return 0;

	if(is_addop(next_token()))
	{
		int optype=token;
		if(!simple_expr_aux(&termexp2))
			return 0;

		*exp=new_op_exp(optype,termexp1,termexp2);
		(*exp)->line=cur_line;

	}
	else
	{
		*exp=termexp1;
		(*exp)->line=cur_line;

		read_next=0;
	}

	return 1;
}

int relation()
{
	next_token();
	if(token=='=' || token=='#' || token=='<' || token=='>' || token==TOK_GREATER_EQ || token==TOK_SMALLER_EQ 
		|| token==TOK_IS || token==TOK_IN)
		return 1;
	else
		read_next=0;

	error("expected a relation : '=' | '#' | '<' | '>' | '<=' | '>=' | 'IS' | 'IN'");
	return 0;
}

int is_relation(int t)
{
	return (t=='=' || t=='#' || t=='<' || t=='>' || t==TOK_GREATER_EQ || t==TOK_SMALLER_EQ 
		|| t==TOK_IS || t==TOK_IN);
}

int term(AST_Exp** exp)
{
	AST_Exp *factorexp1=0,*factorexp2=0;

	if(!factor(&factorexp1))
		return 0;

	if(is_mulop(next_token()))
	{
		int optype=token;
		if(!term(&factorexp2))
			return 0;

		*exp=new_op_exp(optype,factorexp1,factorexp2);
		(*exp)->line=cur_line;

	}
	else
	{
		*exp=factorexp1;
		(*exp)->line=cur_line;

		read_next=0;
	}

	return 1;
}

int is_mulop(int t)
{
	return (t==TOK_DIV || t==TOK_MOD || t=='/' || t=='*' || t=='&' );
}

int mulop()
{
	next_token();
	if(token==TOK_DIV || token==TOK_MOD || token=='/' || token=='*' || token=='&' )
		return 1;
	else
		read_next=0;

	error("expected a mulop : '*' | '/' | '&' | 'DIV' | 'MOD'");
	return 0;
}

int is_addop(int t)
{
	return (t==TOK_OR || t=='+' || t=='-');
}
int addop()
{
	next_token();
	if(token==TOK_OR || token=='+' || token=='-' )
		return 1;
	else
		read_next=0;

	error("expected an addop : '-' | '+' | 'OR'");
	return 0;
}

int factor(AST_Exp** exp)
{
	next_token();

	if(token==TOK_IDENT)
	{
		char* sym_name=get_symbol_name();

		/*if(next_token()=='(')//function call
		{
			if(next_token()!=')')
			{
				read_next=0;
				if(!expr_list())
					return 0;

				if(next_token()!=')')
				{
					error("expected ')'");
					return 0;
				}
			}
			

			return 1;
		}
		else
			read_next=0;*/
		//mere variable
		*exp=new_var_exp(sym_name);
		(*exp)->line=cur_line;

		return 1;
	}
	else if(token=='(')
	{
		//if(next_token()!=')')
		//{
			//read_next=0;
			if(!expr(exp))
				return 0;

			if(next_token()!=')')
			{
				error("expected ')'");
				return 0;
			}
		//}

		return 1;
	}
	/*else if(token=='~')
	{
		return factor();
	}*/
	else if(token==TOK_INT_CONST || token==TOK_STRING_CONST || token==TOK_CHAR_CONST || token==TOK_HEX_CONST
		|| token==TOK_NIL || token==TOK_REAL_CONST || token==TOK_TRUE || token==TOK_FALSE)
	{
		*exp=new_literal_exp(token,get_symbol_name());
		(*exp)->line=cur_line;

		return 1;
	}
	else
	{
		error("expected a factor");
		return 0;
	}
}

int is_firstof_expr(int t)
{
	return (t=='+' || t=='-' || t==TOK_IDENT || t==TOK_INT_CONST || t==TOK_STRING_CONST || t==TOK_CHAR_CONST
	 || t==TOK_HEX_CONST || t==TOK_NIL || t=='(' || t=='~');
}