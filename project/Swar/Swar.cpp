#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

namespace {
    struct Swar : public FunctionPass {
        static char ID;
        Swar() : FunctionPass(ID) {}

        uint64_t generateMask(BinaryOperator *);

        bool isPowerOf2(unsigned num) {
            while (num % 2 == 0 && num > 1) {
                num /= 2;
            }

            return num == 1;
        }

        bool runOnFunction(Function &function) override {
            errs() << function.getName() << ":\n";

            bool modified = false;

            for (auto &block : function) {
                // std::vector<BinaryOperator *> bitCastInstsToErase;

                for (auto &inst : block) {
                    auto addInst = dyn_cast<BinaryOperator>(&inst);

                    if (!inst.isBinaryOp() || inst.getOpcode() != Instruction::Add) {
                        continue;
                    }
                    errs() << "  found binary instruction\n";

                    auto vectorType = dyn_cast<VectorType>(inst.getOperand(0)->getType());
                    auto elemSize = vectorType->getScalarSizeInBits();
                    auto numElems = vectorType->getNumElements();
                    errs() << "    from vector <" << numElems << " x i" << elemSize
                          << ">\n";

                    if (elemSize * numElems > 128) {
                        errs() << "Length larger than 128\n";
                        continue;
                    }

                    Value *vector1 = inst.getOperand(0);
                    Value *vector2 = inst.getOperand(1);
                    VectorType *vector1_ = cast<VectorType>(vector1->getType());
                    VectorType *vector2_ = cast<VectorType>(vector2->getType());

                    uint64_t mask = generateMask(addInst);
                    errs() << mask << "\n";

                    // Value * vector1 = inst.getOperand(1);
                    // errs() << inst.getOperand(1)->getType();
                    // llvm::BinaryOperator::CreateAnd(vector1, llvm::ConstantInt::get(Int32Ty, 1), "", inst);
                }
            }

            return modified;
        }
    };
}

char Swar::ID = 0;

static RegisterPass<Swar> X("Swar", "Bitcast Legalization", false, false);

uint64_t Swar::generateMask(llvm::BinaryOperator *addInst) {
    auto vectorType = cast<VectorType>(addInst->getOperand(0)->getType());
    auto elemSize = vectorType->getScalarSizeInBits();
    auto numElems = vectorType->getNumElements();

    char * ptr;
    char string[elemSize*numElems];
    char unit[elemSize];

    for (unsigned int j = 0; j < elemSize; j ++) {
        if (j == 0) {
            strcat(unit, "0");
            continue;
        }
        strcat(unit, "1");
    }

    for (unsigned int i = 0; i < numElems; i ++) {
        strcat(string, unit);
    }

    long int parsed = strtol(string, & ptr, 2);
    errs() << parsed << "\n";
    return parsed;
}