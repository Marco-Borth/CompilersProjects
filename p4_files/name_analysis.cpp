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

	symTab->setEntry(Scope); //Pushes new scope to table of scopes
	for (auto global : *myGlobals){
		res = global->nameAnalysis(symTab) && res;
	}
	return res;
}

bool VarDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	if (myType->getTypeName() == "void")
	{
		nameAnalysisOk = false;
		Report::fatal(line(), col(),"Invalid type in declaration");

	}
	SemSymbol * varSymbol = new VarSymbol(myType);
	myID->assignSymbol(varSymbol);
	if (!symTab->getScope()->setEntry(ID()->getName(), varSymbol))
	{
		nameAnalysisOk=false;//Keep nameAnalysisOk here to avoid accidentally setting it back to true
		//if it void type but unique.
		Report::fatal(line(), col(), "Multiply declared identifier");
	}
	return nameAnalysisOk;
}

bool FormalDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	if (myType->getTypeName() == "void")
	{
		nameAnalysisOk = false;
		Report::fatal(line(), col(),"Invalid type in declaration");

	}
	SemSymbol * varSymbol = new VarSymbol(myType);
	myID->assignSymbol(varSymbol);
	if (!symTab->getScope()->setEntry(ID()->getName(), varSymbol))
	{
		nameAnalysisOk=false;//Keep nameAnalysisOk here to avoid accidentally setting it back to true
		//if it void type but unique.
		Report::fatal(line(), col(), "Multiply declared identifier");
	}
	return nameAnalysisOk;
}

bool FnDeclNode::nameAnalysis(SymbolTable * symTab){
	bool nameAnalysisOk = true;
	bool formalAnalysisOk = true;
	bool stmtAnalysisOk = true;
	SemSymbol* fnSymbol = new FnSymbol(myRetType);
	myID->assignSymbol(fnSymbol);
	if (!symTab->getScope()->setEntry(ID()->getName(), fnSymbol)) //Is the new identifer unique?
	{
		nameAnalysisOk=false;//Keep nameAnalysisOk here to avoid accidentally setting it back to true
		//if it void type but unique.
		Report::fatal(line(), col(), "Multiply declared identifier");
	}

	symTab->setEntry(new ScopeTable()); //Change scope.

	for (auto formal : *myFormals){ //Are the formals valid unique identifers?
		if (!formal->nameAnalysis(symTab))
		{
			formalAnalysisOk = false;
		}
		fnSymbol->addType(formal->getTypeNode());
	}
	for (auto stmt : *myBody){ //Do the stmts pass name analysis?
		if (!stmt->nameAnalysis(symTab))
		{
			stmtAnalysisOk = false;
		}
	}
	symTab->removeHead();
	return (nameAnalysisOk && formalAnalysisOk && stmtAnalysisOk);
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
	return nameAnalysisOk;
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
	//SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(myCond);
	//symTab->getScope()->setEntry("while", Symbol);

	ScopeTable * Scope = new ScopeTable();

	symTab->setEntry(Scope);

	for (auto stmt : *myBody) {
		nameAnalysisOk = stmt->nameAnalysis(symTab) && nameAnalysisOk;
	}

	return nameAnalysisOk;
}

bool ReturnStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	//SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(myExp);
	//symTab->getScope()->setEntry("return", Symbol);
	//myExp->nameAnalysis(symTab);
	return nameAnalysisOk;
}

bool CallStmtNode::nameAnalysis(SymbolTable * symTab) {
	bool nameAnalysisOk = true;
	SemSymbol * Symbol = new SemSymbol();
	//Symbol->setEntry(myCallExp);
	//symTab->getScope()->setEntry("call", Symbol);
	//myExp->nameAnalysis(symTab);
	return nameAnalysisOk;
}

}
