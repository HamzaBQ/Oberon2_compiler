#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "symbol.h"
#include "symbol_table.h"
#include "utils.h"

Variable* var_table[MAX_VAR];
int num_variables=0;

Variable* add_variable(char* name,int type,char* val,int cnst)
{
	Variable* var=(Variable*)malloc(sizeof(Variable));
	var->name=name;
	var->val=val;
	var->type=type;
	var->constant=cnst;
	var_table[num_variables++]=var;

	return var;
}

void set_var_value(char* name,char* val)
{
	Variable* var=get_variable(name);

	if(var->val)
		free(var->val);

	var->val=val;
}

Variable* get_variable(char* name)//NULL if Symbol doesn't exist!
{
	int i;
	for(i=0;i<num_variables;i++)
	{
		if(!strcmp(var_table[i]->name,name))
		{
			
			return var_table[i];
		}
	}

	return 0;
}


void st_print()
{
	printf("   SYMBOL TABLE:\n num_variables : %d \n variables:\n",num_variables);
	int i;
	db_tree_lvl+=5;
	for(i=0;i<num_variables;i++)
	{
		db_tab();
		printf("-%s :\n",var_table[i]->name);
		db_tree_lvl+=2;
		db_tab();
		printf("-value : %s\n",var_table[i]->val);

		db_tab();
		if(var_table[i]->type==TOK_INTEGER_TYPE)
			printf("-type : integer\n");
		else if(var_table[i]->type==TOK_REAL_TYPE)
			printf("-type : double\n");
		else if(var_table[i]->type==TOK_BOOL_TYPE)
			printf("-type : bool\n");
		if(var_table[i]->constant)
		{
			db_tab();
			printf("-Constant\n");
		}
		db_tree_lvl-=2;
	}

	db_tree_lvl-=5;
}