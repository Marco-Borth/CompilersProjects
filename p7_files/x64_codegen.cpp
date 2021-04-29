#include <ostream>
#include "3ac.hpp"

namespace crona{

void IRProgram::allocGlobals(){
	int gbl_iter = 0;
	int str_iter = 0;
	for (auto gbl : globals)
	{
		std::string lbl;
		if (gbl.second->getWidth() != 0)
		{
			lbl = "gbl_"+std::to_string(gbl_iter);
			gbl_iter++;
		}
		else
		{
			lbl = "str_"+std::to_string(str_iter);
			str_iter++;
		}
		gbl.second->setMemoryLoc(lbl);
	}
	for (auto str : strings)
	{
		std::string lbl ="\0";
		lbl = "str_"+std::to_string(str_iter);
		str_iter++;
		str.first->setMemoryLoc(lbl);
	}

}

void IRProgram::datagenX64(std::ostream& out){
	//Empty IRProgram globals map.
	//Each iteration we will retrieve a map pair of <SemSymbol *, SymOpd *>.
	for (auto gbl : globals)
	{
		out<<gbl.second->getMemoryLoc()<<": ";
		size_t gbl_width = gbl.second->getWidth();
		if (gbl_width == 0)
		{
			out<<".asciz \"\"\n";
			out<<".align 8\n";
		}
		else if (gbl_width == 1)
		{
			out<<".byte 0\n";
		}
		else if (gbl_width == 8)
		{
			out<<".quad 0\n";
		}
		else
		{
			throw new InternalError("Bad variable size");
		}
	}
	for (auto str : strings)
	{
		out<<str.first->getMemoryLoc()<<": ";
		out<<".asciz "<<str.second<<"\n";
		out << ".align 8\n";
	}
	//Create the label using myName() funct of the SymOpd.
	//Choose .quad if an int, .byte if a byte, and .asciz for strings.
	//Global strings will just be var decls and should print like "   	my_string    : .asciz ""    ", with an empty string.
	//Empty IRProgram strings hash map.
	//Each iteration we will retrieve a hash map pair of <AddrOpd *, std::string>.
	//Create the label using getName() funct of AddrOpd.
	//Print the body.

	//Put this directive after you write out strings
	// so that everything is aligned to a quadword value
	// again
	//out << ".align 8\n";

}

void IRProgram::toX64(std::ostream& out){
	//Allocate space for globals
	out<<".globl main\n";
	out<<".data\n";
	allocGlobals();
	datagenX64(out);
	out<<".text\n";
	for (auto proc : *procs)
	{
		//out<<proc->getName()<<":"	;
		proc->toX64(out);
	}
	// Iterate over each procedure and codegen it
	//TODO(Implement me)
}

void Procedure::allocLocals(){
	size_t rbp_offset = 16;
	for (auto local : locals)
	{
		size_t len = local.second->getWidth();
		rbp_offset = rbp_offset + len;
		std::string loc;
		loc = "-"+std::to_string(rbp_offset)+"(%rbp)"; // "-16(%rpb)"
		local.second->setMemoryLoc(loc);
	}
}

void Procedure::toX64(std::ostream& out){
	//Allocate all locals
	allocLocals();

	enter->codegenLabels(out);
	enter->codegenX64(out);
	out << "#Fn body " << myName << "\n";
	for (auto quad : *bodyQuads){
		quad->codegenLabels(out);
		out << "#" << quad->toString() << "\n";
		quad->codegenX64(out);
	}
	out << "#Fn epilogue " << myName << "\n";
	leave->codegenLabels(out);
	leave->codegenX64(out);
}

void Quad::codegenLabels(std::ostream& out){
	if (labels.empty()){ return; }

	size_t numLabels = labels.size();
	size_t labelIdx = 0;
	for ( Label * label : labels){
		out << label->toString() << ": ";
		if (labelIdx != numLabels - 1){ out << "\n"; }
		labelIdx++;
	}
}

void BinOpQuad::codegenX64(std::ostream& out){
	TODO(Implement me)
	/*
	dst->genLoadVal(out, A);
	opr->genLoadVal(out, A);
	src1->genLoadVal(out, A);
	src2->genLoadVal(out, A);
	*/
}

void UnaryOpQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	dst->genLoadVal(out, A);
	//op->genLoadVal(out, A);
	src->genLoadVal(out, A);
}

void AssignQuad::codegenX64(std::ostream& out){
	src->genLoadVal(out, A);
	dst->genStoreVal(out, A);
}

