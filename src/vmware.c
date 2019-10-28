#include "vmware.h"
#include "symbol_table.h"
#include "symbol.h"
#include "parser.h"

VM_Value stack[MAX_STACK];
int sp=0;

void interpret_bytecode(Byte_Code* bc)
{

	if(!bc)
 		return;

 	if(bc->opcode==ADD)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			int a=stack[sp-1].ival,b=stack[sp-2].ival;
 			sp--;
 			stack[sp-1].ival=a+b;
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			double a=stack[sp-1].dval,b=stack[sp-2].dval;
 			sp--;
 			stack[sp-1].dval=a+b;
 		}
 	}
 	else if(bc->opcode==SUB)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			int a=stack[sp-1].ival,b=stack[sp-2].ival;
 			sp--;
 			stack[sp-1].ival=b-a;
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			double a=stack[sp-1].dval,b=stack[sp-2].dval;
 			sp--;
 			stack[sp-1].dval=b-a;
 		}
 	}
 	else if(bc->opcode==MUL)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			int a=stack[sp-1].ival,b=stack[sp-2].ival;
 			sp--;
 			stack[sp-1].ival=a*b;
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			double a=stack[sp-1].dval,b=stack[sp-2].dval;
 			sp--;
 			stack[sp-1].dval=a*b;
 		}
 	}
 	else if(bc->opcode==DIV)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			int a=stack[sp-1].ival,b=stack[sp-2].ival;
 			sp--;
 			stack[sp-1].ival=b/a;
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			double a=stack[sp-1].dval,b=stack[sp-2].dval;
 			sp--;
 			stack[sp-1].dval=b/a;
 		}
 	}
 	else if(bc->opcode==STORE)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			Variable* var=get_variable(bc->var_name);
 			sprintf(var->val,"%d",stack[sp-1].ival);
 			sp--;
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			Variable* var=get_variable(bc->var_name);
 			sprintf(var->val,"%f",stack[sp-1].dval);
 			sp--;
 		}
 	}
 	else if(bc->opcode==LOAD)
 	{
 		Variable* var=get_variable(bc->var_name);
 		if(var->type==TOK_INTEGER_TYPE)
 		{
 			int val=atoi(var->val);
 			stack[sp].type=TOK_INT_CONST;
 			stack[sp].ival=val;
 			sp++;
 		}
 		else if(var->type==TOK_REAL_TYPE)
 		{
 			double val;
 			sscanf(var->val,"%lf",&val);
 			stack[sp].type=TOK_REAL_CONST;
 			stack[sp].dval=val;
 			sp++;
 		}

 	}
 	else if(bc->opcode==PUSH)
 	{
 		if(bc->val_type==TOK_INT_CONST)
 		{
 			int val=atoi(bc->value);
 			stack[sp].type=TOK_INT_CONST;
 			stack[sp].ival=val;
 			sp++;
 		}
 		else if(bc->val_type==TOK_REAL_CONST)
 		{
 			double val;
 			sscanf(bc->value,"%lf",&val);
 			stack[sp].type=TOK_REAL_CONST;
 			stack[sp].dval=val;
 			sp++;
 		}
 	}
 	else if(bc->opcode==JNZ)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			if(stack[sp-1].ival)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			if(fabs(stack[sp-1].dval)<1e-8)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}

 		sp--;
 	}
 	else if(bc->opcode==JMP)
 	{
 		Byte_Code* it=the_head;
		while(it && it->label!=bc->jmpto)it=it->next;
		static int kkk=0;

		interpret_bytecode(it);


		return;
 	}
 	else if(bc->opcode==JZ)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			if(!stack[sp-1].ival)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			if(fabs(stack[sp-1].dval)>=1e-8)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}

 		sp--;
 	}
 	else if(bc->opcode==JSZ)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			if(stack[sp-1].ival<0)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			if(fabs(stack[sp-1].dval)<0.0f)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}

 		sp--;
 	}
 	else if(bc->opcode==JSEZ)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			if(stack[sp-1].ival<=0)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			if(fabs(stack[sp-1].dval)<=0.0f)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}

 		sp--;
 	}
 	else if(bc->opcode==JGZ)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			if(stack[sp-1].ival>0)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			if(fabs(stack[sp-1].dval)>0.0f)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}

 		sp--;
 	}
 	else if(bc->opcode==JGEZ)
 	{
 		if(stack[sp-1].type==TOK_INT_CONST)
 		{
 			if(stack[sp-1].ival>=0)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}
 		else if(stack[sp-1].type==TOK_REAL_CONST)
 		{
 			if(fabs(stack[sp-1].dval)>=0.0f)
 			{
	 			sp--;
		 		Byte_Code* it=the_head;
		 		while(it && it->label!=bc->jmpto)it=it->next;

		 		interpret_bytecode(it);
		 		return;
 			}
 		}

 		sp--;
 	}

 	interpret_bytecode(bc->next);
 }