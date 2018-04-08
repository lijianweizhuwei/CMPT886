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
#include "./Swar.h"
#include "./Type_Legalization.h"
#include <stdlib.h>

using namespace llvm;
using namespace SWAR_TL;

namespace {
    struct Swar_TL : public FunctionPass {
        static char ID;
        Swar_TL() : FunctionPass(ID) {}

        bool runOnFunction(Function &function) override {
            errs() << function.getName() << ":\n";

            bool modified = false;
            int flag = 1;
            Value * result = nullptr;

            for (auto &block : function) {
                std::vector<BinaryOperator *> addInstsToErase;

                for (auto &inst : block) {
                    llvm::BinaryOperator * addInst = dyn_cast<llvm::BinaryOperator>(&inst);

                    if (!inst.isBinaryOp() || inst.getOpcode() != Instruction::Add) {
                        continue;
                    }

                    Value * vector1 = inst.getOperand(0);
                    Value * vector2 = inst.getOperand(1);

                    if (flag == 0) {
                        Swar * swar = new Swar(vector1, vector2, addInst);
                        result = swar->operate();
                    }else{

                        errs() << "We are trying to legalize " <<":\n";
                        Type_Legalization * type_legalization = new Type_Legalization(vector1, vector2, addInst);
                        result = type_legalization->legalize();
                    }

                    addInstsToErase.push_back(addInst);
                    addInst->replaceAllUsesWith(result);
                }

                for (auto &instr : addInstsToErase) {
                    instr->eraseFromParent();
                }
            }

            return modified;
        }
    };
}

char Swar_TL::ID = 0;

static RegisterPass<Swar_TL> X("Swar_TL", "SWAR_TL", false, false);