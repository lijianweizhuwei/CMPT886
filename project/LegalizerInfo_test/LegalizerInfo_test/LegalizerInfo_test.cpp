//===- lib/CodeGen/GlobalISel/LegalizerInfo.cpp - Legalizer ---------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Implement an interface to specify and query how an illegal operation on a
// given type should be expanded.
//
// Issues to be resolved:
//   + Make it fast.
//   + Support weird types like i3, <7 x i3>, ...
//   + Operations with more than one type (ICMP, CMPXCHG, intrinsics, ...)
//
//===----------------------------------------------------------------------===//

#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
// #include "llvm/Instruction.h"

using namespace llvm;

namespace {
  struct LegalizerInfo_test : public FunctionPass {
    static char ID;
    LegalizerInfo_test() : FunctionPass(ID) {}
    bool LegalizeTypes();


    bool isPowerOf2(unsigned num) {
      while (num % 2 == 0 && num > 1) {
        num /= 2;
      }

      return num == 1;
    }

    bool runOnFunction(Function &function) override {
      errs() << "*******function name*********" << ":\n";
      errs() << function.getName() << ":\n";

      bool modified = false;

      for (auto &block : function) {
        // std::vector<BitCastInst *> bitCastInstsToErase;

        for (auto &inst : block) {
          // auto bitCastInst = dyn_cast<BitCastInst>(&inst);
          auto add_operation = dyn_cast<BinaryOperator>(&inst);

          if (!add_operation) {
            continue;
          }
          if (!inst.isBinaryOp() || inst.getOpcode() != Instruction::Add) {
            continue;
          }

          errs() << "*******inst.getOpcode()*********" << ":\n";
          errs() << inst.getOpcode()<< ">\n";
          errs() << "*******inst.isBinaryOp()*********" << ":\n";
          errs() << inst.isBinaryOp()<< ">\n";
          errs() << "*******getOpcodeNamen*************\n";
          errs() << llvm::AddrSpaceCastInst::getOpcodeName(add_operation->getOpcode()) << ">\n";
          errs() << "*************************************" << ">\n";

          auto sourceVectorType =
              dyn_cast<VectorType>(add_operation->getOperand(0)->getType());
          if (!sourceVectorType) {
            continue;
          }
          auto elemSize = sourceVectorType->getScalarSizeInBits();
          auto numElems = sourceVectorType->getNumElements();

          errs() << "Whether it is a valid elementtype" << ">\n";
          errs() << VectorType::isValidElementType(sourceVectorType)<< ">\n";
          errs() << "Whether it is a vector type" << ">\n";
          errs() << sourceVectorType->isVectorTy()<< ">\n";
          errs() << "get the element type" << ">\n";
          errs() << sourceVectorType->getVectorElementType()<< ">\n";
          errs() << add_operation->getOperand(0)->getType()->isVectorTy()<< ">\n";
          errs() << add_operation->getOperand(1)->getType()->isVectorTy()<< ">\n";
          errs() << "    from vector <" << numElems << " x i" << elemSize
                 << ">\n";

          SelectionDAG::LegalizeVectors()
          // if (!bitCastInst->getType()->isIntegerTy()) {
          //   continue;
          // }
          auto width = add_operation->getType()->getPrimitiveSizeInBits();
          errs() << "    to integer i" << width << "\n";

          if (elemSize >= 8 && isPowerOf2(elemSize)) {
            continue;
          }
          errs() << "    i" << elemSize
                  << " is an invalid vector element type."
                  << " performing legalization\n";

          bool flag = LegalizeTypes();
          modified = true;
          // bitCastInstsToErase.push_back(bitCastInst);
        }

        // for (auto &instr : bitCastInstsToErase) {
        //   instr->eraseFromParent();
        // }
      }

      return modified;
    }
  };
}

char LegalizerInfo_test::ID = 0;

static RegisterPass<LegalizerInfo_test> X(
  "LegalizerInfo_test", "LegalizerInfo test", false, false
);

bool LegalizerInfo_test::LegalizeTypes(){
  errs() << "LEGALIZATION!!!"<< ">\n";
  return true;
}


// void LegalizerInfo_test::legalize(llvm::BitCastInst *bitCastInst) {
//   auto sourceVector = bitCastInst->getOperand(0);
//   auto sourceVectorType = cast<VectorType>(sourceVector->getType());
//   auto elemSize = sourceVectorType->getScalarSizeInBits();
//   auto numElems = sourceVectorType->getNumElements();
//   auto totalSize = elemSize * numElems;

//   assert(numElems > 1 &&
//          "BitcastLegalization has nothing to do with v1!");

//   int legalizedTotalSize = 8;
//   while (legalizedTotalSize < totalSize) {
//     legalizedTotalSize *= 2;
//   }

//   IRBuilder<> builder(bitCastInst);

//   Value *lastValue;
//   for (int i = 0; i < numElems; ++i) {
//     Value *elemValue = builder.CreateExtractElement(sourceVector, i);
//     Value *zextElemValue = builder.CreateZExt(elemValue, IntegerType::get(builder.getContext(), legalizedTotalSize));

//     if (i == 0) {
//       lastValue = zextElemValue;
//     }
//     else {
//       Value *shiftedValue = builder.CreateShl(zextElemValue, i * elemSize);
//       lastValue = builder.CreateOr(lastValue, shiftedValue);
//     }
//   }

//   Value *result = builder.CreateTrunc(lastValue, IntegerType::get(builder.getContext(), totalSize));

//   bitCastInst->replaceAllUsesWith(result);
// }