#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
using namespace llvm;

namespace {
  struct BitcastLegalization : public FunctionPass {
    static char ID;
    BitcastLegalization() : FunctionPass(ID) {}

    void legalize(BitCastInst *);

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
        std::vector<BitCastInst *> bitCastInstsToErase;

        for (auto &inst : block) {
          auto bitCastInst = dyn_cast<BitCastInst>(&inst);
          if (!bitCastInst) {
            continue;
          }
          errs() << "  found bitcast instruction\n";

          auto sourceVectorType =
              dyn_cast<VectorType>(bitCastInst->getOperand(0)->getType());
          if (!sourceVectorType) {
            continue;
          }
          auto elemSize = sourceVectorType->getScalarSizeInBits();
          auto numElems = sourceVectorType->getNumElements();
          errs() << "    from vector <" << numElems << " x i" << elemSize
                 << ">\n";

          if (!bitCastInst->getType()->isIntegerTy()) {
            continue;
          }
          auto width = bitCastInst->getType()->getPrimitiveSizeInBits();
          errs() << "    to integer i" << width << "\n";

          if (elemSize >= 8 && isPowerOf2(elemSize)) {
            continue;
          }
          errs() << "    i" << elemSize
                  << " is an invalid vector element type."
                  << " performing legalization\n";

          legalize(bitCastInst);
          modified = true;
          bitCastInstsToErase.push_back(bitCastInst);
        }

        for (auto &instr : bitCastInstsToErase) {
          instr->eraseFromParent();
        }
      }

      return modified;
    }
  };
}

char BitcastLegalization::ID = 0;

static RegisterPass<BitcastLegalization> X(
  "bitcastlegalization", "Bitcast Legalization", false, false
);


void BitcastLegalization::legalize(llvm::BitCastInst *bitCastInst) {
  auto sourceVector = bitCastInst->getOperand(0);
  auto sourceVectorType = cast<VectorType>(sourceVector->getType());
  auto elemSize = sourceVectorType->getScalarSizeInBits();
  auto numElems = sourceVectorType->getNumElements();
  auto totalSize = elemSize * numElems;

  assert(numElems > 1 &&
         "BitcastLegalization has nothing to do with v1!");

  int legalizedTotalSize = 8;
  while (legalizedTotalSize < totalSize) {
    legalizedTotalSize *= 2;
  }

  IRBuilder<> builder(bitCastInst);

  Value *lastValue;
  for (int i = 0; i < numElems; ++i) {
    Value *elemValue = builder.CreateExtractElement(sourceVector, i);
    Value *zextElemValue = builder.CreateZExt(elemValue, IntegerType::get(builder.getContext(), legalizedTotalSize));

    if (i == 0) {
      lastValue = zextElemValue;
    }
    else {
      Value *shiftedValue = builder.CreateShl(zextElemValue, i * elemSize);
      lastValue = builder.CreateOr(lastValue, shiftedValue);
    }
  }

  Value *result = builder.CreateTrunc(lastValue, IntegerType::get(builder.getContext(), totalSize));

  bitCastInst->replaceAllUsesWith(result);
}
