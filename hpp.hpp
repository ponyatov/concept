#ifndef _H_CONCEPT
#define _H_CONCEPT

#include <iostream>
#include <cstdlib>
using namespace std;

struct Sym {
	string val;
	Sym(string);
	string dump();
};

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
#include "ypp.tab.hpp"

#endif // _H_CONCEPT

