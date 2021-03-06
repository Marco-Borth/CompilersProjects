#ifndef CRONAC_AST_HPP
#define CRONAC_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"

// **********************************************************************
// ASTnode class (base class for all other kinds of nodes)
// **********************************************************************

using namespace std;

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
	void unparse(std::ostream& out, int indent) override = 0; //Abstract unparse function.
};

class LValNode : public ExpNode
{
public:
	LValNode (size_t line, size_t col) /*TODO */
	: ExpNode(line, col){
	}
};

class HavocNode : public ExpNode
{
public:
	HavocNode (size_t line, size_t col)
	: ExpNode(line, col){
	}
	void unparse(std::ostream& out, int indent);
};

class FalseNode : public ExpNode
{
public:
	FalseNode (size_t line, size_t col)
	: ExpNode(line,col){
	}
	void unparse(std::ostream& out, int indent);
};

class TrueNode : public ExpNode
{
public:
	TrueNode (size_t line, size_t col)
	: ExpNode(line,col){
	}
	void unparse(std::ostream& out, int indent);
};

/** An identifier. Note that IDNodes subclass
 * ExpNode because they can be used as part of an expression.
**/
class IDNode : public LValNode{
public:
	IDNode(IDToken * token)
	: LValNode(token->line(), token->col()), myStrVal(token->value()){
		//myStrVal = token->value();
	}
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	std::string myStrVal;
};

class IndexNode : public LValNode{
public:
	IndexNode(IDNode* i_id_node, ExpNode* i_exp_node)
	: LValNode(i_id_node->line(),i_id_node->col()){
		m_id_node = i_id_node;
		m_exp_node = i_exp_node;
	}
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	IDNode* m_id_node;
	ExpNode* m_exp_node;
};

class BinaryExpNode : public ExpNode
{ //Abstract class for Binary Expression Subclasses
public:
	BinaryExpNode(size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode, std::string i_op)
	: ExpNode(line,col){
		m_l_expNode = i_l_expNode;
		m_r_expNode = i_r_expNode;
		m_op = i_op;
	}
	//void unparse(std::ostream& out, int indent) override = 0; //Abstract unparse function.
	void unparse(std::ostream& out, int indent);
protected:
	ExpNode* m_l_expNode;
	ExpNode* m_r_expNode;
	std::string m_op;
};

//BEGIN Binary Expression Subclasses
class AndExpNode : public BinaryExpNode
{
public:
	AndExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " && "){
	}
};

class OrExpNode : public BinaryExpNode
{
public:
	OrExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " || "){
	}
};

class MinusExpNode : public BinaryExpNode
{
public:
	MinusExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " - "){
	}
};

class PlusExpNode : public BinaryExpNode
{
public:
	PlusExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " + "){
	}
};

class MultExpNode : public BinaryExpNode
{
public:
	MultExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " * "){
	}
};

class DivExpNode : public BinaryExpNode
{
public:
	DivExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " / "){
	}
};

class EqualsExpNode : public BinaryExpNode
{
public:
	EqualsExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " == "){
	}
};

class NotEqualsExpNode : public BinaryExpNode
{
public:
	NotEqualsExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " != "){
	}
};

class GreaterExpNode : public BinaryExpNode
{
public:
	GreaterExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " > "){
	}
};

class GreaterEqExpNode : public BinaryExpNode
{
public:
	GreaterEqExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " >= "){
	}
};

class LessExpNode : public BinaryExpNode
{
public:
	LessExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " < "){
	}
};

class LessEqExpNode : public BinaryExpNode
{
public:
	LessEqExpNode (size_t line, size_t col, ExpNode* i_l_expNode, ExpNode* i_r_expNode)
	: BinaryExpNode(line, col, i_l_expNode, i_r_expNode, " <= "){
	}
};
//END Binary Expression Subclasses

class UnaryExpNode : public ExpNode
{
public:
	UnaryExpNode (size_t line, size_t col, ExpNode* i_expNode)
	: ExpNode(line,col){
		m_expNode = i_expNode;
	}
	void unparse(std::ostream& out, int indent) override = 0; //Abstract Unparse Function
protected:
	ExpNode* m_expNode;
};

//BEGIN Unary Expression Subclasses.

class NegNode : public UnaryExpNode
{
public:
	NegNode (size_t line, size_t col, ExpNode* i_expNode)
	: UnaryExpNode ( line, col, i_expNode){
	}
	void unparse(std::ostream& out, int indent);
};

class NotNode : public UnaryExpNode
{
public:
	NotNode (size_t line, size_t col, ExpNode* i_expNode)
	: UnaryExpNode ( line, col, i_expNode){
	}
	void unparse(std::ostream& out, int indent);
};
//END Unary Expression Subclasses.

class CallExpNode : public ExpNode
{
public:
	CallExpNode (size_t line, size_t col, IDNode* i_id_node ,std::list<ExpNode*>* i_exp_list)
	: ExpNode (line, col){
		m_id_node = i_id_node;
		m_exp_list = i_exp_list;
	}
	void unparse(std::ostream& out, int indent);
private:
	IDNode* m_id_node;
	std::list<ExpNode*>* m_exp_list;
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
	virtual void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IDNode * myId;
};

