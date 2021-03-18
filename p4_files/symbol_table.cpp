#include "symbol_table.hpp"
namespace crona{

SemSymbol::SemSymbol(TypeNode* inp_type)
{
	m_type = inp_type;
	fnSymbol = false;
}

void SemSymbol::convertToFn() {
	fnSymbol = true;
}

bool SemSymbol::isFn(){
	return fnSymbol;
}

std::list<TypeNode*>* SemSymbol::returnList() {
	return m_type_list;
}

TypeNode* SemSymbol::getType() const
{
	return (m_type);
}

void SemSymbol::addType(TypeNode* inp_type){
	m_type_list->push_front(inp_type);
}

/*
std::string FnSymbol::print() const
{
	//Fn Print funct goes here.
}
*/


ScopeTable::ScopeTable(){
	symbols = new HashMap<std::string, SemSymbol *>();
}

bool ScopeTable::setEntry(std::string idLit, SemSymbol * symbol){
	return (symbols->emplace(idLit, symbol).second); //Emplace returns a pair. The first element
	//is an iterator relating where the new pair we added is located in the scope table hash. The
	//second element is true the entry was unique and added or false if the entry was not unique
	//and not added to the hash map.
}

HashMap<std::string, SemSymbol *> * ScopeTable::returnHashMap(){
	return symbols;
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

void SymbolTable::setEntry (ScopeTable* scopeTableEntry){
	scopeTableChain->push_front(scopeTableEntry);
}

ScopeTable* SymbolTable::getScope() const{
	return (scopeTableChain->front()); //Returns the lastest element pushed of the ScopeTable list.
}
void SymbolTable::removeHead(){
		scopeTableChain->pop_front();
}

std::list<ScopeTable *> * SymbolTable::returnList() {
	return scopeTableChain;
}

}
