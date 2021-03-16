#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"

namespace crona{

//TODO here is a subset of the nodes needed to do nameAnalysis,
// you should add the rest to allow for a complete treatment
// of any AST

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		"been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	bool res = true;
	ScopeTable * Scope = new ScopeTable();

	symTab->push_front(Scope);
	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	return res;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	SemSymbol * varSymbol = new SemSymbol();

	varSymbol->push_front(myType);
	mySymbol = varSymbol;

	for (auto symbol : *symTab->myFront) {
		if (ID() == symbol->myID) {
			nameAnalysisOk = false;
		}
	}

	if (nameAnalysisOk) {
		symTab->myFront->insert(myID, varSymbol);
	} else {
		throw new ToDoError("[DELETE ME] I'm a varDecl"
			" you should add the information from my"
			" subtree to the symbolTable as a new"
			" entry in the current scope table"
		);
	}

	if (mySymbol == nullptr) {

	}
	return nameAnalysisOk;
}

bool formalDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	/*
	throw new ToDoError("[DELETE ME] I'm a varDecl"
		" you should add the information from my"
		" subtree to the symbolTable as a new"
		" entry in the current scope table"
	);
	*/
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	bool formalAnalysisOk = true;
	bool stmtAnalysisOk = true;
	SemSymbol * fnSymbol;

	fnSymbol->push_front(myRetType);

	for (auto formal : *myFormals){
		fnSymbol->push_front(myType);
	}

	ScopeTable * Scope = new ScopeTable();

	symTab->push_front(Scope);

	for (auto formal : *myFormals){
		formalAnalysisOk = formal->nameAnalysis(symTab) && formalAnalysisOk;
	}

	stmtAnalysisOk = formalAnalysisOk && stmtAnalysisOk;

	if (stmtAnalysisOk) {
		for (auto stmt : *myBody) {
			stmtAnalysisOk = stmt->nameAnalysis(symTab) && stmtAnalysisOk;
		}
	}

	nameAnalysisOk = stmtAnalysisOk && nameAnalysisOk;
	/*
	throw new ToDoError("[DELETE ME] I'm an fnDecl."
		" you should add and make current a new"
		" scope table for my body"
	);
	*/
	return nameAnalysisOk;
}

}
