#define MAX_ERROR 100

struct Error
{
	char msg[512];	
};

typedef struct Error Error;
extern Error error_table[MAX_ERROR];
extern int num_errors;

void add_error(char* fmt,...);
void print_errors();