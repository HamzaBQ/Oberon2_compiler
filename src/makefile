all: lex.yy.o parser.o symbol_table.o AST.o utils.o error.o driver.o semantizer.o bytecode.o vmware.o
	gcc lex.yy.o parser.o symbol_table.o AST.o utils.o driver.o error.o semantizer.o bytecode.o vmware.o -o compiler

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

lex.yy.c: scanner.lex
	flex scanner.lex

parser.o: parser.c
	gcc -c parser.c

symbol_table.o: symbol_table.c
	gcc -c symbol_table.c

AST.o: AST.c
	gcc -c AST.c

driver.o: driver.c
	gcc -c driver.c

semantizer.o: semantizer.c
	gcc -c semantizer.c

error.o: error.c
	gcc -c error.c

utils.o: utils.c
	gcc -c utils.c

bytecode.o: bytecode.c
	gcc -c bytecode.c

vmware.o: vmware.c
	gcc -c vmware.c