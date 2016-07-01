#include <iostream>
#include <vector>
using namespace std;

struct Sym {
	string tag,val;
	Sym(string T, string V) { tag=T; val=V; }
	Sym(string V):Sym("sym",V) {}
	vector<Sym*> nest; void push(Sym*o) { nest.push_back(o); }
	virtual string head() { return "<"+tag+":"+val+">"; }
	string pad(int n) { string S; for (int i=0;i<n;i++) S+='\t'; return S; }
	string dump(int depth=0) { string S = "\n"+pad(depth)+head();
		for (auto it=nest.begin(),e=nest.end();it!=e;it++)
			S += (*it)->dump(depth+1);
		return S; }
	virtual Sym* unify(Sym*);
	virtual bool equal(Sym*);
	string vs(Sym*);
	virtual Sym* rval();
};

auto nil = new Sym("nil");
auto fail = new Sym("fail");

string Sym::vs(Sym*o) { return "\n"+head()+" === "+o->head(); }

Sym* Sym::unify(Sym*o) { cout<<vs(o);
	Sym* E1 = this; Sym* E2 = o;
	if (this->equal(o)) return this;			// unify itself or copy
//	if (E1->tag=="sym" && E2->tag=="sym")
//		if (E1->eq(E2)) return nil; else return fail;
	return fail;
}

bool Sym::equal(Sym*o) {
	if (this==o) return true;						// itself
	if (tag==o->tag && val==o->val) return true;	// copy
	return false; }

struct Var: Sym {
	Sym* ref;
	Var(string V) : Sym("var", V) {	ref=nil; }
	string head() { return Sym::head()+"_"+ref->head(); }
	Sym* unify(Sym*o);
	Sym* rval();
};

Sym* Var::unify(Sym*o) { cout<<vs(o);
	if (ref!=nil) return ref->unify(o);
	else { ref=o; return o->rval(); } }

Sym* Sym::rval() { return this; }
Sym* Var::rval() { if (ref) return ref; else return this; }

int main() {
	auto A = new Var("A");
	auto B = new Var("B");
	auto C = new Sym("C");
	cout << B->unify(C)->dump();
	cout << A->unify(B)->dump();
//	cout << A->unify(C)->unify(B)->dump();
}
