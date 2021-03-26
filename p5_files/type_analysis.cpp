#include "ast.hpp"
#include "symbol_table.hpp"
#include "errors.hpp"
#include "types.hpp"
#include "name_analysis.hpp"
#include "type_analysis.hpp"

namespace crona{

TypeAnalysis * TypeAnalysis::build(NameAnalysis * nameAnalysis){
	//To emphasize that type analysis depends on name analysis
	// being complete, a name analysis must be supplied for
	// type analysis to be performed.
	TypeAnalysis * typeAnalysis = new TypeAnalysis();
	auto ast = nameAnalysis->ast;
	typeAnalysis->ast = ast;

	ast->typeAnalysis(typeAnalysis);
	if (typeAnalysis->hasError){
		return nullptr;
	}

	return typeAnalysis;

}

void ProgramNode::typeAnalysis(TypeAnalysis * ta){

	//pass the TypeAnalysis down throughout
	// the entire tree, getting the types for
	// each element in turn and adding them
	// to the ta object's hashMap
	for (auto global : *myGlobals){
		global->typeAnalysis(ta);
	}

	//The type of the program node will never
	// be needed. We can just set it to VOID
	//(Alternatively, we could make our type
	// be error if the DeclListNode is an error)
	ta->nodeType(this, BasicType::produce(VOID));
}

void FnDeclNode::typeAnalysis(TypeAnalysis * ta){

	//HINT: you might want to change the signature for
	// typeAnalysis on FnBodyNode to take a second
	// argument which is the type of the current
	// function. This will help you to know at a
	// return statement whether the return type matches
	// the current function

	//Note: this function may need extra code

	for (auto stmt : *myBody){
		stmt->typeAnalysis(ta);
	}
}

void StmtNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Implement me in the subclass");
}

void AssignStmtNode::typeAnalysis(TypeAnalysis * ta){
	myExp->typeAnalysis(ta);

	//It can be a bit of a pain to write
	// "const DataType *" everywhere, so here
	// the use of auto is used instead to tell the
	// compiler to figure out what the subType variable
	// should be
	auto subType = ta->nodeType(myExp);

	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself
	if (subType->asError()){
		ta->nodeType(this, subType);
	} else {
		ta->nodeType(this, BasicType::produce(VOID));
	}
}

void WriteStmtNode::typeAnalysis(TypeAnalysis * ta){
	mySrc->typeAnalysis(ta); //Sets an entry in the Type Hash Table nodeToType
	const DataType * tgtType = ta->nodeType(mySrc); //Retrieves the DataType in the hash table
	if (tgtType->getString() != "void") {
		ta->nodeType(this, tgtType);
	 	return;
	}
	ta->errWriteVoid(this->line(), this->col()); //Outputs error message if we try to write a void value.

	//It can be a bit of a pain to write
	// "const DataType *" everywhere, so here
	// the use of auto is used instead to tell the
	// compiler to figure out what the subType variable
	// should be
	auto subType = ta->nodeType(mySrc);

	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself
	if (subType->asError()){
		ta->nodeType(this, subType);
	} else {
		ta->nodeType(this, BasicType::produce(VOID));
	}
}




void IfStmtNode::typeAnalysis(TypeAnalysis * ta){
	myCond->typeAnalysis(ta);

	const DataType * condType = ta->nodeType(myCond);

	if(condType->getString() == "bool") {
		for (auto stmt : *myBody){
			stmt->typeAnalysis(ta);
		}
		ta->nodeType(this, condType);
		return;
	} else {
		ta->errIfCond(this->line(), this->col());
		ta->nodeType(this, ErrorType::produce());
	}

	//It can be a bit of a pain to write
	// "const DataType *" everywhere, so here
	// the use of auto is used instead to tell the
	// compiler to figure out what the subType variable
	// should be
	auto subType = ta->nodeType(myCond);

	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself
	if (subType->asError()){
		ta->nodeType(this, subType);
	} else {
		ta->nodeType(this, BasicType::produce(VOID));
	}
}

void IfElseStmtNode::typeAnalysis(TypeAnalysis * ta){
	myCond->typeAnalysis(ta);

	const DataType * condType = ta->nodeType(myCond);

	if(condType->getString() == "bool") {
		for (auto stmt : *myBodyTrue){
			stmt->typeAnalysis(ta);
		}
		for (auto stmt : *myBodyFalse){
			stmt->typeAnalysis(ta);
		}
		ta->nodeType(this, condType);
		return;
	} else {
		ta->errIfCond(this->line(), this->col());
		ta->nodeType(this, ErrorType::produce());
	}

	//It can be a bit of a pain to write
	// "const DataType *" everywhere, so here
	// the use of auto is used instead to tell the
	// compiler to figure out what the subType variable
	// should be
	auto subType = ta->nodeType(myCond);

	// As error returns null if subType is NOT an error type
	// otherwise, it returns the subType itself
	if (subType->asError()){
		ta->nodeType(this, subType);
	} else {
		ta->nodeType(this, BasicType::produce(VOID));
	}
}

void ExpNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void AssignExpNode::typeAnalysis(TypeAnalysis * ta){
	//TODO: Note that this function is incomplete.
	// and needs additional code

	//Do typeAnalysis on the subexpressions
	myDst->typeAnalysis(ta);
	mySrc->typeAnalysis(ta);

	const DataType * tgtType = ta->nodeType(myDst);
	const DataType * srcType = ta->nodeType(mySrc);

	//While incomplete, this gives you one case for
	// assignment: if the types are exactly the same
	// it is usually ok to do the assignment. One
	// exception is that if both types are function
	// names, it should fail type analysis
	if (tgtType == srcType){
		ta->nodeType(this, tgtType);
		return;
	}

	//Some functions are already defined for you to
	// report type errors. Note that these functions
	// also tell the typeAnalysis object that the
	// analysis has failed, meaning that main.cpp
	// will print "Type check failed" at the end
	ta->errAssignOpr(this->line(), this->col());


	//Note that reporting an error does not set the
	// type of the current node, so setting the node
	// type must be done
	ta->nodeType(this, ErrorType::produce());
}

void BinaryExpNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void ArithmeticExpNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	const DataType * myExp1Type = ta->nodeType(myExp1);
	const DataType * myExp2Type = ta->nodeType(myExp2);

	if (myExp1Type == myExp2Type &&
			myExp1Type->getString() == "int" &&
			myExp2Type->getString() == "int"){
		ta->nodeType(this, myExp1Type);
		return;
	}
	ta->errMathOpd(this->line(), this->col());
	ta->nodeType(this, ErrorType::produce());
}

void RelationalExpNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	const DataType * myExp1Type = ta->nodeType(myExp1);
	const DataType * myExp2Type = ta->nodeType(myExp2);

	if (myExp1Type == myExp2Type &&
			myExp1Type->getString() == "int" &&
			myExp2Type->getString() == "int"){
		ta->nodeType(this, BasicType::produce(BOOL));
		return;
	}
	ta->errRelOpd(this->line(), this->col());
	ta->nodeType(this, ErrorType::produce());
}

void LogicalExpNode::typeAnalysis(TypeAnalysis * ta){
	myExp1->typeAnalysis(ta);
	myExp2->typeAnalysis(ta);

	const DataType * myExp1Type = ta->nodeType(myExp1);
	const DataType * myExp2Type = ta->nodeType(myExp2);

	if (myExp1Type == myExp2Type &&
			myExp1Type->getString() == "bool" &&
			myExp2Type->getString() == "bool"){
		ta->nodeType(this, myExp1Type);
		return;
	}
	ta->errLogicOpd(this->line(), this->col());
	ta->nodeType(this, ErrorType::produce());
}

void NotNode::typeAnalysis(TypeAnalysis * ta){
	myExp->typeAnalysis(ta);

	const DataType * myExpType = ta->nodeType(myExp);

	if (myExpType->getString() == "bool"){
		ta->nodeType(this, myExpType);
		return;
	}
	ta->errLogicOpd(this->line(), this->col());
	ta->nodeType(this, ErrorType::produce());
}

void NegNode::typeAnalysis(TypeAnalysis * ta){
	myExp->typeAnalysis(ta);

	const DataType * myExpType = ta->nodeType(myExp);

	if (myExpType->getString() == "int"){
		ta->nodeType(this, myExpType);
		return;
	}
	ta->errMathOpd(this->line(), this->col());
	ta->nodeType(this, ErrorType::produce());
}

void DeclNode::typeAnalysis(TypeAnalysis * ta){
	TODO("Override me in the subclass");
}

void PostDecStmtNode::typeAnalysis(TypeAnalysis * ta){
	myLVal->typeAnalysis(ta);
	const DataType * myLValType = ta->nodeType(myLVal);

	if (myLValType->getString() == "int")
	{
		ta->nodeType(this, myLValType);
		return;
	}
	ta->errMathOpd(this->line(), this->col());
	ta->nodeType(this, ErrorType::produce());

}

void PostIncStmtNode::typeAnalysis(TypeAnalysis * ta){
	myLVal->typeAnalysis(ta);
	const DataType * myLValType = ta->nodeType(myLVal);

	if (myLValType->getString() == "int")
	{
		ta->nodeType(this, myLValType);
		return;
	}
	ta->errMathOpd(this->line(), this->col());
	ta->nodeType(this, ErrorType::produce());

}

void VarDeclNode::typeAnalysis(TypeAnalysis * ta){
	// VarDecls always pass type analysis, since they
	// are never used in an expression. You may choose
	// to type them void (like this), as discussed in class
	ta->nodeType(this, BasicType::produce(VOID));
}

void IDNode::typeAnalysis(TypeAnalysis * ta){
	// IDs never fail type analysis and always
	// yield the type of their symbol (which
	// depends on their definition)
	ta->nodeType(this, this->getSymbol()->getDataType());
}

void IntLitNode::typeAnalysis(TypeAnalysis * ta){
	// IntLits never fail their type analysis and always
	// yield the type INT
	ta->nodeType(this, BasicType::produce(INT));
}

void StrLitNode::typeAnalysis(TypeAnalysis * ta){
	ta->nodeType(this, ArrayType::produce(BasicType::produce(BYTE),0));
}

void TrueNode::typeAnalysis(TypeAnalysis * ta){
	// TrueNodes never fail their type analysis and always
	// yield the type BOOl
	ta->nodeType(this, BasicType::produce(BOOL));
}

void FalseNode::typeAnalysis(TypeAnalysis * ta){
	// FalseNodes never fail their type analysis and always
	// yield the type BOOl
	ta->nodeType(this, BasicType::produce(BOOL));
}

void HavocNode::typeAnalysis(TypeAnalysis * ta){
	// HavocNodes never fail their type analysis and always
	// yield the type BOOl
	ta->nodeType(this, BasicType::produce(BOOL));
}

// void LValNode::typeAnalysis(TypeAnalysis * ta){
//
// }

}