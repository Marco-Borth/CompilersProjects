#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"

namespace crona{

//TODO here is a subset of the nodes needed to do nameAnalysis,
// you should add the rest to allow for a complete treatment
// of any AST

bool ASTNode::nameAnalysis(SymbolTable * symTab){
	throw new ToDoError("This function should have"
		" been overriden in the subclass!");
}

bool ProgramNode::nameAnalysis(SymbolTable * symTab){
	bool res = true;
	ScopeTable * Scope = new ScopeTable();

	symTab->setEntry(Scope);
	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	return res;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	SemSymbol * varSymbol = new SemSymbol();
	IDNode * id = ID();

	varSymbol->setEntry(getTypeNode());

	/*
	ScopeTable * varScope = symTab->getScope();
	for (auto symbol : *varScope) {
		if (ID() == symbol->getFIFO()->myID) {
			nameAnalysisOk = false;
		}
	}
	*/

	std::string symbolMsg = ID()->getName();
	symbolMsg == symbolMsg + "()";

	id->assignSymbol(varSymbol);

	if (nameAnalysisOk) {
		symTab->getScope()->setEntry(ID()->getName(), varSymbol);
	} else {
		/*
		throw new ToDoError("[DELETE ME] I'm a varDecl"
			" you should add the information from my"
			" subtree to the symbolTable as a new"
			" entry in the current scope table"
		);
		*/
	}

	return nameAnalysisOk;
}

bool FormalDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	SemSymbol * varSymbol = new SemSymbol();
	IDNode * id = ID();

	varSymbol->setEntry(getTypeNode());

	/*
	ScopeTable * varScope = symTab->getScope();
	for (auto symbol : *varScope) {
		if (ID() == symbol->getFIFO()->myID) {
			nameAnalysisOk = false;
		}
	}
	*/

	std::string symbolMsg = ID()->getName();
	symbolMsg == symbolMsg + "()";

	id->assignSymbol(varSymbol);

	if (nameAnalysisOk) {
		symTab->getScope()->setEntry(ID()->getName(), varSymbol);
	} else {
		/*
		throw new ToDoError("[DELETE ME] I'm a varDecl"
			" you should add the information from my"
			" subtree to the symbolTable as a new"
			" entry in the current scope table"
		);
		*/
	}

	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	bool formalAnalysisOk = true;
	bool stmtAnalysisOk = true;
	SemSymbol * fnSymbol = new SemSymbol();
	IDNode * id = ID();

	/*
	for (auto symbol : *varScope) {
		if (ID() == symbol->myID) {
			nameAnalysisOk = false;
		}
	}
	*/

	for (auto formal : *myFormals){
		fnSymbol->setEntry(formal->getTypeNode());
	}

	fnSymbol->setEntry(myRetType);

	symTab->getScope()->setEntry(ID()->getName(), fnSymbol);

	id->assignSymbol(fnSymbol);

	ScopeTable * Scope = new ScopeTable();

	symTab->setEntry(Scope);

	for (auto formal : *myFormals){
		formalAnalysisOk = formal->nameAnalysis(symTab) && formalAnalysisOk;
	}

	stmtAnalysisOk = formalAnalysisOk && stmtAnalysisOk;

	if (stmtAnalysisOk) {
		for (auto stmt : *myBody) {
			//if(stmt->)
			stmtAnalysisOk = stmt->nameAnalysis(symTab) && stmtAnalysisOk;
		}
	}

	nameAnalysisOk = stmtAnalysisOk && nameAnalysisOk;
	if(!nameAnalysisOk) {
		/*
		throw new ToDoError("[DELETE ME] I'm an fnDecl."
			" you should add and make current a new"
			" scope table for my body"
		);
		*/
	}
	return nameAnalysisOk;
}

bool AssignStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	//SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(myExp);
	//symTab->getScope()->setEntry("assign", Symbol);
	//myExp->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool ReadStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	//SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(myDst);
	//symTab->getScope()->setEntry("read", Symbol);
	//myExp->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool WriteStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	//SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(mySrc);
	//symTab->getScope()->setEntry("write", Symbol);
	//myExp->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool PostDecStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	//SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(myLVal);
	//symTab->getScope()->setEntry("inc", Symbol);
	//myExp->nameAnalysis(symTab);


	return myLVal->nameAnalysis(symTab);
	//return nameAnalysisOk;
}

bool PostIncStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	//SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(myLVal);
	//symTab->getScope()->setEntry("dec", Symbol);
	//myExp->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool IfStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	//SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(myCond);
	//symTab->getScope()->setEntry("if", Symbol);

	ScopeTable * Scope = new ScopeTable();

	symTab->setEntry(Scope);

	for (auto stmt : *myBody) {
		nameAnalysisOk = stmt->nameAnalysis(symTab) && nameAnalysisOk;
	}

	return nameAnalysisOk;
}

bool IfElseStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	//SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(myCond);
	//symTab->getScope()->setEntry("ifElse", Symbol);

	ScopeTable * Scope = new ScopeTable();

	symTab->setEntry(Scope);

	for (auto stmt : *myBodyTrue) {
		nameAnalysisOk = stmt->nameAnalysis(symTab) && nameAnalysisOk;
	}

	for (auto stmt : *myBodyFalse) {
		nameAnalysisOk = stmt->nameAnalysis(symTab) && nameAnalysisOk;
	}

	return nameAnalysisOk;
}

bool WhileStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	nameAnalysisOk = nameAnalysisOk && myCond->nameAnalysis(symTab);
	if(nameAnalysisOk) {
		ScopeTable * Scope = new ScopeTable();
		symTab->setEntry(Scope);

		for (auto stmt : *myBody) {
			nameAnalysisOk = stmt->nameAnalysis(symTab) && nameAnalysisOk;
		}
	}

	return nameAnalysisOk;
}

bool ReturnStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myExp->nameAnalysis(symTab);
}

bool CallStmtNode::nameAnalysis(SymbolTable * symTab) {
	return myCallExp->nameAnalysis(symTab);;
}

bool UnaryExpNode::nameAnalysis(SymbolTable * symTab) {
	return myExp->nameAnalysis(symTab);
}

bool BinaryExpNode::nameAnalysis(SymbolTable * symTab) {
	return myExp1->nameAnalysis(symTab) && myExp2->nameAnalysis(symTab);
}

bool AssignExpNode::nameAnalysis(SymbolTable * symTab) {
	return myDst->nameAnalysis(symTab) && mySrc->nameAnalysis(symTab);
}

bool CallExpNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	nameAnalysisOk = nameAnalysisOk && myID->nameAnalysis(symTab);

	if(nameAnalysisOk) {
		for (auto exp : * myArgs) {
			nameAnalysisOk = nameAnalysisOk && exp->nameAnalysis(symTab);
		}
	}

	return nameAnalysisOk;
}

bool IndexNode::nameAnalysis(SymbolTable * symTab) {
	return myBase->nameAnalysis(symTab) && myOffset->nameAnalysis(symTab);
}

bool IDNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	for(auto scope : *symTab->returnList()) {
		for(auto symbol : *scope->returnHashMap()) {
			if(symbol.first == getName()) {
				mySymbol = symbol.second;
			}
		}
	}
	if (mySymbol == nullptr) {
		nameAnalysisOk = false;
	}

	return nameAnalysisOk;
}



}
