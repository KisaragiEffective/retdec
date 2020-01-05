/**
* @file src/llvmir2hll/ir/ext_cast_expr.cpp
* @brief Implementation of ExtCastExpr.
*        - floating point, sign and zero extension.
* @copyright (c) 2017 Avast Software, licensed under the MIT license
*/

#include "retdec/llvmir2hll/ir/ext_cast_expr.h"
#include "retdec/llvmir2hll/support/debug.h"
#include "retdec/llvmir2hll/support/visitor.h"

namespace retdec {
namespace llvmir2hll {

/**
* @brief Constructs a cast operator of LLVM Ext instructions.
*
* See create() for more information.
*/
ExtCastExpr::ExtCastExpr(Expression* op, Type* dstType,
		Variant variant):
	CastExpr(Value::ValueKind::ExtCastExpr, op, dstType), variant(variant) {}

bool ExtCastExpr::isEqualTo(Value* otherValue) const {
	// Both types and values of all operands have to be equal.
	if (ExtCastExpr* otherCastExpr = cast<ExtCastExpr>(otherValue)) {
		return dstType->isEqualTo(otherCastExpr->getType()) &&
			op->isEqualTo(otherCastExpr->getOperand());
	}
	return false;
}

/**
* @brief Clones the cast operator.
*/
Value* ExtCastExpr::clone() {
	ExtCastExpr* castExpr(ExtCastExpr::create(
		ucast<Expression>(op->clone()), dstType));
	castExpr->setMetadata(getMetadata());
	return castExpr;
}

/**
* @brief Returns the variant of the cast.
*/
ExtCastExpr::Variant ExtCastExpr::getVariant() const {
	return variant;
}

/**
* @brief Creates a new cast operand of LLVM Ext instructions.
*
* @param[in] op Operand.
* @param[in] dstType Destination type.
* @param[in] variant Variant of the cast.
*
* @par Preconditions
*  - @a op is non-null
*/
ExtCastExpr* ExtCastExpr::create(Expression* op, Type* dstType,
		Variant variant) {
	PRECONDITION_NON_NULL(op);
	PRECONDITION_NON_NULL(dstType);

	ExtCastExpr* expr(new ExtCastExpr(op, dstType, variant));

	// Initialization (recall that this cannot be called in a
	// constructor).
	op->addObserver(expr);

	return expr;
}

void ExtCastExpr::accept(Visitor *v) {
	v->visit(ucast<ExtCastExpr>(this));
}

} // namespace llvmir2hll
} // namespace retdec
