#include "ast.hpp"

using namespace std;

namespace crona{

	/*
	doIndent is declared static, which means that it can
	only be called in this file (its symbol is not exported).
	*/
	static void doIndent(ostream& out, int indent){
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

	void ProgramNode::unparse(ostream& out, int indent){
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

	void VarDeclNode::unparse(ostream& out, int indent){
		this->myId->unparse(out, 0);
		out << " : ";
		this->myType->unparse(out, 0);
		out << ";\n";
	}

	void FnDeclNode::unparse(ostream& out, int indent){
		out << "\n";
		this->myId->unparse(out, indent);
		out << " : ";
		this->myType->unparse(out, indent);
		out << " ( ";

		if (myFormalList != nullptr) {
			for (auto formal : *myFormalList){
				formal->unparse(out, indent);
				out << ", ";
			}
		}
		out << ") {";
		if (myStmtList != nullptr) {
			out << "\n";
			indent++;
			for (auto stmt : *myStmtList){
				doIndent(out, indent);
				stmt->unparse(out, indent);
				out << "\n";
			}
		}
		out << "}\n";
	}

	void FormalDeclNode::unparse(ostream& out, int indent){
		this->myId->unparse(out, indent);
		out<<" : ";
		this->myType->unparse(out, indent);
	}

	// lvalNode needs to replace IdNode.
	void AssignExpNode::unparse(ostream& out, int indent){
		this->myId->unparse(out, 0);
		out << " = ";
		this->myExp->unparse(out, 0);
		out << "; ";
	}

	// lvalNode needs to replace IdNode.
	void AssignStmtNode::unparse(ostream& out, int indent){
		this->myId->unparse(out, 0);
		out << " = ";
		this->myExp->unparse(out, 0);
		out << "; ";
	}

	void PostIncStmtNode::unparse(ostream& out, int indent){
		this->myLVal->unparse(out, 0);
		out << "++; ";
	}

	void PostDecStmtNode::unparse(ostream& out, int indent){
		this->myLVal->unparse(out, 0);
		out << "--; ";
	}

	void ReadStmtNode::unparse(ostream& out, int indent){
		out << "read ";
		this->myLVal->unparse(out, 0);
		out << "; ";
	}

	void WriteStmtNode::unparse(ostream& out, int indent){
		out << "write ";
		this->myExp->unparse(out, 0);
		out << "; ";
	}

	void IfStmtNode::unparse(ostream& out, int indent){
		out << "if ( ";
		this->myExp->unparse(out, 0);
		out << " ) {";
		if (myStmtList != nullptr) {
			out << "\n";
			indent++;
			for (auto stmt : *myStmtList){
				doIndent(out, indent);
				stmt->unparse(out, indent);
				out << "\n";
			}
			indent--;
		}
		doIndent(out, indent);
		out << "} ";
	}

	void IfElseStmtNode::unparse(ostream& out, int indent){
		out << "if ( ";
		this->myExp->unparse(out, 0);
		out << " ) {";
		if (myIfStmtList != nullptr) {
			out << "\n";
			indent++;
			for (auto stmt : *myIfStmtList){
				doIndent(out, indent);
				stmt->unparse(out, indent);
				out << "\n";
			}
			indent--;
		}
		doIndent(out, indent);
		out << "} else {";
		if (myElseStmtList != nullptr) {
			out << "\n";
			indent++;
			for (auto stmt : *myElseStmtList){
				doIndent(out, indent);
				stmt->unparse(out, indent);
				out << "\n";
			}
			indent--;
		}
		doIndent(out, indent);
		out << "} ";
	}

	void WhileStmtNode::unparse(ostream& out, int indent){
		out << "while ( ";
		this->myExp->unparse(out, 0);
		out << " ) {";
		if (myStmtList != nullptr) {
			out << "\n";
			indent++;
			for (auto stmt : *myStmtList){
				doIndent(out, indent);
				stmt->unparse(out, indent);
				out << "\n";
			}
			indent--;
		}
		doIndent(out, indent);
		out << "}";
	}

	void ReturnStmtNode::unparse(ostream& out, int indent){
		out << "return";
		if (this->myExp != nullptr) {
			out << " ";
			this->myExp->unparse(out, 0);
		}
		out << "; ";
	}

	void CallStmtNode::unparse(ostream& out, int indent){
		this->myId->unparse(out, 0);
		out << " (";
		this->myExp->unparse(out, 0);
		out << "); ";
	}
	void IndexNode::unparse(ostream& out, int indent){
		m_id_node->unparse(out, indent);
		out <<'[';
		m_exp_node->unparse(out, indent);
		out <<']';
	}
	void IDNode::unparse(ostream& out, int indent){
		out << this->myStrVal;
	}

	void HavocNode::unparse(ostream& out, int indent){
		out << "havoc";
	}

	void FalseNode::unparse(ostream& out, int indent){
		out << "false";
	}

	void TrueNode::unparse(ostream& out, int indent){
		out << "true";
	}

	void IntTypeNode::unparse(ostream& out, int indent){
		out << "int";
	}

	void BoolTypeNode::unparse(ostream& out, int indent){
		out << "bool";
	}

	void ByteTypeNode::unparse(ostream& out, int indent){
		out << "byte";
	}

	void StringTypeNode::unparse(ostream& out, int indent){
		out << "string";
	}

	void VoidTypeNode::unparse(ostream& out, int indent){
		out << "void";
	}

	void ArrayTypeNode::unparse(ostream& out, int indent){
		this->myType->unparse(out, 0);
		out << " array[";
		this->myIntVal->unparse(out, 0);
		out << "]";
	}

	void IntLitNode::unparse(ostream& out, int indent){
		out << this->myIntVal;
	}

	void StrLitNode::unparse(ostream& out, int indent){
		out << this->myStrVal;
	}

} // End namespace crona
