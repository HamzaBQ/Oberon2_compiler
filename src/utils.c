#include <stdio.h>
#include "utils.h"

int db_tree_lvl=0;
void db_tab()
{
	int i;
	for(i=0;i<db_tree_lvl;i++)
		printf(" ");
}