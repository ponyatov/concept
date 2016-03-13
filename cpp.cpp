#include "hpp.hpp"
#define YYERR "\n\n"<<yylineno<<":"<<msg<<"["<<yytext<<"]\n\n"
void yyerror(string msg) { cout<<YYERR; cerr<<YYERR; exit(-1); }
int main() { env_init(); return yyparse(); }

Sym::Sym(string T, string V) { tag=T; val=V; }
Sym::Sym(string V):Sym("",V) {}
void Sym::push(Sym*o) { nest.push_back(o); }

string Sym::dump(int depth) { string S = "\n" + pad(depth) + tagval();
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		S += (*it)->dump(depth+1);
	return S; }
string Sym::pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
string Sym::tagval() { return "<"+tag+":"+val+">"; }
string Sym::tagstr() { return "<"+tag+":'"+val+"'>"; }

Sym* glob = new Sym("env","global");

Sym* Sym::lookup(string V) {
	auto E = par.find(V);
	if (E!=par.end()) return E->second;
	else return NULL;
}

Sym* Sym::eval(Sym*env) {
	Sym* E = env->lookup(val); if (E) return E;
	for (auto it=nest.begin(),e=nest.end();it!=e;it++)
		(*it) = (*it)->eval(env);
	return this; }

Sym* Sym::str() { return new Str(val); }
Sym* Sym::at(Sym*o) { push(o); return this; }

Str::Str(string V):Sym("str",V) {}
string Str::tagval() { return tagstr(); }

List::List():Sym("[","]") {}

Op::Op(string V):Sym("op",V) {}
Sym* Op::eval(Sym*env) {
	Sym::eval(env);
	if (val=="@") return nest[0]->at(nest[1]);
	return this;
}

Fn::Fn(string V, FN F):Sym("fn",V) { fn=F; }
Sym* Fn::at(Sym*o) { return fn(o); }

File::File(string V):Sym("file",V) {}
Sym* File::file(Sym*o) { return new File(o->str()->val); }
string File::tagval() { return tagstr(); }

void env_init() {
	glob->par["file"] = new Fn("file",File::file);
}
