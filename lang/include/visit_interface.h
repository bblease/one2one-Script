/*******************************************************************************
 * Name        : visit_interface.h
 * Author      : Ben Blease
 * Email	   : bblease@stevens.edu
 * Date        : 6/28/16
 * Description : Visitor interfaces for one2one. This file is needed for pushing to type stacks
 				 if typing is ever introduced.
 ******************************************************************************/

#ifndef VISIT_INTERFACE_H_
#define VISIT_INTERFACE_H_

class Visitor;
//expression.h
struct NullExp;
struct BlockExp;
struct NumExp;
struct OpExp;
struct BoolExp;
struct BoolOp;
struct NumBoolExp;
struct VarExp;
struct SetExp;
struct IfExp;
struct ForExp;
struct WhileExp;
struct ProcExp;
struct CallExp;
struct PrintExp;
struct IncExp;
struct DelExp;
struct EmptyExp;
//environment.h
struct Closure;
struct NumVal;
struct BoolVal;

class Visitor {
public:
	Visitor() { }

	~Visitor() { }
	
	//interp.h
	virtual void visit(NullExp &exp) = 0;
	
	virtual void visit(BlockExp &exp) = 0;

	virtual void visit(NumExp &exp) = 0;

	virtual void visit(OpExp &exp) = 0;

	virtual void visit(BoolExp &exp) = 0;

	virtual void visit(BoolOp &exp) = 0;

	virtual void visit(NumBoolExp &exp) = 0;

	virtual void visit(VarExp &exp) = 0;

	virtual void visit(SetExp &exp) = 0;

	virtual void visit(IfExp &exp) = 0;

	virtual void visit(ForExp &exp) = 0;

	virtual void visit(WhileExp &exp) = 0;

	virtual void visit(ProcExp &exp) = 0;

	virtual void visit(CallExp &exp) = 0;

	virtual void visit(PrintExp &exp) = 0;

	virtual void visit(IncExp &exp) = 0;

	virtual void visit(DelExp &exp) = 0;

	virtual void visit(EmptyExp &exp) = 0;

	//environment.h
	virtual void visit(Closure &exp) = 0;

	virtual void visit(NumVal &exp) = 0;

	virtual void visit(BoolVal &exp) = 0;
};


#endif /* VISIT_INTERFACE_H_ */