void JmpQuad::codegenX64(std::ostream& out){
	out << "jmp " /*<< tgt->toString()*/ << "\n";
}

void JmpIfQuad::codegenX64(std::ostream& out){
	TODO(Implement me)
	cnd->genLoadVal(out, A);
	out << "jmp if" << cnd->getMemoryLoc() << tgt->toString() << "\n";
}

void NopQuad::codegenX64(std::ostream& out){
	out << "nop" << "\n";
}

void HavocQuad::codegenX64(std::ostream& out){
	TODO(Randomly set rax to 1 or 0)
	//myDst->genLoadVal(out, A);
}

void IntrinsicOutputQuad::codegenX64(std::ostream& out){
	if (myType->isBool()){
		myArg->genLoadVal(out, DI);
		out << "callq printBool\n";
	}
	else if (myType->isByte())
	{
		myArg->genLoadVal(out,DI);
		out<<"callq printChar\n";
	}
	else //Int
	{
		myArg->genLoadVal(out,DI);
		out<<"callq printInt\n";
	}
}

void IntrinsicInputQuad::codegenX64(std::ostream& out){
	if (myType->isBool()){
		out << "callq getBool\n";
		myArg->genStoreVal(out, A);
	}
	else if (myType->isByte())
	{
		out<<"callq getChar\n";
		myArg->genStoreVal(out, A);
	}
	else //Int
	{
		out<<"callq getInt\n";
		myArg->genStoreVal(out, A);
	}
}

void CallQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
	/*
	const FnType * calleeType = callee->getDataType()->asFn();
	const std::list<const DataType *> * argsList = calleeType->getFormalTypes();
	int argIndex = 1;
	for (auto arg : *argsList) {
		out<< "movq $" << ", %rbp\n";
	}
	*/
	out << "callq fun_" << callee->getName() << endl;
}

void EnterQuad::codegenX64(std::ostream& out){
	//IP is already saved in the first 8 bytes after the caller's AR (callq also moved the rsp to infront of the saved IP)
	//Need to store caller's RBP before we update RBP to the front of our new RBP
	out<<"pushq %rbp\n"; //Caller's RBP is pushed infront of the stored IP (rsp is also moved to the front of the saved rbp (not where we want it)
	out<<"movq %rsp , %rbp\n"; //Move rbp to point at the same location as rsp
	out<<"addq $16, %rbp\n"; //RBP fixed to new location before the bookkeepers.
	size_t ar_size = myProc->arSize();
	out<<"subq $"<<ar_size<<", %rsp\n";
}

void LeaveQuad::codegenX64(std::ostream& out){
	//epilogue
	size_t ar_size = myProc->arSize();
	out<<"addq $"<<ar_size<<", %rsp\n";//Move rsp back to just before the books.
	out<<"popq %rbp\n";//Set rbp back to the caller's rbp (the second bookkeeping value), increment %rsp by 8.
	out<<"retq\n";//Pop the old rip, increment %rsp by 8 and return to the addr stored in old rip.
}

void SetArgQuad::codegenX64(std::ostream& out){
	//TODO(Implement me)
}

void GetArgQuad::codegenX64(std::ostream& out){
	//We don't actually need to do anything here
}

void SetRetQuad::codegenX64(std::ostream& out){
	TODO(Implement me)
}

void GetRetQuad::codegenX64(std::ostream& out){
	TODO(Implement me)
}

void IndexQuad::codegenX64(std::ostream& out){
	TODO(Implement me)
}

void SymOpd::genLoadVal(std::ostream& out, Register reg){
	//TODO(Implement me)
	out << getMovOp()<<' '<<getMemoryLoc() << ", " << getReg(reg) << "\n";
}

void SymOpd::genStoreVal(std::ostream& out, Register reg){
	//TODO(Implement me)
	out<< getMovOp()<<' '<<getReg(reg)<<" , "<<getMemoryLoc()<<'\n';
}

void SymOpd::genLoadAddr(std::ostream& out, Register reg) {
	TODO(Implement me if necessary)
}

void AuxOpd::genLoadVal(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AuxOpd::genStoreVal(std::ostream& out, Register reg){
	//TODO(Implement me)
}

void AuxOpd::genLoadAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}


void AddrOpd::genStoreVal(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genLoadVal(std::ostream& out, Register reg){
	//TODO(Implement me)
	//out << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

void AddrOpd::genStoreAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genLoadAddr(std::ostream & out, Register reg){
	TODO(Implement me)
}

void LitOpd::genLoadVal(std::ostream & out, Register reg){
	out << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

}
