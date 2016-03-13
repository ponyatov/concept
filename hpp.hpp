#ifndef _H_CONCEPT
#define _H_CONCEPT

#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

struct Sym {
	string tag,val;
	Sym(string,string); Sym(string);
	virtual string tagval(); string tagstr(); 
	virtual string dump(int depth=0); string pad(int);
	vector<Sym*> nest; void push(Sym*o);
	map<string,Sym*> par; Sym* lookup(string);
	virtual Sym* eval(Sym*env);
	virtual Sym* str();
	virtual Sym* eq(Sym*);
	virtual Sym* at(Sym*);
};
extern Sym* glob;
extern void env_init();

struct Str: Sym { Str(string); string tagval(); };

struct List: Sym { List(); };

struct Op: Sym { Op(string); Sym*eval(Sym*); };

typedef Sym*(*FN)(Sym*);
struct Fn: Sym { Fn(string,FN); FN fn; Sym*at(Sym*); };

struct File: Sym { File(string); static Sym* file(Sym*); string tagval(); };

struct Module: Sym { Module(string); };

struct Var: Sym { Var(Sym*); Sym*ptr; Sym*eq(Sym*); string dump(int); };

extern int yylex();
extern int yylineno;
extern char* yytext;
#define TOC(C,X) { yylval.o = new C(yytext); return X; }
extern int yyparse();
extern void yyerror(string);
extern Module unit;
#include "ypp.tab.hpp"

#endif // _H_CONCEPT

