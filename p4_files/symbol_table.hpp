#ifndef HOLEYC_SYMBOL_TABLE_HPP
#define HOLEYC_SYMBOL_TABLE_HPP
#include <string>
#include <unordered_map>
#include <list>

#include "ast.hpp" //Access to the TypeNode

//Use an alias template so that we can use
// "HashMap" and it means "std::unordered_map"
template <typename K, typename V>
using HashMap = std::unordered_map<K, V>;

using namespace std;

namespace crona{

//A semantic symbol, which represents a single
// variable, function, etc. Semantic symbols
// exist for the lifetime of a scope in the
// symbol table.
class SemSymbol { //Abstract Parent SemSymbol class
	public:
		SemSymbol() {};
};

class VarSymbol : SemSymbol
{
public:
	VarSymbol(TypeNode* inp_type); //Passes in the type of the variable at instantiation.
	TypeNode* getType() const; //Retrieve the type of var symbol.
private:
	TypeNode* m_type;
};

class FnSymbol : SemSymbol
{
public:
	FnSymbol();
	void addType (TypeNode * inp_type);
	std::list<TypeNode*>* getTypeList () const;
private:
	std::list<TypeNode*>* m_type_list;
}

//A single scope. The symbol table is broken down into a
// chain of scope tables, and each scope table holds
// semantic symbols for a single scope. For example,
// the globals scope will be represented by a ScopeTable,
// and the contents of each function can be represented by
// a ScopeTable.
class ScopeTable {
	public:
		ScopeTable();
		void setEntry(std::string idLit, SemSymbol * symbol);
		//TODO: add functions for looking up symbols
		// and/or returning information to indicate
		// that the symbol does not exist within the
		// current scope.
	private:
		HashMap<std::string, SemSymbol *> * symbols;
};

class SymbolTable{
	public:
		SymbolTable();
		void setEntry(ScopeTable* scopeTableEntry);
		ScopeTable* getScope() const;
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc.
	private:
		std::list<ScopeTable *> * scopeTableChain;
};


}

#endif
