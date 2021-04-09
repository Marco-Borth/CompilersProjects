#include "ast.hpp"

namespace crona{

IRProgram * ProgramNode::to3AC(TypeAnalysis * ta){
	IRProgram * prog = new IRProgram(ta);
	for (auto global : *myGlobals){
		global->to3AC(prog);
	}
	return prog;
}

void FnDeclNode::to3AC(IRProgram * prog){
	Procedure* newProc = prog->makeProc(myID->getName()); //Create a new procedure and push
	//it to the list of procedures.
	size_t iter =1;
	std::list<FormalDeclNode *> * formals_list = getFormals();
	for (std::list<FormalDeclNode *>::iterator it = formals_list->begin(); it!=formals_list->end(); ++it)
	{
		(*it)->to3AC(newProc);
		SymOpd* my_symOpd = newProc->getSymOpd((*it)->ID()->getSymbol());
		GetArgQuad* my_ArgQuad = new GetArgQuad(iter, my_symOpd);
		newProc->addQuad(my_ArgQuad);
		iter++;
	}
	for (std::list<StmtNode *>::iterator it = myBody->begin(); it!=myBody->end(); ++it)
	{
		(*it)->to3AC(newProc);
	}
}

void FnDeclNode::to3AC(Procedure * proc){
	//This never needs to be implemented,
	// the function only exists because of
	// inheritance needs (A function declaration
	// never occurs within another function)
	throw new InternalError("FnDecl at a local scope");
}

void FormalDeclNode::to3AC(IRProgram * prog){
	//This never needs to be implemented,
	// the function only exists because of
	// inheritance needs (A formal never
	// occurs at global scope)
	throw new InternalError("Formal at a global scope");
}

void FormalDeclNode::to3AC(Procedure * proc){
	proc->gatherFormal(ID()->getSymbol());
}

Opd * IntLitNode::flatten(Procedure * proc){
	const DataType * type = proc->getProg()->nodeType(this);
	if (type->isByte()){
		return new LitOpd(std::to_string(myNum), 1);
	} else {
		return new LitOpd(std::to_string(myNum), 8);
	}
}

Opd * StrLitNode::flatten(Procedure * proc){
	Opd * res = proc->getProg()->makeString(myStr);
	return res;
}

Opd * HavocNode::flatten(Procedure * proc){
	Opd * dst = proc->makeTmp(1);
	HavocQuad * havoc = new HavocQuad(dst);
	proc->addQuad(havoc);
	return dst;
}

Opd * TrueNode::flatten(Procedure * proc){
	//TODO(Implement me)
	//const DataType * type = proc->getProg()->nodeType(this);
	//Opd * dst = proc->makeTmp(1);
	LitOpd * lit = new LitOpd("1", 1);
	//AssignQuad * assign = new AssignQuad(dst, lit);
	//proc->addQuad(assign);
	return lit;
}

Opd * FalseNode::flatten(Procedure * proc){
	//TODO(Implement me)
	//const DataType * type = proc->getProg()->nodeType(this);
	//Opd * dst = proc->makeTmp(1);
	LitOpd * lit = new LitOpd("0", 1);
	//AssignQuad * assign = new AssignQuad(dst, lit);
	//proc->addQuad(assign);
	return lit;
}

Opd * AssignExpNode::flatten(Procedure * proc){
	AssignQuad * my_AssignQuad = new AssignQuad(myDst->flatten(proc), mySrc->flatten(proc)); //Returns the two operands of assign exp and inserts them into the quad.
	proc->addQuad(my_AssignQuad); //Add the quad to the list of stmts.
	return(myDst->flatten(proc)); //Returns either myDst or mySrc. I don't know which specific item to return.
}

Opd * LValNode::flatten(Procedure * proc){
	TODO(Implement me)
}

Opd * CallExpNode::flatten(Procedure * proc){
	//TODO(Implement me)
	//Opd* my_Opd = proc->getSymOpd(ID()->getSymbol());
	//SetArgQuad* my_ArgQuad = new SetArgQuad(1, my_Opd);
	//newProc->addQuad(my_ArgQuad);

	size_t iter =1;
	std::list<ExpNode *> * args_list = getArgs();
	for (std::list<ExpNode *>::iterator it = args_list->begin(); it!=args_list->end(); ++it)
	{
		Opd* my_Opd = (*it)->flatten(proc);
		SetArgQuad* my_ArgQuad = new SetArgQuad(iter, my_Opd);
		proc->addQuad(my_ArgQuad);
		iter++;
	}
	CallQuad * callee = new CallQuad(myID->getSymbol());
	proc->addQuad(callee);
	SymOpd* my_symOpd = proc->getSymOpd(myID->getSymbol());
	return my_symOpd;
}

Opd * ByteToIntNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myChild->flatten(proc);
	IntLitNode * convert = new IntLitNode(0,0, 8);
	Opd * opd2 = new LitOpd(std::to_string(convert->getNum()), 8);
	BinOp opr = FAIL64;
	dst = proc->makeTmp(8);
	opr = MULT64;
	if(opd1->getWidth() == 8) {
		BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
		proc->addQuad(myBinOp);
	} else {
		AssignQuad* assign = new AssignQuad (dst, opd1);
		proc->addQuad(assign);
	}
	return (dst);
}

