#include "ast.hpp"

namespace crona{

IRProgram * ProgramNode::to3AC(TypeAnalysis * ta){
	IRProgram * prog = new IRProgram(ta);
	for (auto global : *myGlobals){
		global->to3AC(prog);
	}
	return prog;
}

static void formalsTo3AC(Procedure * proc, 
  std::list<FormalDeclNode *> * myFormals){
	for (auto formal : *myFormals){
		formal->to3AC(proc);
	}
	unsigned int argIdx = 1;
	for (auto formal : *myFormals){
		SemSymbol * sym = formal->ID()->getSymbol();
		SymOpd * opd = proc->getSymOpd(sym);
		
		Quad * inQuad = new GetArgQuad(argIdx, opd);
		proc->addQuad(inQuad);
		argIdx += 1;
	}
}

void FnDeclNode::to3AC(IRProgram * prog){
	SemSymbol * mySym = this->ID()->getSymbol();
	Procedure * proc = prog->makeProc(mySym->getName());

	//Generate the getin quads
	formalsTo3AC(proc, myFormals);

	for (auto stmt : *myBody){
		stmt->to3AC(proc);
	}
}

void FnDeclNode::to3AC(Procedure * proc){
	throw new InternalError("FnDecl at a local scope");
}

//We only get to this node if we are in a stmt
// context (DeclNodes protect descent) 
Opd * IDNode::flatten(Procedure * proc){
	SemSymbol * sym = this->getSymbol();
	Opd * res = proc->getSymOpd(sym);
	if (!res){
		throw new InternalError("null id sym");;
	}
	return res;
}

void FormalDeclNode::to3AC(IRProgram * prog){
	throw new InternalError("Formal at a global level");
}

void FormalDeclNode::to3AC(Procedure * proc){
	SemSymbol * sym = ID()->getSymbol();
	proc->gatherFormal(sym);
}

Opd * IntLitNode::flatten(Procedure * proc){
	return LitOpd::buildInt(myNum);
}

Opd * StrLitNode::flatten(Procedure * proc){
	Opd * res = proc->getProg()->makeString(myStr);
	return res;
}

Opd * HavocNode::flatten(Procedure * proc){
	Opd * res = proc->makeTmp(1);
	proc->addQuad(new HavocQuad(res));
	return res;
}

Opd * TrueNode::flatten(Procedure * proc){
	Opd * res = new LitOpd("1", 1);
	return res;
}

Opd * FalseNode::flatten(Procedure * proc){
	Opd * res = new LitOpd("0", 1);
	return res;
}


Opd * AssignExpNode::flatten(Procedure * proc){
	Opd * rhs = mySrc->flatten(proc);
	Opd * lhs = myDst->flatten(proc);
	if (!lhs){
		throw InternalError("null tgt");
	}
	
	AssignQuad * quad = new AssignQuad(lhs, rhs);
	quad->setComment("Assign");
	proc->addQuad(quad);
	return lhs;
}

static void argsTo3AC(Procedure * proc, std::list<ExpNode *> * args){
	/*
	size_t argIdx = 1;
	for (auto elt : *args){
		Opd * arg = elt->flatten(proc);
		Quad * argQuad = new SetInQuad(argIdx, arg);
		proc->addQuad(argQuad);
		argIdx++;
	}
	*/
	std::list<Opd *> argOpds;
	for (auto elt : *args){
		Opd * argOpd = elt->flatten(proc);
		argOpds.push_back(argOpd);
	}
	size_t argIdx = 1;
	for (auto argOpd : argOpds){
		Quad * argQuad = new SetArgQuad(argIdx, argOpd);
		proc->addQuad(argQuad);
		argIdx++;
	}
}

Opd * CallExpNode::flatten(Procedure * proc){
	argsTo3AC(proc, myArgs);
	Quad * callQuad = new CallQuad(myID->getSymbol());
	proc->addQuad(callQuad);

	SemSymbol * idSym = myID->getSymbol();
	const FnType * calleeType = idSym->getDataType()->asFn();
	const DataType * retType = calleeType->getReturnType();
	if (retType->isVoid()){
		return nullptr;
	} else {
		Opd * retVal = proc->makeTmp(Opd::width(retType));
		Quad * getRet = new GetRetQuad(retVal);
		proc->addQuad(getRet);
		return retVal;
	}
}

Opd * ByteToIntNode::flatten(Procedure * proc){
	Opd * child = myChild->flatten(proc);
	Opd * dst = proc->makeTmp(8);

	AssignQuad * quad = new AssignQuad(dst, child);
	quad->setComment("Promotion");
	proc->addQuad(quad);
	return dst;
}

Opd * NegNode::flatten(Procedure * proc){
	Opd * child = myExp->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	UnaryOp opr = UnaryOp::NEG64;
	Quad * quad = new UnaryOpQuad(dst, opr, child);
	proc->addQuad(quad);
	return dst;
}

Opd * NotNode::flatten(Procedure * proc){
	Opd * child = myExp->flatten(proc);
	Opd * dst = proc->makeTmp(1);
	Quad * quad = new UnaryOpQuad(dst, NOT8, child);
	proc->addQuad(quad);
	return dst;
}

Opd * PlusNode::flatten(Procedure * proc){
	Opd * childL = myExp1->flatten(proc);
	Opd * childR = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::ADD64;
	if (width == 1){ opr = BinOp::ADD8; }
	Quad * quad = new BinOpQuad(dst, opr, childL, childR);
	proc->addQuad(quad);
	return dst;
}

Opd * MinusNode::flatten(Procedure * proc){
	Opd * childL = myExp1->flatten(proc);
	Opd * childR = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::SUB64;
	if (width == 1){ opr = BinOp::SUB8; }
	Quad * quad = new BinOpQuad(dst, opr, childL, childR);
	proc->addQuad(quad);
	return dst;
}

Opd * TimesNode::flatten(Procedure * proc){
	Opd * childL = myExp1->flatten(proc);
	Opd * childR = myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::MULT64;
	if (width == 1){ opr = BinOp::MULT8; }
	Quad * quad = new BinOpQuad(dst, opr, childL, childR);
	proc->addQuad(quad);
	return dst;
}

Opd * DivideNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::DIV64;
	if (width == 1){ opr = BinOp::DIV8; }
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * AndNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	Opd * opRes = proc->makeTmp(1);
	BinOpQuad * quad = new BinOpQuad(opRes, AND8, op1, op2);
	proc->addQuad(quad);
	return opRes;
}

