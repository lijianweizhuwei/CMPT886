/* Problems I have now:
** 1) We need to use BasicBlockPass instead of FunctionPass (difference?)
**
** 5) Constants are all changed to i128? (Or changed to i32??)
** 6) Mask 149796 -> -112348??????? two-complement something.
**
** ./bin/opt -load lib/LLVMSwar.dylib -Swar /Users/sally/code/test/add2.ll
** updaet2
** 1) test performance
** 2) bitcast
** 3) negative numbers
**
*/

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/Support/Casting.h"
#include <stdlib.h>

using namespace llvm;

namespace SWAR_TL {

    class Swar {
    private:
        Value * vector1;
        Value * vector2;
        BinaryOperator * inst;

    public:
        llvm::Value * generateMask(llvm::VectorType * vectorType, bool isLow);
        llvm::Value * swar_add(llvm::Value * a1, llvm::Value * a2, llvm::VectorType * vectorType, llvm::Instruction * addInst);
        llvm::Value * swar_sub(llvm::Value * a1, llvm::Value * a2, llvm::VectorType * vectorType, llvm::Instruction * subInst);
        llvm::Value * bitcast(llvm::Value * vector, llvm::BinaryOperator * addInst);

    public:
        Swar(Value * _vector1, Value * _vector2, BinaryOperator * _inst) {
            vector1 = _vector1;
            vector2 = _vector2;
            inst = _inst;
        }
        llvm::Value * operate();
    };

} // end llvm namespace
