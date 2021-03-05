#ifndef CRONAC_AST_HPP
#define CRONAC_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"

// **********************************************************************
// ASTnode class (base class for all other kinds of nodes)
// **********************************************************************

namespace crona{

/* You may find it useful to forward declare AST subclasses
   here so that you can use a class before it's full definition
*/
class DeclListNode;
class DeclNode;
class StmtNode;
class StmtListNode;
class TypeNode;
class IDNode;

class ASTNode{
public:
	ASTNode(size_t lineIn, size_t colIn)
	: l(lineIn), c(colIn){
	}
	virtual void unparse(std::ostream& out, int indent) = 0;
	size_t line(){ return l; }
	size_t col() { return c; }

	/**
	* Return a string specifying the position this node begins.
	* For some node a position doesn't really make sense (i.e.
	* ProgramNode) but for the rest it's the position in the
	* input file that represents that node
	**/
	std::string pos(){
		return "[" + std::to_string(line()) + ","
		+ std::to_string(col()) + "]";
	}

private:
	size_t l; /// The line at which the node starts in the input file
	size_t c; /// The column at which the node starts in the input file
};

/**
* \class ProgramNode
* Class that contains the entire abstract syntax tree for a program.
* Note the list of declarations encompasses all global declarations
* which includes (obviously) all global variables and struct declarations
* and (perhaps less obviously), all function declarations
**/
class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn)
	: ASTNode(1, 1), myGlobals(globalsIn){
	}
	void unparse(std::ostream& out, int indent) override;
private:
	std::list<DeclNode * > * myGlobals;
};

/*
class StmtListNode : public ASTNode  {
	public:
		StmtListNode(size_t lineIn, size_t colIn, StmtNode * stmt, std::list<StmtNode*> * stmtList)
		: StmtNode(lineIn, colIn), myStmt(stmt), myStmtList(stmtList) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		StmtNode * myStmt;
		std::list<StmtNode*> * myStmtList;
};
*/

class StmtNode : public ASTNode {
protected:
	StmtNode(size_t lineIn, size_t colIn)
	: ASTNode(lineIn, colIn){
	}
public:
	void unparse(std::ostream& out, int indent) override = 0;
	//TODO: consider adding an isRef to use in unparse to
	// indicate if this is a reference type
};

/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to
* declare a struct, function, variable, etc).  This base class will
**/
class DeclNode : public StmtNode{
public:
	DeclNode(size_t line, size_t col)
	: StmtNode(line, col) {
	}
	void unparse(std::ostream& out, int indent) override = 0;
};

/**  \class ExpNode
* Superclass for expression nodes (i.e. nodes that can be used as
* part of an expression).  Nodes that are part of an expression
* should inherit from this abstract superclass.
**/
class ExpNode : public ASTNode{
protected:
	ExpNode(size_t line, size_t col)
	: ASTNode(line, col){
	}
public:
	void unparse(std::ostream& out, int indent) override = 0;
};

/**  \class TypeNode
* Superclass of nodes that indicate a data type. For example, in
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
	TypeNode(size_t lineIn, size_t colIn)
	: ASTNode(lineIn, colIn){
	}
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
	//TODO: consider adding an isRef to use in unparse to
	// indicate if this is a reference type
};

/** An identifier. Note that IDNodes subclass
 * ExpNode because they can be used as part of an expression.
**/
class IDNode : public ExpNode{
public:
	IDNode(IDToken * token)
	: ExpNode(token->line(), token->col()), myStrVal(token->value()){
		myStrVal = token->value();
	}
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	std::string myStrVal;
};

class IntLitNode : public ExpNode{
public:
	IntLitNode(IntLitToken * token)
	: ExpNode(token->line(), token->col()), myIntVal(token->num()){
		myIntVal = token->num();
	}
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	int myIntVal;
};

class StrLitNode : public ExpNode{
public:
	StrLitNode(StrToken * token)
	: ExpNode(token->line(), token->col()), myStrVal(token->str()){
		myStrVal = token->str();
	}
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	std::string myStrVal;
};