Opd * NegNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd = myExp->flatten(proc);
	UnaryOp opr = NEG64;
	dst = proc->makeTmp(8);
	UnaryOpQuad* myUnaryOp = new UnaryOpQuad(dst, opr, opd);
	proc->addQuad(myUnaryOp);
	return (dst);
}

Opd * NotNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd = myExp->flatten(proc);
	UnaryOp opr = NOT8;
	dst = proc->makeTmp(1);
	UnaryOpQuad* myUnaryOp = new UnaryOpQuad(dst, opr, opd);
	proc->addQuad(myUnaryOp);
	return (dst);
}

Opd * PlusNode::flatten(Procedure * proc){
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = ADD8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = ADD64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * MinusNode::flatten(Procedure * proc){
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = SUB8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = SUB64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * TimesNode::flatten(Procedure * proc){
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = MULT8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = MULT64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * DivideNode::flatten(Procedure * proc){
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = DIV8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = DIV64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * AndNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = AND8;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * OrNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = OR8;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * EqualsNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = EQ8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = EQ64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = NEQ8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = NEQ64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * LessNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = LT8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = LT64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * GreaterNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = GT8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = GT64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * LessEqNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = LTE8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = LTE64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myExp1->flatten(proc);
	Opd * opd2 = myExp2->flatten(proc);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == opd2->getWidth())
	{
		if (opd1->getWidth() == 1)
		{
			dst = proc->makeTmp(1);
			opr = GTE8;
		}
		else
		{
			dst = proc->makeTmp(8);
			opr = GTE64;
		}
	}
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return (dst);
}

void AssignStmtNode::to3AC(Procedure * proc){
	myExp->flatten(proc);
}

void PostIncStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myLVal->flatten(proc);
	IntLitNode * convert = new IntLitNode(0,0, 1);
	Opd * opd2 = new LitOpd(std::to_string(convert->getNum()), 1);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == 1) {
		opr = ADD8;
	} else {
		opr = ADD64;
	}
	BinOpQuad* myBinOp = new BinOpQuad (opd1, opr, opd1, opd2);
	proc->addQuad(myBinOp);
}

void PostDecStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
	Opd * dst = nullptr;
	Opd * opd1 = myLVal->flatten(proc);
	IntLitNode * convert = new IntLitNode(0,0, 1);
	Opd * opd2 = new LitOpd(std::to_string(convert->getNum()), 1);
	BinOp opr = FAIL64;
	if (opd1->getWidth() == 1) {
		opr = SUB8;
	} else {
		opr = SUB64;
	}
	BinOpQuad* myBinOp = new BinOpQuad (opd1, opr, opd1, opd2);
	proc->addQuad(myBinOp);
}

void ReadStmtNode::to3AC(Procedure * proc){
	Opd* my_Opd = myDst->flatten(proc);
	ReadQuad * read = new ReadQuad(my_Opd, proc->getProg()->nodeType(myDst));
	proc->addQuad(read);
}