class FormalDeclNode : public VarDeclNode {
public:
	FormalDeclNode(size_t l, size_t c, TypeNode * type, IDNode * id)
	: VarDeclNode(type->line(), type->col(), type, id){
		myType = type;
		myId	 = id;
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IDNode * myId;
};

class FnDeclNode : public DeclNode{
public:
	FnDeclNode(size_t l, size_t c, TypeNode * type, IDNode * id, list<FormalDeclNode*> * formalList, list<StmtNode*> * stmtList)
	: DeclNode(type->line(), type->col()), myType(type), myId(id), myFormalList(formalList), myStmtList(stmtList) {
	}
	void unparse(ostream& out, int indent);
private:
	TypeNode * myType;
	IDNode * myId;
	list<FormalDeclNode*> * myFormalList;
	list<StmtNode*> * myStmtList;
};

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(ostream& out, int indent);
};

class BoolTypeNode : public TypeNode{
public:
	BoolTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(ostream& out, int indent);
};

class ByteTypeNode : public TypeNode{
public:
	ByteTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(ostream& out, int indent);
};

class StringTypeNode : public TypeNode{
public:
	StringTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(ostream& out, int indent);
};

class VoidTypeNode : public TypeNode{
public:
	VoidTypeNode(size_t lineIn, size_t colIn)
	: TypeNode(lineIn, colIn){
	}
	void unparse(ostream& out, int indent);
};

class ArrayTypeNode : public TypeNode{
public:
	ArrayTypeNode(size_t lineIn, size_t colIn, TypeNode * type, IntLitNode * intVal)
	: TypeNode(lineIn, colIn), myType(type), myIntVal(intVal){
	}
	void unparse(ostream& out, int indent);
private:
	TypeNode * myType;
	IntLitNode * myIntVal;
};

// lvalNode needs to replace IDNode.
class AssignExpNode : public ExpNode {
	public:
		AssignExpNode(size_t lineIn, size_t colIn, LValNode * i_lval, ExpNode * exp)
		: ExpNode(lineIn, colIn), m_lval(i_lval), myExp(exp) {
		}
		void unparse(ostream& out, int indent);
	private:
		LValNode * m_lval;
		ExpNode * myExp;
};

// lvalNode needs to replace IDNode.
class AssignStmtNode : public StmtNode {
	public:
		AssignStmtNode(AssignExpNode* i_expNode)
		: StmtNode(i_expNode->line(), i_expNode->col()) {
			m_expNode=i_expNode;
		}
		void unparse(ostream& out, int indent);
	private:
		AssignExpNode* m_expNode;
};

class PostIncStmtNode : public StmtNode {
	public:
		PostIncStmtNode(size_t lineIn, size_t colIn, LValNode * lVal)
		: StmtNode(lineIn, colIn), myLVal(lVal) {
		}
		void unparse(ostream& out, int indent);
	private:
		LValNode * myLVal;
};

class PostDecStmtNode : public StmtNode {
	public:
		PostDecStmtNode(size_t lineIn, size_t colIn, LValNode * lVal)
		: StmtNode(lineIn, colIn), myLVal(lVal) {
		}
		void unparse(ostream& out, int indent);
	private:
		LValNode * myLVal;
};

class ReadStmtNode : public StmtNode {
	public:
		ReadStmtNode(size_t lineIn, size_t colIn, LValNode * lVal)
		: StmtNode(lineIn, colIn), myLVal(lVal) {
		}
		void unparse(ostream& out, int indent);
	private:
		LValNode * myLVal;
};

class WriteStmtNode : public StmtNode {
	public:
		WriteStmtNode(size_t lineIn, size_t colIn, ExpNode * exp)
		: StmtNode(lineIn, colIn), myExp(exp) {
		}
		void unparse(ostream& out, int indent);
	private:
		ExpNode * myExp;
};

class IfStmtNode : public StmtNode {
	public:
		IfStmtNode(size_t lineIn, size_t colIn, ExpNode * exp, list<StmtNode*> * stmtList)
		: StmtNode(lineIn, colIn), myExp(exp), myStmtList(stmtList) {
		}
		void unparse(ostream& out, int indent);
	private:
		ExpNode * myExp;
		list<StmtNode*> * myStmtList;
};

class IfElseStmtNode : public StmtNode {
	public:
		IfElseStmtNode(size_t lineIn, size_t colIn, ExpNode * exp, list<StmtNode*> * ifStmtList, list<StmtNode*> * elseStmtList)
		: StmtNode(lineIn, colIn), myExp(exp), myIfStmtList(ifStmtList), myElseStmtList(elseStmtList) {
		}
		void unparse(ostream& out, int indent);
	private:
		ExpNode * myExp;
		list<StmtNode*> * myIfStmtList;
		list<StmtNode*> * myElseStmtList;
};

class WhileStmtNode : public StmtNode {
	public:
		WhileStmtNode(size_t lineIn, size_t colIn, ExpNode * exp, list<StmtNode*> * stmtList)
		: StmtNode(lineIn, colIn), myExp(exp), myStmtList(stmtList) {
		}
		void unparse(ostream& out, int indent);
	private:
		ExpNode * myExp;
		list<StmtNode*> * myStmtList;
};

class ReturnStmtNode : public StmtNode {
	public:
		ReturnStmtNode(size_t lineIn, size_t colIn, ExpNode * exp)
		: StmtNode(lineIn, colIn), myExp(exp) {
		}
		void unparse(ostream& out, int indent);
	private:
		ExpNode * myExp;
};

class CallStmtNode : public StmtNode {
	public:
		CallStmtNode(CallExpNode* i_call_exp_node)
		: StmtNode(i_call_exp_node->line(), i_call_exp_node->col()){
			m_call_exp_node = i_call_exp_node;
		}
		void unparse(ostream& out, int indent);
	private:
		CallExpNode* m_call_exp_node;
};

}
//End namespace crona

#endif