/** A variable declaration. Note that this class is intended to
 * represent a global or local variable of any type (including a struct
 * type. Note that this is not intended to represent a declaration of
 * a struct. In other words:
 * struct MyStruct {
 *   int fieldA;
 * };
 * is NOT a VarDeclNode because it introduces a new datatype, not a new
 * variable (in this case, the example is a StructDeclNode).  * However,
 * struct MyStruct instance; *is* a VarDeclNode, since it introduces a
 * new variable to the program.
**/
class VarDeclNode : public DeclNode{
public:
	VarDeclNode(size_t l, size_t c, TypeNode * type, IDNode * id)
	: DeclNode(type->line(), type->col()), myType(type), myId(id){
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IDNode * myId;
};

class FormalDeclNode : public VarDeclNode {
protected:
	FormalDeclNode(size_t lineIn, size_t colIn, TypeNode * type, IDNode * id)
	: VarDeclNode(lineIn, colIn, type, id) {
	}
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
	//TODO: consider adding an isRef to use in unparse to
	// indicate if this is a reference type
};

// Stmt List needed with James Fix.
class FnDeclNode : public DeclNode{
public:
	FnDeclNode(size_t l, size_t c, TypeNode * type, IDNode * id, FormalDeclNode * formalList, StmtNode * stmtList)
	: DeclNode(type->line(), type->col()), myType(type), myId(id), myFormalList(formalList), myStmtList(stmtList) {
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IDNode * myId;
	FormalDeclNode * myFormalList;
	StmtNode * myStmtList;
};

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(std::ostream& out, int indent);
};

class BoolTypeNode : public TypeNode{
public:
	BoolTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(std::ostream& out, int indent);
};

class ByteTypeNode : public TypeNode{
public:
	ByteTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(std::ostream& out, int indent);
};

class StringTypeNode : public TypeNode{
public:
	StringTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(std::ostream& out, int indent);
};

class VoidTypeNode : public TypeNode{
public:
	VoidTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(std::ostream& out, int indent);
};

class ArrayTypeNode : public TypeNode{
public:
	ArrayTypeNode(size_t lineIn, size_t colIn, TypeNode * type, IntLitNode * intVal)
	: TypeNode(lineIn, colIn), myType(type), myIntVal(intVal){
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IntLitNode * myIntVal;
};

// lvalNode needs to replace IdNode.
class AssignExpNode : public ExpNode {
	public:
		AssignExpNode(size_t lineIn, size_t colIn, IDNode * id, ExpNode * exp)
		: ExpNode(lineIn, colIn), myId(id), myExp(exp) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		IDNode * myId;
		ExpNode * myExp;
};

// lvalNode needs to replace IdNode.
class AssignStmtNode : public StmtNode {
	public:
		AssignStmtNode(size_t lineIn, size_t colIn, IDNode * id, ExpNode * exp)
		: StmtNode(lineIn, colIn), myId(id), myExp(exp) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		IDNode * myId;
		ExpNode * myExp;
};

// lvalNode needs to replace IdNode.
class PostIncStmtNode : public StmtNode {
	public:
		PostIncStmtNode(size_t lineIn, size_t colIn, IDNode * id)
		: StmtNode(lineIn, colIn), myId(id) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		IDNode * myId;
};

// lvalNode needs to replace IdNode.
class PostDecStmtNode : public StmtNode {
	public:
		PostDecStmtNode(size_t lineIn, size_t colIn, IDNode * id)
		: StmtNode(lineIn, colIn), myId(id) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		IDNode * myId;
};

// lvalNode needs to replace IdNode.
class ReadStmtNode : public StmtNode {
	public:
		ReadStmtNode(size_t lineIn, size_t colIn, IDNode * id)
		: StmtNode(lineIn, colIn), myId(id) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		IDNode * myId;
};

class WriteStmtNode : public StmtNode {
	public:
		WriteStmtNode(size_t lineIn, size_t colIn, ExpNode * exp)
		: StmtNode(lineIn, colIn), myExp(exp) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		ExpNode * myExp;
};

// Stmt List needed with James Fix.
//std::list<StmtNode*> * stmtList
class IfStmtNode : public StmtNode {
	public:
		IfStmtNode(size_t lineIn, size_t colIn, ExpNode * exp, StmtNode * stmtList)
		: StmtNode(lineIn, colIn), myExp(exp), myStmtList(stmtList) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		ExpNode * myExp;
		StmtNode * myStmtList;
};

// Stmt List needed with James Fix.
//std::list<StmtNode*> * stmtList
class IfElseStmtNode : public StmtNode {
	public:
		IfElseStmtNode(size_t lineIn, size_t colIn, ExpNode * exp, StmtNode * ifStmtList, StmtNode * elseStmtList)
		: StmtNode(lineIn, colIn), myExp(exp), myIfStmtList(ifStmtList), myElseStmtList(elseStmtList) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		ExpNode * myExp;
		StmtNode * myIfStmtList;
		StmtNode * myElseStmtList;
};

// Stmt List needed with James Fix.
class WhileStmtNode : public StmtNode {
	public:
		WhileStmtNode(size_t lineIn, size_t colIn, ExpNode * exp, StmtNode * stmtList)
		: StmtNode(lineIn, colIn), myExp(exp), myStmtList(stmtList) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		ExpNode * myExp;
		StmtNode * myStmtList;
};

class ReturnStmtNode : public StmtNode {
	public:
		ReturnStmtNode(size_t lineIn, size_t colIn, ExpNode * exp)
		: StmtNode(lineIn, colIn), myExp(exp) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		ExpNode * myExp;
};

// Possible ActualsListNode needed.
class CallStmtNode : public StmtNode {
	public:
		CallStmtNode(size_t lineIn, size_t colIn, IDNode * id, ExpNode * exp)
		: StmtNode(lineIn, colIn), myId(id), myExp(exp) {
		}
		void unparse(std::ostream& out, int indent);
	private:
		IDNode * myId;
		ExpNode * myExp;
};

} //End namespace crona

#endif
