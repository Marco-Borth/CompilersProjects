#include "symbol_table.hpp"
namespace crona{

SemSymbol::SemSymbol(){
	m_type_list = new std::list<TypeNode*>;
}

bool SemSymbol::isEmpty() const{
	return (m_type_list->empty()); //If the list of type nodes is empty then return true.
}

TypeNode* SemSymbol::getFIFO() const{
	return (m_type_list->back()); //Returns the first in element of the type node list.
}

void SemSymbol::setEntry (TypeNode* inp_type_entry){
	m_type_list->push_front(inp_type_entry);
}



ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>();
}

SymbolTable::SymbolTable(){
	//TODO: implement the list of hashtables approach
	// to building a symbol table:
	// Upon entry to a scope a new scope table will be
	// entered into the front of the chain and upon exit the
	// latest scope table will be removed from the front of
	// the chain.
	scopeTableChain = new std::list<ScopeTable *>();
}

}