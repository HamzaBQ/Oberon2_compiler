#ifndef VMWARE_P
#define VMWARE_P

#define MAX_STACK 1024
#include "bytecode.h"

typedef struct VM_Value
{
	int type;
	union
	{
		int ival;
		double dval;
	};
} VM_Value;


//virtual machine (stack machine)
extern VM_Value stack[MAX_STACK];
extern int sp;


void interpret_bytecode(Byte_Code* bc);

#endif