Opd * OrNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	Opd * opRes = proc->makeTmp(1);
	BinOpQuad * quad = new BinOpQuad(opRes, OR8, op1, op2);
	proc->addQuad(quad);
	return opRes;
}

Opd * EqualsNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::EQ64;
	if (width == 1){ opr = BinOp::EQ8; }
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * NotEqualsNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::NEQ64;
	if (width == 1){ opr = BinOp::NEQ8; }
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * GreaterNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::GT64;
	if (width == 1){ opr = BinOp::GT8; }
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * GreaterEqNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::GTE64;
	if (width == 1){ opr = BinOp::GTE8; }
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * LessNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::LT64;
	if (width == 1){ opr = BinOp::LT8; }
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

Opd * LessEqNode::flatten(Procedure * proc){
	Opd * op1 = this->myExp1->flatten(proc);
	Opd * op2 = this->myExp2->flatten(proc);
	size_t width = proc->getProg()->opWidth(this->myExp1);
	Opd * dst = proc->makeTmp(width);
	BinOp opr = BinOp::LTE64;
	if (width == 1){ opr = BinOp::LTE8; }
	BinOpQuad * quad = new BinOpQuad(dst, opr, op1, op2);
	proc->addQuad(quad);
	return dst;
}

void AssignStmtNode::to3AC(Procedure * proc){
	Opd * res = myExp->flatten(proc);
	// Since we're at the stmt level, we know
	// this opd isn't going to be used. We 
	// could delete it
}

void PostIncStmtNode::to3AC(Procedure * proc){
	Opd * child = this->myLVal->flatten(proc);
	size_t width = proc->getProg()->opWidth(myLVal);
	BinOp opr = BinOp::ADD64;
	if (width == 1){ opr = BinOp::ADD8; }
	LitOpd * litOpd = new LitOpd("1", width);
	BinOpQuad * quad = new BinOpQuad(child, opr, child, litOpd);
	proc->addQuad(quad);
}

void PostDecStmtNode::to3AC(Procedure * proc){
	Opd * child = this->myLVal->flatten(proc);
	size_t width = proc->getProg()->opWidth(myLVal);
	BinOp opr = BinOp::SUB64;
	if (width == 1){ opr = BinOp::SUB8; }
	LitOpd * litOpd = new LitOpd("1", width);
	BinOpQuad * quad = new BinOpQuad(child, opr, child, litOpd);
	proc->addQuad(quad);
}

