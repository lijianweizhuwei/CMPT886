/* Problems I have now:
** 1) We need to use BasicBlockPass instead of FunctionPass (difference?)
** 2) How to remove the original instructions?
** 3) How to test the result?
** 4) How to test performance?
**
** 5) Constants are all changed to i128? (Or changed to i32??)
** 6) BinaryOperator::createAnd??
** 7) Vectors are changed to i18 or i128?
** 8) How to move mask generator outside the function? (I can optimize it tho...)
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
#include <stdlib.h>

using namespace llvm;

namespace {
    struct Swar : public FunctionPass {
        static char ID;
        Swar() : FunctionPass(ID) {}

        uint64_t generateHMask(VectorType *);
        uint64_t generateLMask(VectorType *);
        Value * swar_add(Value * a1, Value * a2, uint64_t lmask, uint64_t hmask);

        bool runOnFunction(Function &function) override {
            errs() << function.getName() << ":\n";

            bool modified = false;

            for (auto &block : function) {
                // std::vector<BinaryOperator *> bitCastInstsToErase;

                for (auto &inst : block) {

                    if (!inst.isBinaryOp() || inst.getOpcode() != Instruction::Add) {
                        continue;
                    }
                    errs() << "  found binary instruction\n";

                    Value * vector1 = inst.getOperand(0);
                    Value * vector2 = inst.getOperand(1);

                    auto vectorType = dyn_cast<VectorType>(vector1->getType());
                    auto elemSize = vectorType->getScalarSizeInBits();
                    auto numElems = vectorType->getNumElements();
                    errs() << "    from vector <" << numElems << " x i" << elemSize
                          << ">\n";

                    auto vectorSize = elemSize * numElems;
                    Type * IntTy;
                    if (vectorSize > 256) { // larger than 256
                        errs() << "Length larger than 256\n";
                        continue;
                    } else if (vectorSize > 128) { // larger than 128, smaller than 256
                        // IntTy = llvm::IntegerType::getInt256Ty(inst.getContext());
                        errs() << "Doesn't find 256 yet\n";
                        continue;
                    } else { // smaller than 128
                        IntTy = llvm::IntegerType::getInt128Ty(inst.getContext());
                    }
                    modified = true;

                    // ???? Where is the destination?
                    BitCastInst * BCI1 = new llvm::BitCastInst(vector1, IntTy, "", &inst);
                    BitCastInst * BCI2 = new llvm::BitCastInst(vector2, IntTy, "", &inst);
                    uint64_t lmask = generateLMask(vectorType);
                    uint64_t hmask = generateHMask(vectorType);
                    Value * resultInt = swar_add(vector1, vector2, lmask, hmask);
                    BitCastInst * BCI3 = new llvm::BitCastInst(resultInt, vectorType, "", &inst);
                }
            }

            return modified;
        }
    };
}

char Swar::ID = 0;

static RegisterPass<Swar> X("Swar", "SWAR", false, false);

Value * swar_add(Value * a1, Value * a2, uint64_t lmask, uint64_t hmask) {
    IRBuilder<> builder(llvm::getGlobalContext());

    Value * m1 = builder.CreateAnd(a1, lmask);
    Value * m2 = builder.CreateAnd(a2, lmask);
    Value * r1 = builder.CreateAdd(m1, m2);
    Value * r2 = builder.CreateXor(a1, a2);
    Value * r3 = builder.CreateAnd(r2, hmask);
    Value * r4 = builder.CreateXor(r1, r3);
    // BinaryOperator *m1 = new BinaryOperator(Instruction::And, vector1, mask, "", tail);
    // BinaryOperator *m2 = new BinaryOperator(Instruction::And, vector2, mask, "", tail);

    return r4;
}

// Need to OPTIMIZE!!
uint64_t Swar::generateLMask(VectorType * vectorType) {
    auto elemSize = vectorType->getScalarSizeInBits();
    auto numElems = vectorType->getNumElements();

    char * ptr;
    char string[128];
    char unit[128];

    unsigned int j;
    for (j = 0; j < elemSize; j ++) {
        if (j == 0) {
            unit[j] = '0';
            continue;
        }
        unit[j] = '1';
    }
    unit[j] = '\0';

    for (j = 0; j < numElems; j ++) {
        for (unsigned int i = 0; i < elemSize; i ++) {
            string[j * elemSize + i] = unit[i];
        }
    }
    string[j * elemSize] = '\0';

    long int parsed = strtol(string, &ptr, 2);
    return parsed;
}

uint64_t Swar::generateHMask(VectorType * vectorType) {
    auto elemSize = vectorType->getScalarSizeInBits();
    auto numElems = vectorType->getNumElements();

    char * ptr;
    char string[128];
    char unit[128];

    unsigned int j;
    for (j = 0; j < elemSize; j ++) {
        if (j == 0) {
            unit[j] = '1';
            continue;
        }
        unit[j] = '0';
    }
    unit[j] = '\0';

    for (j = 0; j < numElems; j ++) {
        for (unsigned int i = 0; i < elemSize; i ++) {
            string[j * elemSize + i] = unit[i];
        }
    }
    string[j * elemSize] = '\0';

    long int parsed = strtol(string, &ptr, 2);
    return parsed;
}