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
// #include "llvm/CodeGen/ValueTypes.h"
// #include "llvm/CodeGen/SelectionDAGNodes.h"

using namespace llvm;

const int registerSize = 128;

namespace {
  // enum LegalizeTypeAction : uint8_t {
  //   TypeLegal,           // The target natively supports this type.
  //   TypePromoteInteger,  // Replace this integer with a larger one.
  //   TypeExpandInteger,   // Split this integer into two of half the size.
  //   TypeSoftenFloat,     // Convert this float to a same size integer type,
  //                        // if an operation is not supported in target HW.
  //   TypeExpandFloat,     // Split this float into two of half the size.
  //   TypeScalarizeVector, // Replace this one-element vector with its element.
  //   TypeSplitVector,     // Split this vector into two of half the size.
  //   TypeWidenVector,     // This vector should be widened into a larger vector.
  //   TypePromoteFloat     // Replace this float with a larger one.
  // };


  struct LegalizerInfo_test : public FunctionPass {
    static char ID;
    LegalizerInfo_test() : FunctionPass(ID) {}

    //*********** function declaration ************** //
    bool LegalizeTypes();
    Value * widen(Value * vector, const int numElems, const int elemSize);
    Value * promote(Value * vector, const int numElems, const int elemSize);
    bool isPowerOf2(unsigned num);
    int nextPowerOf2(unsigned num);
    void binaryOp(Value * vector1, Value * vector2, Instruction * );
    //***********  ending ********************* //

    bool runOnFunction(Function &function) override {
      errs() << "*******function name*********" << ":\n";
      errs() << function.getName() << ":\n";

      bool modified = false;

      for (auto &block : function) {
        std::vector<BinaryOperator *> binInstsToErase;

        for (auto &inst : block) {
          BinaryOperator * binaryOpInst = dyn_cast<BinaryOperator>(&inst);

          if (!inst.isBinaryOp() || inst.getOpcode() != Instruction::Add) {  // check   || 
            continue;
          }

          //*********** get Vector infomation **************//

          Value * vector1 = inst.getOperand(0);
          Value * vector2 = inst.getOperand(1);
          auto vectorType_1 = dyn_cast<VectorType>(vector1->getType());
          auto vectorType_2 = dyn_cast<VectorType>(vector2->getType());
          auto elemSize = vectorType_1->getScalarSizeInBits();
          auto numElems = vectorType_1->getNumElements();
          auto vectorSize = elemSize * numElems;

          errs() << vectorType_1 << ":\n";
          errs() << "Element Size: " << elemSize << ":\n";
          errs() << "num of element: " << numElems << ":\n";
          errs() << "vectorSize: " << vectorSize << ":\n";
          errs() << "isVectorTy: " << vectorType_1->isVectorTy() << ":\n";
          errs() << "elementtype is Int: " << vectorType_1->getVectorElementType()->isIntegerTy() << ":\n";
          errs() << "nextPowerOf2 for elementSize: " << nextPowerOf2(elemSize) << ":\n";
          errs() << "nextPowerOf2 for numElems: " << nextPowerOf2(numElems) << ":\n";
          
          if (elemSize >= 8 && isPowerOf2(elemSize) && vectorSize == registerSize){
            continue;   //there is no need to type legalization
          }

          bool modified = false;
          bool promote_modified = false;

          // step 1: widen the vector
          if (elemSize < 8 || !isPowerOf2(elemSize)){
            errs() << "We need to widen the vector: " << numElems << ":\n";
            // *******   call widen vector **************
            Value * vector1 = widen(vector1, numElems, elemSize);
            Value * vector2 = widen(vector2, numElems, elemSize);
            // check result
            auto vectorType_1 = dyn_cast<VectorType>(vector1->getType());
            errs() << "After Widen_NumElements: " << vectorType_1->getNumElements() << ":\n"; 
            errs() << "After elementsize: " << vectorType_1->getScalarSizeInBits() << ":\n";

            numElems = int(vectorType_1->getNumElements());
            modified = true;

          }

          // step 2: promote the vector
          vectorSize = numElems * elemSize;
          if (vectorSize < registerSize){
            errs() << "Weed to promote the vector " << ":\n";
            // *******   call promotion vector **************
            Value * vector_1 = promote(vector1, numElems, elemSize);
            Value * vector_2 = promote(vector2, numElems, elemSize);
            // check result
            auto vectorType_2 = dyn_cast<VectorType>(vector_1->getType());
            errs() << "After promote NumElements: " << vectorType_2->getNumElements() << ":\n"; 
            errs() << "After promote elementsize: " << vectorType_2->getScalarSizeInBits() << ":\n";
            promote_modified = true;
            modified = true;
          }


          // step3 call Binary Op
          binaryOp(vector1, vector2, binaryOpInst);


          binInstsToErase.push_back(binaryOpInst);
        }

        for (auto &instr : binInstsToErase) {
          instr->eraseFromParent();
        }
      }

      return modified;
    }
  };
}

char LegalizerInfo_test::ID = 0;

static RegisterPass<LegalizerInfo_test> X(
  "LegalizerInfo_test", "LegalizerInfo test", false, false
);



Value * LegalizerInfo_test::widen(Value * vector, const int numElems, const int elemSize){
  int widen_numElems = int(pow(2, nextPowerOf2(numElems)));
  static LLVMContext TheContext;
  static IRBuilder<> Builder(TheContext);
  Value *vector_2 = Builder.CreateZExt(vector, VectorType::get(IntegerType::get(Builder.getContext(),elemSize), widen_numElems));
  return vector_2;
}

Value * LegalizerInfo_test::promote(Value * vector, const int numElems, const int elemSize){
  int promotion_size = registerSize / numElems;
  // auto sourceVector = dyn_cast<VectorType>(vector->getType());
  static LLVMContext TheContext;
  static IRBuilder<> Builder(TheContext);
  // Vector *sourcevect = VectorType::get(IntegerType::get(Builder.getContext(),elemSize), numElems));
  Value *return_value = Builder.CreateZExt(vector, VectorType::get(IntegerType::get(Builder.getContext(),promotion_size), numElems));
  // auto elemType_2 = dyn_cast<VectorType>(return_value->getType());
  // errs() << "After promotion numElements: " << elemType_2->getNumElements() << ":\n"; 
  // errs() << "After promotion elementsize: " << elemType_2->getScalarSizeInBits() << ":\n";
  return return_value;
}

void LegalizerInfo_test::binaryOp(Value * vector1, Value * vector2, Instruction * InsertBefore=nullptr){
  static LLVMContext TheContext;
  static IRBuilder<> Builder(TheContext);
  if (InsertBefore->getOpcode() == Instruction::Add){
    Builder.CreateAdd(vector1, vector1);
  }
}


bool LegalizerInfo_test::LegalizeTypes(){
  errs() << "LEGALIZATION!!!"<< ">\n";
  return true;
}

bool LegalizerInfo_test::isPowerOf2(unsigned num) {
  while (num % 2 == 0 && num > 1) {
        num /= 2;
  }
  return num == 1;
}

int LegalizerInfo_test::nextPowerOf2(unsigned num){
  int order = ceil(log(num) / log(2));
  return order;
}