void ReadStmtNode::to3AC(Procedure * proc){
	Opd * child = this->myDst->flatten(proc);
	proc->addQuad(new IntrinsicInputQuad(child,
		proc->getProg()->nodeType(myDst)));
}

void WriteStmtNode::to3AC(Procedure * proc){
	Opd * child = this->mySrc->flatten(proc);
	proc->addQuad(new IntrinsicOutputQuad(child,
		proc->getProg()->nodeType(mySrc)));
}

void IfStmtNode::to3AC(Procedure * proc){
	Opd * cond = myCond->flatten(proc);
	Label * afterLabel = proc->makeLabel();
	Quad * afterNop = new NopQuad();
	afterNop->addLabel(afterLabel);

	proc->addQuad(new JmpIfQuad(cond, afterLabel));
	for (auto stmt : *myBody){
		stmt->to3AC(proc);
	}
	proc->addQuad(afterNop);
}

void IfElseStmtNode::to3AC(Procedure * proc){
	Label * elseLabel = proc->makeLabel();
	Quad * elseNop = new NopQuad();
	elseNop->addLabel(elseLabel);
	Label * afterLabel = proc->makeLabel();
	Quad * afterNop = new NopQuad();
	afterNop->addLabel(afterLabel);

	Opd * cond = myCond->flatten(proc);

	Quad * jmpFalse = new JmpIfQuad(cond, elseLabel);
	proc->addQuad(jmpFalse);
	for (auto stmt : *myBodyTrue){
		stmt->to3AC(proc);
	}
	
	Quad * skipFall = new JmpQuad(afterLabel);
	proc->addQuad(skipFall);

	proc->addQuad(elseNop);
	
	for (auto stmt : *myBodyFalse){
		stmt->to3AC(proc);
	}

	proc->addQuad(afterNop);
}

void WhileStmtNode::to3AC(Procedure * proc){
	Quad * headNop = new NopQuad();
	Label * headLabel = proc->makeLabel();
	headNop->addLabel(headLabel);

	Label * afterLabel = proc->makeLabel();
	Quad * afterQuad = new NopQuad();
	afterQuad->addLabel(afterLabel);

	proc->addQuad(headNop);
	Opd * cond = myCond->flatten(proc);
	Quad * jmpFalse = new JmpIfQuad(cond, afterLabel);
	proc->addQuad(jmpFalse);

	for (auto stmt : *myBody){
		stmt->to3AC(proc);
	}

	Quad * loopBack = new JmpQuad(headLabel);
	proc->addQuad(loopBack);
	proc->addQuad(afterQuad);
}

void CallStmtNode::to3AC(Procedure * proc){
	Opd * res = myCallExp->flatten(proc);
	//Since we're in a callStmt, the GetRet quad
	// generated as the last action of the subtree
	// was unnecessary. Remove it from the procedure.
	if (res != nullptr){
		//A void call will not generate a getout
		Quad * last = proc->popQuad();
	}
	//Should probably delete the last quad, but
	// we've leaked so much memory why start worrying now?
}

void ReturnStmtNode::to3AC(Procedure * proc){
	if (myExp != nullptr){
		Opd * res = myExp->flatten(proc);
		Quad * setOut = new SetRetQuad(res);
		proc->addQuad(setOut);
	}
	
	Label * leaveLbl = proc->getLeaveLabel();
	Quad * jmpLeave = new JmpQuad(leaveLbl);
	proc->addQuad(jmpLeave);
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
	Opd * baseOpd = myBase->flatten(proc);
	Opd * offOpd = myOffset->flatten(proc);
	
	const DataType * type = proc->getProg()->nodeType(this);
	size_t width = Opd::width(type);
	
	if (width == 8){
		LitOpd * stride = new LitOpd("8", 8);
		Opd * scaledOff = proc->makeTmp(8);
		Quad * scale = new BinOpQuad(scaledOff, MULT64, offOpd, stride);
		offOpd = scaledOff;
		proc->addQuad(scale);
	} else if (width == 1){
		//No need to scale bytes
	}


	AddrOpd * idxLoc = proc->makeAddrOpd(width);
	IndexQuad * idx = new IndexQuad(idxLoc, baseOpd, offOpd);
	proc->addQuad(idx);
	return idxLoc;
}

}
