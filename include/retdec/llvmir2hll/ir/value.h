/**
* @file include/retdec/llvmir2hll/ir/value.h
* @brief A base class of all objects a module can contain.
* @copyright (c) 2017 Avast Software, licensed under the MIT license
*/

#ifndef RETDEC_LLVMIR2HLL_IR_VALUE_H
#define RETDEC_LLVMIR2HLL_IR_VALUE_H

#include <iosfwd>
#include <string>

#include <llvm/Support/raw_ostream.h>

#include "retdec/llvmir2hll/support/metadatable.h"
#include "retdec/llvmir2hll/support/observer.h"
#include "retdec/llvmir2hll/support/smart_ptr.h"
#include "retdec/llvmir2hll/support/subject.h"
#include "retdec/llvmir2hll/support/visitable.h"
#include "retdec/utils/non_copyable.h"

namespace retdec {
namespace llvmir2hll {

/**
* @brief A base class of all objects a module can contain.
*
* Instances of this class have reference object semantics.
*/
class Value: public Visitable, public Metadatable<std::string>,
		public SharableFromThis<Value>, public Observer<Value>,
		public Subject<Value>, private retdec::utils::NonCopyable {
public:
	virtual ~Value() = default;

	virtual Value* getSelf() override;

	/**
	* @brief Returns a clone of the value.
	*
	* A clone is (in most cases) an exact copy of the value. This member
	* function provides the copy mechanism for reference objects.
	*
	* The following parts of values are not cloned:
	*  - predecessors and successors of statements
	*
	* The following subclasses of Value are not cloned, i.e. they are
	* returned without any copying:
	*  - Function
	*  - Variable
	*
	* Statements in compound statements (i.e. statements where @c isCompound()
	* returns @c true) are cloned without their successors; therefore, e.g.,
	* just the first statement of every if's clause is cloned.
	*/
	virtual Value* clone() = 0;

	/**
	* @brief Returns @c true if this value is equal to @a otherValue, @c false
	*        otherwise.
	*
	* This member function brings the support of value object semantics into
	* reference objects, namely equality based not only on identity.
	*
	* This function doesn't consider observers, metadata, etc.
	*/
	virtual bool isEqualTo(Value* otherValue) const = 0;

	std::string getTextRepr();

public:
	enum class ValueKind
	{
		Value,
			GlobalVarDef,
			Function,
			Statement,
				AssignStmt,
				BreakStmt,
				CallStmt,
				ContinueStmt,
				EmptyStmt,
				ForLoopStmt,
				GotoStmt,
				IfStmt,
				ReturnStmt,
				SwitchStmt,
				UForLoopStmt,
				UnreachableStmt,
				VarDefStmt,
				WhileLoopStmt,
				_Statement_END,
			Type,
				ArrayType,
				FloatType,
				FunctionType,
				IntType,
				PointerType,
				StringType,
				StructType,
				UnknownType,
				VoidType,
				_Type_END,
			Expression,
				BinaryOpExpr,
					AddOpExpr,
					AndOpExpr,
					ArrayIndexOpExpr,
					AssignOpExpr,
					BitAndOpExpr,
					BitOrOpExpr,
					BitShlOpExpr,
					BitShrOpExpr,
					BitXorOpExpr,
					CommaOpExpr,
					DivOpExpr,
					EqOpExpr,
					GtEqOpExpr,
					GtOpExpr,
					LtEqOpExpr,
					LtOpExpr,
					ModOpExpr,
					MulOpExpr,
					NeqOpExpr,
					OrOpExpr,
					StructIndexOpExpr,
					SubOpExpr,
					_BinaryOpExpr_END,
				CallExpr,
				CastExpr,
					BitCastExpr,
					ExtCastExpr,
					FPToIntCastExpr,
					IntToFPCastExpr,
					IntToPtrCastExpr,
					PtrToIntCastExpr,
					TruncCastExpr,
					_CastExpr_END,
				Constant,
					ConstArray,
					ConstBool,
					ConstFloat,
					ConstInt,
					ConstNullPointer,
					ConstString,
					ConstStruct,
					ConstSymbol,
					_Constant_END,
				UnaryOpExpr,
					AddressOpExpr,
					DerefOpExpr,
					NegOpExpr,
					NotOpExpr,
					_UnaryOpExpr_END,
				TernaryOpExpr,
				Variable,
				_Expression_END,
			_Value_END,
	};
public:
	Value(ValueKind k) : kind(k) {}
	ValueKind getKind() const { return kind; }
	static bool classof(const Value* v) { return true; }
private:
	const ValueKind kind = ValueKind::Value;

protected:
	Value() = default;
};

/// @name Emission To Streams
/// @{

// These functions have to be declared in the same namespace that defines Value
// and its subclasses; C++ lookup rules rely on that.
llvm::raw_ostream &operator<<(llvm::raw_ostream &os, Value* value);
// The following function is used to print values in tests (Google Tests
// framework). It must have this signature; Value* does not work.
std::ostream &operator<<(std::ostream &os, Value *value);

/// @}

} // namespace llvmir2hll
} // namespace retdec

#endif
