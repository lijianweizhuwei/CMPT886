/* Problems I have now:
** 1) We need to use BasicBlockPass instead of FunctionPass (difference?)
** 2) How to remove the original instructions?
** 3) How to test the result?
** 4) How to test performance?
**
** 5) Constants are all changed to i128? (Or changed to i32??)
** 6) Mask 149796 -> -112348???????
** 7) Vectors are changed to i18 or i128? zext or bitcast??
** 8) How to move mask generator outside the function? (I can optimize it tho...)
**
** ./bin/opt -load lib/LLVMSwar.dylib -Swar /Users/sally/code/test/add2.ll
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
#include <stdlib.h>

using namespace llvm;

#define HIGHMASK false
#define LOWMASK true

namespace {
    struct Swar : public FunctionPass {
        static char ID;
        Swar() : FunctionPass(ID) {}

        Constant * generateMask(VectorType * vectorType, bool isLow);
        void swar_add(Value * a1, Value * a2, Value * lmask, Value * hmask, Instruction * InsertBefore=nullptr);

        bool runOnFunction(Function &function) override {
            errs() << function.getName() << ":\n";

            bool modified = false;

            for (auto &block : function) {
                std::vector<BinaryOperator *> addInstsToErase;

                for (auto &inst : block) {

                    if (!inst.isBinaryOp() || inst.getOpcode() != Instruction::Add) {
                        continue;
                    }
                    BinaryOperator * addInst = dyn_cast<BinaryOperator>(&inst);
                    LLVMContext & context = llvm::getGlobalContext();
                    errs() << "  found binary instruction\n";

                    Value * vector1 = inst.getOperand(0);
                    Value * vector2 = inst.getOperand(1);

                    auto vectorType = dyn_cast<VectorType>(vector1->getType());
                    auto elemSize = vectorType->getScalarSizeInBits();
                    auto numElems = vectorType->getNumElements();
                    errs() << "    from vector <" << numElems << " x i" << elemSize << ">\n";

                    auto vectorSize = elemSize * numElems;
                    Type * castTy;
                    if (vectorSize > 256) { // larger than 256
                        errs() << "Length larger than 256\n";
                        continue;
                    } else {
                        castTy = llvm::IntegerType::get(context, vectorSize);
                    }
                    modified = true;

                    // ???? Where is the destination?
                    BitCastInst * a1 = new BitCastInst(vector1, castTy, "", addInst);
                    BitCastInst * a2 = new BitCastInst(vector2, castTy, "", addInst);
                    Constant * lmask = generateMask(vectorType, LOWMASK);
                    Constant * hmask = generateMask(vectorType, HIGHMASK);

                    // // Value * resultInt = swar_add((Value *)BCI1, (Value *)BCI2, _lmask, _hmask, addInst);
                    BinaryOperator * m1 = BinaryOperator::Create(Instruction::And, (Value *)a1, (Value *)lmask, "", addInst);
                    BinaryOperator * m2 = BinaryOperator::Create(Instruction::And, (Value *)a2, (Value *)lmask, "", addInst);
                    BinaryOperator * r1 = BinaryOperator::Create(Instruction::Add, (Value *)m1, (Value *)m2, "", addInst);
                    BinaryOperator * r2 = BinaryOperator::Create(Instruction::Xor, (Value *)a1, (Value *)a2, "", addInst);
                    BinaryOperator * r3 = BinaryOperator::Create(Instruction::And, (Value *)r2, (Value *)hmask, "", addInst);
                    BinaryOperator * r4 = BinaryOperator::Create(Instruction::Xor, (Value *)r1, (Value *)r3, "", addInst);

                    BitCastInst * BCI3 = new llvm::BitCastInst((Value *)r4, vectorType, "", addInst);
                    addInstsToErase.push_back(addInst);
                }

                for (auto &instr : addInstsToErase) {
                    instr->eraseFromParent();
                }
            }

            return modified;
        }
    };
}

char Swar::ID = 0;

static RegisterPass<Swar> X("Swar", "SWAR", false, false);

void swar_add(Value * a1, Value * a2, Value * lmask, Value * hmask, Instruction *InsertBefore=nullptr) {
    // IRBuilder<> builder(llvm::getGlobalContext());

    // BinaryOperator::Create(Instruction::And, a1, lmask, "");
    // BinaryOperator::Create(Instruction::And, a2, lmask, "");
    // Value * m1 = builder.CreateAnd(a1, lmask);
    // Value * m2 = builder.CreateAnd(a2, lmask);
    // Value * r1 = builder.CreateAdd(m1, m2);
    // Value * r2 = builder.CreateXor(a1, a2);
    // Value * r3 = builder.CreateAnd(r2, hmask);
    // Value * r4 = builder.CreateXor(r1, r3);
}

// Need to OPTIMIZE!!
Constant * Swar::generateMask(VectorType * vectorType, bool isLow = false) {
    auto elemSize = vectorType->getScalarSizeInBits();
    auto numElems = vectorType->getNumElements();
    Type * castTy = llvm::IntegerType::get(llvm::getGlobalContext(), elemSize * numElems);

    char * ptr;
    char string[128];
    char unit[128];

    unsigned int j;
    for (j = 0; j < elemSize; j ++) {
        if (j == 0) {
            unit[j] = (isLow) ? '0' : '1';
            continue;
        }
        unit[j] = (isLow) ? '1' : '0';
    }
    unit[j] = '\0';

    for (j = 0; j < numElems; j ++) {
        for (unsigned int i = 0; i < elemSize; i ++) {
            string[j * elemSize + i] = unit[i];
        }
    }
    string[j * elemSize] = '\0';

    unsigned long int parsed = strtol(string, &ptr, 2);
    Constant * mask = llvm::ConstantInt::get(castTy, parsed);

    return mask;
}