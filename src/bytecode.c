#include "bytecode.h"
#include <stdio.h>
#include "parser.h"

int cur_label=0;
Byte_Code* the_head=0;

void chain_bytecode(Byte_Code** bc1,Byte_Code* bc2)
{
	if(!*bc1)
		*bc1=bc2;
	else
	{
		Byte_Code* it=*bc1;
		while(it->next)it=it->next;
		it->next=bc2;
	}
}


 Byte_Code* gen_bytecode_exp(AST_Exp* exp)
 {

 	if(!exp)
 		return 0;

	Byte_Code* bc = (Byte_Code*)malloc(sizeof(Byte_Code));
	char* tmp;

	switch(exp->type)
	{
		case TOK_INT_CONST :
			bc->val_type=TOK_INT_CONST;
			bc->opcode=PUSH;
			bc->value=exp->literal;
			bc->label=cur_label++;
			break;
		case TOK_REAL_CONST :
				bc->val_type=TOK_REAL_CONST;
				bc->opcode=PUSH;
				bc->value=exp->literal;
				bc->label=cur_label++;
		break;
		case TOK_TRUE :
				bc->val_type=TOK_INT_CONST;
				bc->opcode=PUSH;
				tmp=(char*)malloc(sizeof(char));
				*tmp='1';
				bc->value=tmp;
				bc->label=cur_label++;
		break;
		case TOK_FALSE:
				bc->val_type=TOK_INT_CONST;
				bc->opcode=PUSH;
				tmp=(char*)malloc(sizeof(char));
				*tmp='0';
				bc->value=tmp;
				bc->label=cur_label++;
		break;
		case VAR_EXP:
			bc->opcode=LOAD;
			bc->var_name=exp->var_name;
			bc->label=cur_label++;
			break;
		case OP_EXP:
			switch(exp->op.type)
			{
				case '+' :
				bc->opcode=ADD;
				break;
				case '-':
				bc->opcode=SUB;
				break;
				case '*':
				bc->opcode=MUL;
				break;
				case  '/':
				bc->opcode=DIV;
				break;
				case '=':
				case '#':
				case '>':
				case '<':
				case TOK_GREATER_EQ:
				case TOK_SMALLER_EQ:
				bc->opcode=SUB;
				break;
			}

			Byte_Code* bc_left=gen_bytecode_exp(exp->op.left);
			Byte_Code* bc_right=gen_bytecode_exp(exp->op.right);
			bc->label=cur_label++;
			chain_bytecode(&bc_left,bc_right);
			chain_bytecode(&bc_left,bc);

			bc=bc_left;
		break;
	}

	return bc;
 }

 Byte_Code* gen_bytecode_stmt(AST_Stmt* stmt)
 {
 	if(!stmt)
 		return 0;


	Byte_Code* bc = (Byte_Code*)malloc(sizeof(Byte_Code));


	//HACK---BAD
	static int first=1;
 	if(first)
 	{
 		the_head=bc;
 		first=0;
 	}

 	if(stmt->type==ASSIGN_STMT)
 	{
 		Byte_Code* bc_exp=gen_bytecode_exp(stmt->assign.exp);
 		bc->opcode=STORE;
 		bc->var_name=stmt->assign.var_name;
		bc->label=cur_label++;

 		chain_bytecode(&bc_exp,bc);
 		bc=bc_exp;
 	}
 	if(stmt->type==IF_STMT)
 	{
 		Byte_Code* bc_exp;
 		Byte_Code* bc_elsesmt;
 		Byte_Code* bc_ifsmt;
 		
 		if(stmt->ifelse.exp->op.type=='=')
 		{
 			bc_exp=gen_bytecode_exp(stmt->ifelse.exp);
 			bc_elsesmt=gen_bytecode_stmt(stmt->ifelse.elsesmt);
 			bc_ifsmt=gen_bytecode_stmt(stmt->ifelse.ifsmt);

 			Byte_Code* it=bc_ifsmt;
	 		while(it->next)it=it->next;
	 		Byte_Code* temp;

	 		if(stmt->ifelse.elsesmt)
	 		{
	 			Byte_Code* temp = (Byte_Code*)malloc(sizeof(Byte_Code));
		 		temp->opcode=JMP;
		 		temp->jmpto=cur_label;
		 		temp->label=-1;
		 		chain_bytecode(&bc_ifsmt,temp);
		 	}

	 		temp = (Byte_Code*)malloc(sizeof(Byte_Code));
	 		temp->opcode=JNZ;
	 		if(stmt->ifelse.elsesmt)temp->jmpto=bc_elsesmt->label; else  temp->jmpto=it->label+1;
	 		temp->label=-1;
	 		chain_bytecode(&bc_exp,temp);

	 		chain_bytecode(&bc_exp,bc_ifsmt);
	 		chain_bytecode(&bc_exp,bc_elsesmt);

	 		bc=bc_exp;
	 	}
	 	else if(stmt->ifelse.exp->op.type=='#')
 		{
 			bc_exp=gen_bytecode_exp(stmt->ifelse.exp);
 			bc_elsesmt=gen_bytecode_stmt(stmt->ifelse.elsesmt);
 			bc_ifsmt=gen_bytecode_stmt(stmt->ifelse.ifsmt);

 			Byte_Code* it=bc_ifsmt;
	 		while(it->next)it=it->next;
	 		Byte_Code* temp;

 			if(stmt->ifelse.elsesmt)
	 		{
	 			Byte_Code* temp = (Byte_Code*)malloc(sizeof(Byte_Code));
		 		temp->opcode=JMP;
		 		temp->jmpto=cur_label;
		 		temp->label=-1;
		 		chain_bytecode(&bc_ifsmt,temp);
		 	}

	 		temp = (Byte_Code*)malloc(sizeof(Byte_Code));
	 		temp->opcode=JZ;
	 		if(stmt->ifelse.elsesmt)temp->jmpto=bc_elsesmt->label; else  temp->jmpto=it->label+1;
	 		temp->label=-1;
	 		chain_bytecode(&bc_exp,temp);

	 		chain_bytecode(&bc_exp,bc_ifsmt);
	 		chain_bytecode(&bc_exp,bc_elsesmt);

	 		bc=bc_exp;
 		}
 		else if(stmt->ifelse.exp->op.type=='>')
 		{
 			bc_exp=gen_bytecode_exp(stmt->ifelse.exp);
 			bc_elsesmt=gen_bytecode_stmt(stmt->ifelse.elsesmt);
 			bc_ifsmt=gen_bytecode_stmt(stmt->ifelse.ifsmt);

 			Byte_Code* it=bc_ifsmt;
	 		while(it->next)it=it->next;
	 		Byte_Code* temp;

	 		if(stmt->ifelse.elsesmt)
	 		{
	 			Byte_Code* temp = (Byte_Code*)malloc(sizeof(Byte_Code));
		 		temp->opcode=JMP;
		 		temp->jmpto=cur_label;
		 		temp->label=-1;
		 		chain_bytecode(&bc_ifsmt,temp);
		 	}

	 		temp = (Byte_Code*)malloc(sizeof(Byte_Code));
	 		temp->opcode=JSEZ;
	 		if(stmt->ifelse.elsesmt)temp->jmpto=bc_elsesmt->label; else  temp->jmpto=it->label+1;
	 		temp->label=-1;
	 		chain_bytecode(&bc_exp,temp);

	 		chain_bytecode(&bc_exp,bc_ifsmt);
	 		chain_bytecode(&bc_exp,bc_elsesmt);

	 		bc=bc_exp;
 		}
 		else if(stmt->ifelse.exp->op.type=='<')
 		{
 			bc_exp=gen_bytecode_exp(stmt->ifelse.exp);
 			bc_elsesmt=gen_bytecode_stmt(stmt->ifelse.elsesmt);
 			bc_ifsmt=gen_bytecode_stmt(stmt->ifelse.ifsmt);

 			Byte_Code* it=bc_ifsmt;
	 		while(it->next)it=it->next;
	 		Byte_Code* temp;

 			if(stmt->ifelse.elsesmt)
	 		{
	 			Byte_Code* temp = (Byte_Code*)malloc(sizeof(Byte_Code));
		 		temp->opcode=JMP;
		 		temp->jmpto=cur_label;
		 		temp->label=-1;
		 		chain_bytecode(&bc_ifsmt,temp);
		 	}

	 		temp = (Byte_Code*)malloc(sizeof(Byte_Code));
	 		temp->opcode=JGEZ;
	 		if(stmt->ifelse.elsesmt)temp->jmpto=bc_elsesmt->label; else  temp->jmpto=it->label+1;
	 		temp->label=-1;
	 		chain_bytecode(&bc_exp,temp);

	 		chain_bytecode(&bc_exp,bc_ifsmt);
	 		chain_bytecode(&bc_exp,bc_elsesmt);

	 		bc=bc_exp;
 		}
 		else if(stmt->ifelse.exp->op.type==TOK_GREATER_EQ)
 		{
 			bc_exp=gen_bytecode_exp(stmt->ifelse.exp);
 			bc_elsesmt=gen_bytecode_stmt(stmt->ifelse.elsesmt);
 			bc_ifsmt=gen_bytecode_stmt(stmt->ifelse.ifsmt);

 			Byte_Code* it=bc_ifsmt;
	 		while(it->next)it=it->next;
	 		Byte_Code* temp;

 			if(stmt->ifelse.elsesmt)
	 		{
	 			Byte_Code* temp = (Byte_Code*)malloc(sizeof(Byte_Code));
		 		temp->opcode=JMP;
		 		temp->jmpto=cur_label;
		 		temp->label=-1;
		 		chain_bytecode(&bc_ifsmt,temp);
		 	}

	 		temp = (Byte_Code*)malloc(sizeof(Byte_Code));
	 		temp->opcode=JSZ;
	 		if(stmt->ifelse.elsesmt)temp->jmpto=bc_elsesmt->label; else  temp->jmpto=it->label+1;
	 		temp->label=-1;
	 		chain_bytecode(&bc_exp,temp);

	 		chain_bytecode(&bc_exp,bc_ifsmt);
	 		chain_bytecode(&bc_exp,bc_elsesmt);

	 		bc=bc_exp;
 		}
 		else if(stmt->ifelse.exp->op.type==TOK_SMALLER_EQ)
 		{
 			bc_exp=gen_bytecode_exp(stmt->ifelse.exp);
 			bc_elsesmt=gen_bytecode_stmt(stmt->ifelse.elsesmt);
 			bc_ifsmt=gen_bytecode_stmt(stmt->ifelse.ifsmt);

 			Byte_Code* it=bc_ifsmt;
	 		while(it->next)it=it->next;
	 		Byte_Code* temp;
 			if(stmt->ifelse.elsesmt)
	 		{
	 			Byte_Code* temp = (Byte_Code*)malloc(sizeof(Byte_Code));
		 		temp->opcode=JMP;
		 		temp->jmpto=cur_label;
		 		temp->label=-1;
		 		chain_bytecode(&bc_ifsmt,temp);
		 	}

	 		temp = (Byte_Code*)malloc(sizeof(Byte_Code));
	 		temp->opcode=JGZ;
	 		if(stmt->ifelse.elsesmt)temp->jmpto=bc_elsesmt->label; else  temp->jmpto=it->label+1;
	 		temp->label=-1;
	 		chain_bytecode(&bc_exp,temp);

	 		chain_bytecode(&bc_exp,bc_ifsmt);
	 		chain_bytecode(&bc_exp,bc_elsesmt);

	 		bc=bc_exp;
 		}
 	}
 	else if(stmt->type==WHILE_STMT)
 	{
 		Byte_Code* bc_exp;
 		Byte_Code* bc_smt;
 		int sav_label=cur_label;

 		bc_exp=gen_bytecode_exp(stmt->whilee.exp);
 		bc_smt=gen_bytecode_stmt(stmt->whilee.smt);

 		Byte_Code* it=bc_smt;
 		Byte_Code* prev;
	 	while(it->next){prev=it;it=it->next;}

	 	Byte_Code* temp = (Byte_Code*)malloc(sizeof(Byte_Code));
	 	temp->opcode=JMP;
	 	temp->jmpto=sav_label;
	 	temp->label=cur_label++;
	 	chain_bytecode(&bc_smt,temp);

	 	temp = (Byte_Code*)malloc(sizeof(Byte_Code));
 		if(stmt->whilee.exp->op.type=='=')
 		{
	 		temp->opcode=JNZ;
	 	}
	 	else if(stmt->whilee.exp->op.type=='<')
 		{
	 		temp->opcode=JGEZ;
	 	}
	 	else if(stmt->whilee.exp->op.type=='>')
 		{
	 		temp->opcode=JSEZ;
	 	}
	 	else if(stmt->whilee.exp->op.type==TOK_GREATER_EQ)
 		{
	 		temp->opcode=JSZ;
	 	}
	 	else if(stmt->whilee.exp->op.type==TOK_SMALLER_EQ)
 		{
	 		temp->opcode=JGZ;
	 	}

	 	//@XXX_HACK
	 	
	 	temp->jmpto=it->label+2;

	 	temp->label=-1;
	 	chain_bytecode(&bc_exp,temp);
	 	chain_bytecode(&bc_exp,bc_smt);
	 	bc=bc_exp;
 	}

 	Byte_Code* next_code=gen_bytecode_stmt(stmt->next);

 	chain_bytecode(&bc,next_code);

 	return bc;
 }

 Byte_Code* gen_bytecode(AST_Module* mod)
 {
 	//print_module(mod);
 	return gen_bytecode_stmt(mod->stmt_seq);
 }


 void print_bytecode(Byte_Code* bc)
 {
 	if(!bc)
 		return;
 	printf("%d  ",bc->label);
 	if(bc->opcode==ADD)
 	{
 		printf("ADD\n");
 	}
 	else if(bc->opcode==SUB)
 	{
 		printf("SUB\n");
 	}
 	else if(bc->opcode==MUL)
 	{
 		printf("MUL\n");
 	}
 	else if(bc->opcode==DIV)
 	{
 		printf("DIV\n");
 	}
 	else if(bc->opcode==STORE)
 	{
 		printf("STORE %s\n",bc->var_name);
 	}
 	else if(bc->opcode==LOAD)
 	{
 		printf("LOAD %s\n",bc->var_name);
 	}
 	else if(bc->opcode==PUSH)
 	{
 		printf("PUSH %s\n",bc->value);
 	}
 	else if(bc->opcode==JNZ)
 	{
 		printf("JNZ %d\n",bc->jmpto);
 	}
 	else if(bc->opcode==JMP)
 	{
 		printf("JMP %d\n",bc->jmpto);
 	}
 	else if(bc->opcode==JZ)
 	{
 		printf("JZ %d\n",bc->jmpto);
 	}
 	else if(bc->opcode==JSZ)
 	{
 		printf("JSZ %d\n",bc->jmpto);
 	}
 	else if(bc->opcode==JGZ)
 	{
 		printf("JGZ %d\n",bc->jmpto);
 	}
 	else if(bc->opcode==JSEZ)
 	{
 		printf("JSEZ %d\n",bc->jmpto);
 	}
 	else if(bc->opcode==JGEZ)
 	{
 		printf("JGEZ %d\n",bc->jmpto);
 	}
 	

 	print_bytecode(bc->next);
 }

void save_bytecode_file(Byte_Code* bc,char* filename)
{
	char fn[128];
	filename[strlen(filename)-3]=0;

	strcpy(fn,filename);
	strcat(fn,".bytecode");

	freopen(fn,"wt",stdout);

	print_bytecode(bc);
}