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
class SemSymbol {
	public:
		SemSymbol();
		//Build a destructor to deallocate the dynamic list
		bool isEmpty() const;
		TypeNode * getFIFO() const;
		void setEntry (TypeNode* inp_type_entry);
	private:
		std::list<TypeNode*>* m_type_list;
	//TODO add the fields that
	// each semantic symbol should track
	// (i.e. the kind of the symbol (either a variable or function)
	// and functions to get/set those fields
};

//A single scope. The symbol table is broken down into a
// chain of scope tables, and each scope table holds
// semantic symbols for a single scope. For example,
// the globals scope will be represented by a ScopeTable,
// and the contents of each function can be represented by
// a ScopeTable.
class ScopeTable {
	public:
		ScopeTable();
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
		//TODO: add functions to create a new ScopeTable
		// when a new scope is entered, drop a ScopeTable
		// when a scope is exited, etc.
	private:
		std::list<ScopeTable *> * scopeTableChain;
};


}

#endif
