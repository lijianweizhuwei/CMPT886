#include <math.h>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/DerivedTypes.h"

using namespace llvm;

namespace SWAR_TL {
    class Type_Legalization{
        public:

            Type_Legalization(Value * _vector1, Value * _vector2, BinaryOperator * _inst){
                vector1 = _vector1;
                vector2 = _vector2;
                inst = _inst;
            }
            ~Type_Legalization();
            Value * legalize();

        private:

            const int registerSize = 128; 
            const int split_max_num = 10;
            Value * vector1;
            Value * vector2;
            BinaryOperator * inst;

            Value * widen(Value * vector, const int numElems, const int elemSize);
            Value * promote(Value * vector, const int numElems, const int elemSize);
            Value * split(Value * vector, const int numElems, const int elemSize);
            Value * binaryOp(Value * vector1, Value * vector2, Instruction * binaryOpInst=nullptr);

            bool isPowerOf2(unsigned num);
            int nextPowerOf2(unsigned num);


    };


}
