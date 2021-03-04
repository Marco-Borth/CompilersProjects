#include "ast.hpp"

namespace crona{

	/*
	doIndent is declared static, which means that it can
	only be called in this file (its symbol is not exported).
	*/
	static void doIndent(std::ostream& out, int indent){
		for (int k = 0 ; k < indent; k++){ out << "\t"; }
	}

	/*
	In this code, the intention is that functions are grouped
	into files by purpose, rather than by class.
	If you're used to having all of the functions of a class
	defined in the same file, this style may be a bit disorienting,
	though it is legal. Thus, we can have
	ProgramNode::unparse, which is the unparse method of ProgramNodes
	defined in the same file as DeclNode::unparse, the unparse method
	of DeclNodes.
	*/


	void ProgramNode::unparse(std::ostream& out, int indent){
		/* Oh, hey it's a for-each loop in C++!
		   The loop iterates over each element in a collection
		   without that gross i++ nonsense.
		 */
		for (auto global : *myGlobals){
			/* The auto keyword tells the compiler
			   to (try to) figure out what the
			   type of a variable should be from
			   context. here, since we're iterating
			   over a list of DeclNode *s, it's
			   pretty clear that global is of
			   type DeclNode *.
			*/
			global->unparse(out, indent);
		}
	}

	void VarDeclNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << " : ";
		this->myType->unparse(out, 0);
		out << ";\n";
	}

	void FnDeclNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << " : ";
		this->myType->unparse(out, 0);
		out << " (";
		this->myFormals->unparse(out, 0);
		out << ") {";
		this->myStmts->unparse(out, 0);
		out << "}\n";
	}

	void AssignStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << " = ";
		this->myExp->unparse(out, 0);
		out << "; ";
	}

	void PostIncStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << "++; ";
	}

	void PostDecStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << "--; ";
	}

	void ReadStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		out << "read ";
		this->myId->unparse(out, 0);
		out << "; ";
	}

	void WriteStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		out << "write ";
		this->myExp->unparse(out, 0);
		out << "; ";
	}

	void IfStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		out << "if (";
		this->myExp->unparse(out, 0);
		out << ") {";
		this->myStmt->unparse(out, 0);
		out << "} ";
	}

	void IfElseStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		out << "if (";
		this->myExp->unparse(out, 0);
		out << ") {";
		this->myIfStmt->unparse(out, 0);
		out << "} else {";
		this->myElseStmt->unparse(out, 0);
		out << "} ";
	}

	void WhileStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		out << "while (";
		this->myExp->unparse(out, 0);
		out << ") {";
		this->myStmt->unparse(out, 0);
		out << "} ";
	}

	void ReturnStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		out << "return ";
		this->myExp->unparse(out, 0);
		out << "; ";
	}

	void CallStmtNode::unparse(std::ostream& out, int indent){
		doIndent(out, indent);
		this->myId->unparse(out, 0);
		out << " (";
		this->myExp->unparse(out, 0);
		out << "); ";
	}

	void IDNode::unparse(std::ostream& out, int indent){
		out << this->myStrVal;
	}

	void IntTypeNode::unparse(std::ostream& out, int indent){
		out << "int";
	}

	void BoolTypeNode::unparse(std::ostream& out, int indent){
		out << "bool";
	}

	void ByteTypeNode::unparse(std::ostream& out, int indent){
		out << "byte";
	}

	void StringTypeNode::unparse(std::ostream& out, int indent){
		out << "string";
	}

	void VoidTypeNode::unparse(std::ostream& out, int indent){
		out << "void";
	}

} // End namespace crona
