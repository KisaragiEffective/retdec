/**
* @file include/retdec/llvmir2hll/ir/var_def_stmt.h
* @brief A variable definition statement.
* @copyright (c) 2017 Avast Software, licensed under the MIT license
*/

#ifndef RETDEC_LLVMIR2HLL_IR_VAR_DEF_STMT_H
#define RETDEC_LLVMIR2HLL_IR_VAR_DEF_STMT_H

#include "retdec/llvmir2hll/ir/statement.h"
#include "retdec/llvmir2hll/support/smart_ptr.h"

namespace retdec {
namespace llvmir2hll {

class Expression;
class Variable;
class Visitor;

/**
* @brief A variable definition statement.
*
* Use create() to create instances. Instances of this class have reference
* object semantics. This class is not meant to be subclassed.
*/
class VarDefStmt final: public Statement {
public:
	static VarDefStmt* create(Variable* var,
		Expression* init = nullptr,
		Statement* succ = nullptr,
		Address a = Address::Undefined);

	virtual Value* clone() override;
	virtual bool isEqualTo(Value* otherValue) const override;
	virtual bool isCompound() override { return false; }
	virtual void replace(Expression* oldExpr, Expression* newExpr) override;
	virtual Expression* asExpression() const override;

	Variable* getVar() const;
	Expression* getInitializer() const;
	bool hasInitializer() const;

	void setVar(Variable* newVar);
	void setInitializer(Expression* newInit);
	void removeInitializer();

	/// @name Observer Interface
	/// @{
	virtual void update(Value* subject, Value* arg = nullptr) override;
	/// @}

	/// @name Visitor Interface
	/// @{
	virtual void accept(Visitor *v) override;
	/// @}

    static bool classof(const Value* v) {
        return v->getKind() == Value::ValueKind::VarDefStmt; }

private:
	// Since instances are created by calling the static function create(), the
	// constructor can be private.
	explicit VarDefStmt(Variable* var, Expression* init = nullptr,
		Address a = Address::Undefined);

private:
	/// Variable.
	Variable* var = nullptr;

	/// Variable initializer.
	Expression* init = nullptr;
};

} // namespace llvmir2hll
} // namespace retdec

#endif