void WriteStmtNode::to3AC(Procedure * proc){
	Opd* my_Opd = mySrc->flatten(proc);
	WriteQuad * wrt = new WriteQuad(my_Opd, proc->getProg()->nodeType(mySrc));
	proc->addQuad(wrt);
}

void IfStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
	Opd* my_Opd = myCond->flatten(proc);
	JmpIfQuad * cond = new JmpIfQuad(my_Opd, proc->makeLabel());
	proc->addQuad(cond);

	for (std::list<StmtNode *>::iterator it = myBody->begin(); it!=myBody->end(); ++it)
	{
		(*it)->to3AC(proc);
	}

	NopQuad * nope = new NopQuad();
	proc->addQuad(nope);
}

void IfElseStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
	Opd* my_Opd = myCond->flatten(proc);
	JmpIfQuad * cond = new JmpIfQuad(my_Opd, proc->makeLabel());
	proc->addQuad(cond);

	for (std::list<StmtNode *>::iterator it = myBodyTrue->begin(); it!=myBodyTrue->end(); ++it)
	{
		(*it)->to3AC(proc);
	}

	NopQuad * nope = new NopQuad();
	proc->addQuad(nope);

	JmpQuad* otherCond = new JmpQuad(proc->makeLabel());
	proc->addQuad(otherCond);

	for (std::list<StmtNode *>::iterator it = myBodyFalse->begin(); it!=myBodyFalse->end(); ++it)
	{
		(*it)->to3AC(proc);
	}

	nope = new NopQuad();
	proc->addQuad(nope);
}

void WhileStmtNode::to3AC(Procedure * proc){
	//TODO(Implement me)
	Label * iterate = proc->makeLabel();
	NopQuad * nope = new NopQuad();
	proc->addQuad(nope);

	Opd* my_Opd = myCond->flatten(proc);
	JmpIfQuad * cond = new JmpIfQuad(my_Opd, proc->makeLabel());
	proc->addQuad(cond);

	for (std::list<StmtNode *>::iterator it = myBody->begin(); it!=myBody->end(); ++it)
	{
		(*it)->to3AC(proc);
	}

	JmpQuad* otherCond = new JmpQuad(iterate);
	proc->addQuad(otherCond);

	nope = new NopQuad();
	proc->addQuad(nope);
}

void CallStmtNode::to3AC(Procedure * proc){
	myCallExp->flatten(proc);
}

void ReturnStmtNode::to3AC(Procedure * proc){
	Opd* my_Opd = myExp->flatten(proc);
	SetRetQuad * setRet = new SetRetQuad(my_Opd);
	proc->addQuad(setRet);
	JmpQuad * jmp = new JmpQuad(proc->getLeaveLabel());
	proc->addQuad(jmp);
}

void VarDeclNode::to3AC(Procedure * proc){
	SemSymbol * sym = ID()->getSymbol();
	if (sym == nullptr){
		throw new InternalError("null sym");
	}

	proc->gatherLocal(sym);
}

void VarDeclNode::to3AC(IRProgram * prog){
	SemSymbol * sym = ID()->getSymbol();
	if (sym == nullptr){
		throw new InternalError("null sym");
	}

	prog->gatherGlobal(sym);
}

Opd * IndexNode::flatten(Procedure * proc){
	//TODO(Implement me)
	//proc->gatherLocal(myBase->getSymbol());
	SemSymbol * sym = myBase->getSymbol();
	SymOpd * dst = proc->getSymOpd(myBase->getSymbol());
	//dst = proc->makeLoc(myBase->getSymbol(), dst->getWidth());
	Opd * opd1 = myBase->flatten(proc);
	Opd * opd2 = myOffset->flatten(proc);
	BinOp opr = ADD64;
	BinOpQuad* myBinOp = new BinOpQuad (dst, opr, opd1, opd2);
	proc->addQuad(myBinOp);
	return dst;

	//AddrOpd * addr = proc->makeAddrOpd(opd2->getWidth());
	//IndexQuad * index = new IndexQuad(addr,opd1,opd2);
	//proc->addQuad(index);
}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent)
Opd * IDNode::flatten(Procedure * proc){
	SymOpd* my_Opd = proc->getSymOpd(getSymbol());
	return my_Opd;
}

}
