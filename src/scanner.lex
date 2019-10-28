%{
#include "parser.h"
%}

%option noyywrap

alpha					[a-zA-Z]
alnum					[a-zA-Z0-9]
digit 					[0-9]
hex_digit				{digit}|"A"|"B"|"C"|"D"|"E"|"F"
scale_factor			[ED][+-]{digit}{digit}*


%%

"IF"															return TOK_IF;cur_column+=yyleng;
"IMPORT"														return TOK_IMPORT;cur_column+=yyleng;
"IN"															return TOK_IN;cur_column+=yyleng;
"IS"															return TOK_IS;cur_column+=yyleng;
"LOOP"															return TOK_LOOP;cur_column+=yyleng;
"MOD"															return TOK_MOD;cur_column+=yyleng;
"NIL"															return TOK_NIL;cur_column+=yyleng;
"OF"															return TOK_OF;cur_column+=yyleng;
"OR"															return TOK_OR;cur_column+=yyleng;
"POINTER"														return TOK_POINTER;cur_column+=yyleng;
"PROCEDURE"														return TOK_PROCEDURE;cur_column+=yyleng;
"RECORD"														return TOK_RECORD;cur_column+=yyleng;
"REPEAT"														return TOK_REPEAT;cur_column+=yyleng;
"RETURN"														return TOK_RETURN;cur_column+=yyleng;
"THEN"															return TOK_THEN;cur_column+=yyleng;
"MODULE"														return TOK_MODULE;cur_column+=yyleng;
"BEGIN"															return TOK_BEGIN;cur_column+=yyleng;
"ARRAY"															return TOK_ARRAY;cur_column+=yyleng;
"CASE"															return TOK_CASE;cur_column+=yyleng;
"CONST"															return TOK_CONST;cur_column+=yyleng;
"DIV"															return TOK_DIV;cur_column+=yyleng;
"DO"															return TOK_DO;cur_column+=yyleng;
"ELSE"															return TOK_ELSE;cur_column+=yyleng;
"ELSIF"															return TOK_ELSIF;cur_column+=yyleng;
"END"															return TOK_END;cur_column+=yyleng;
"BY"															return TOK_BY;cur_column+=yyleng;
"FOR"															return TOK_FOR;cur_column+=yyleng;
"EXIT"															return TOK_EXIT;cur_column+=yyleng;
"WITH"															return TOK_WITH;cur_column+=yyleng;
"WHILE"															return TOK_WHILE;cur_column+=yyleng;
"VAR"															return TOK_VAR;cur_column+=yyleng;
"TYPE"															return TOK_TYPE;cur_column+=yyleng;
"TO"															return TOK_TO;cur_column+=yyleng;
"UNTIL"															return TOK_UNTIL;cur_column+=yyleng;

"CHAR"															return TOK_CHAR_TYPE;cur_column+=yyleng;
"INTEGER"														return TOK_INTEGER_TYPE;cur_column+=yyleng;
"REAL"															return TOK_REAL_TYPE;cur_column+=yyleng;
"BOOLEAN"														return TOK_BOOL_TYPE;cur_column+=yyleng;
"SHORTINT"														return TOK_SHORTINT_TYPE;cur_column+=yyleng;
"LONGINT"														return TOK_LONGINT_TYPE;cur_column+=yyleng;
"LONGREAL"														return TOK_LONGREAL_TYPE;cur_column+=yyleng;
"SET"															return TOK_SET_TYPE;cur_column+=yyleng;
"TRUE"															return TOK_TRUE;cur_column+=yyleng;
"FALSE"															return TOK_FALSE;cur_column+=yyleng;													

":="															return TOK_ASSIGN;cur_column+=yyleng;
">="															return TOK_GREATER_EQ;cur_column+=yyleng;
"<="															return TOK_SMALLER_EQ;cur_column+=yyleng;

":"																return ':';cur_column+=yyleng;
"."																return '.';cur_column+=yyleng;
"+"																return '+';cur_column+=yyleng;
"-"																return '-';cur_column+=yyleng;
"*"																return '*';cur_column+=yyleng;
"/"																return '/';cur_column+=yyleng;
";"																return ';';cur_column+=yyleng;
","																return ',';cur_column+=yyleng;
">"																return '>';cur_column+=yyleng;
"<"																return '<';cur_column+=yyleng;
"("																return '(';cur_column+=yyleng;
")"																return ')';cur_column+=yyleng;
"~"																return '~';cur_column+=yyleng;
"&"																return '&';cur_column+=yyleng;
"|"																return '|';cur_column+=yyleng;
"^"																return '^';cur_column+=yyleng;
"#"																return '#';cur_column+=yyleng;
"["																return '[';cur_column+=yyleng;
"]"																return ']';cur_column+=yyleng;
"{"																return '{';cur_column+=yyleng;
"}"																return '}';cur_column+=yyleng;
"="																return '=';cur_column+=yyleng;

{alpha}{alnum}*													return TOK_IDENT;cur_column+=yyleng;
{digit}{digit}*													return TOK_INT_CONST;cur_column+=yyleng;
{digit}{hex_digit}*"H"											return TOK_HEX_CONST;cur_column+=yyleng;
{digit}{digit}*"."{digit}*{scale_factor}?						return TOK_REAL_CONST;cur_column+=yyleng;
{digit}{hex_digit}*"X"											return TOK_CHAR_CONST;cur_column+=yyleng;
"'"[^\n']*"'"|\"[^\n\"]*\"										return TOK_STRING_CONST;cur_column+=yyleng;

"(*".*"*)"														cur_column+=yyleng;
"\n"															cur_line++;cur_column=0;										
[ \t\v\f]														cur_column+=yyleng;
.																//return -1000;

%%

/*
int main( int argc, char **argv )
{
++argv, --argc;
if ( argc > 0 )
yyin = fopen( argv[0], "r" );
else
yyin = stdin;

int c;
	while(c==yylex())printf("%d ",c);

}
*/
