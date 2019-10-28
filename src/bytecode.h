#ifndef BYTECODE_H
#define BYTECODE_H

#include "AST.h"

enum {ADD,MUL,DIV,SUB,PUSH,LOAD,SWAP,STORE,JMP,JNZ,JZ,JGEZ,JGZ,JSZ,JSEZ};


extern int cur_label;
typedef struct Byte_Code{
	int opcode;
	char* var_name;
    char* value;
    int label;
    int jmpto;
    int val_type;
	struct Byte_Code* next;
}Byte_Code;



void chain_bytecode(Byte_Code** bc1,Byte_Code* bc2);

Byte_Code* gen_bytecode(AST_Module* mod);
Byte_Code* gen_bytecode_exp(AST_Exp* exp);
Byte_Code* gen_bytecode_stmt(AST_Stmt* stmt);

 //print bytecode

void print_bytecode(Byte_Code* bc);
void save_bytecode_file(Byte_Code* bc,char* filename);

extern Byte_Code* the_head;
#endif 

