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
  enum LegalizeTypeAction : uint8_t {
    TypeLegal,           // The target natively supports this type.
    TypePromoteInteger,  // Replace this integer with a larger one.
    TypeExpandInteger,   // Split this integer into two of half the size.
    TypeSoftenFloat,     // Convert this float to a same size integer type,
                         // if an operation is not supported in target HW.
    TypeExpandFloat,     // Split this float into two of half the size.
    TypeScalarizeVector, // Replace this one-element vector with its element.
    TypeSplitVector,     // Split this vector into two of half the size.
    TypeWidenVector,     // This vector should be widened into a larger vector.
    TypePromoteFloat     // Replace this float with a larger one.
  };


  struct LegalizerInfo_test : public FunctionPass {
    static char ID;
    LegalizerInfo_test() : FunctionPass(ID) {}
    bool LegalizeTypes();
    // bool LegalizeVectors(VectorType *vector);
    Value * widen(Value * vector, const int numElems, const int elemSize);
    Value * promote(Value * vector, const int numElems, const int elemSize);
    bool isPowerOf2(unsigned num) {
      while (num % 2 == 0 && num > 1) {
        num /= 2;
      }

      return num == 1;
    }

    int nextPowerOf2(unsigned num){
      int order = ceil(log(num) / log(2));
      return order;
    }

    bool runOnFunction(Function &function) override {
      errs() << "*******function name*********" << ":\n";
      errs() << function.getName() << ":\n";

      bool modified = false;

      for (auto &block : function) {
        // std::vector<BitCastInst *> bitCastInstsToErase;

        for (auto &inst : block) {
          // auto bitCastInst = dyn_cast<BitCastInst>(&inst);

          if (!inst.isBinaryOp() || inst.getOpcode() != Instruction::Add) {  // check
            continue;
          }

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


          bool change = false;

          // step 1: widen the vector
          if (elemSize < 8 || !isPowerOf2(elemSize)){
            // *call widen vector
            Value * widen_v1 = widen(vector1, numElems, elemSize);
            Value * widen_v2 = widen(vector2, numElems, elemSize);
            // check result
            auto vectorType_1 = dyn_cast<VectorType>(widen_v1->getType());
            numElems = vectorType_1->getNumElements();
            elemSize = vectorType_1->getScalarSizeInBits();
            errs() << "After Widen_NumElements: " << numElems << ":\n"; 
            errs() << "After elementsize: " << elemSize << ":\n";
          }

          // step 2: promote the vector
          vectorSize = numElems * elemSize;
          if (vectorSize < registerSize){
            Value * promote_v1 = promote(vector1, numElems, elemSize);
            auto vectorType_1 = dyn_cast<VectorType>(promote_v1->getType());
            numElems = vectorType_1->getNumElements();
            elemSize = vectorType_1->getScalarSizeInBits();
            errs() << "After Widen_NumElements: " << numElems << ":\n"; 
            errs() << "After elementsize: " << elemSize << ":\n";
          } 




          // if (vectorSize < 128 && !isPowerOf2(elemSize)){
          //   IntTy = llvm::IntegerType::getInt128Ty(inst.getContext());
          //   uint64_t high_part = legaliz_h_part(vectorType);
          //   uint64_t low_part = legaliz_l_part(vectorType);
          // }





          // VectorType* vector_1 = VectorType(vectorType->getVectorElementType(), numElems); 
          // llvm::EVT ResultVT = EVT::getVectorVT(IntTy, MVT::i32, 4, /*Scalable=*/true);
          // llvm::EVT ResultVT = inst.getValueType();
          // errs() << vector1->getType() << ":\n";   //EVT ResultVT = 
          // errs() << "*******inst.getOpcode()*********" << ":\n";
          // errs() << inst.getOpcode()<< ">\n";
          // errs() << "*******inst.isBinaryOp()*********" << ":\n";
          // errs() << inst.isBinaryOp()<< ">\n";
          // errs() << "*******getOpcodeNamen*************\n";
          // errs() << llvm::AddrSpaceCastInst::getOpcodeName(add_operation->getOpcode()) << ">\n";
          // errs() << "*************************************" << ">\n";

          // auto sourceVectorType =
          //     dyn_cast<VectorType>(add_operation->getOperand(0)->getType());
          // if (!sourceVectorType) {
          //   continue;
          // }
          // auto elemSize = sourceVectorType->getScalarSizeInBits();
          // auto numElems = sourceVectorType->getNumElements();

          // errs() << "Whether it is a valid elementtype" << ">\n";
          // errs() << VectorType::isValidElementType(sourceVectorType)<< ">\n";
          // errs() << "Whether it is a vector type" << ">\n";
          // errs() << sourceVectorType->isVectorTy()<< ">\n";
          // errs() << "get the element type" << ">\n";
          // errs() << sourceVectorType->getVectorElementType()<< ">\n";
          // errs() << add_operation->getOperand(0)->getType()->isVectorTy()<< ">\n";
          // errs() << add_operation->getOperand(1)->getType()->isVectorTy()<< ">\n";
          // errs() << "    from vector <" << numElems << " x i" << elemSize
          //        << ">\n";

          // SelectionDAG::LegalizeVectors()
          // if (!bitCastInst->getType()->isIntegerTy()) {
          //   continue;
          // }
          // auto width = add_operation->getType()->getPrimitiveSizeInBits();
          // errs() << "    to integer i" << width << "\n";


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


// uint64_t LegalizerInfo_test::widen_part(VectorType * vectorType) {

// }

Value * LegalizerInfo_test::widen(Value * vector, const int numElems, const int elemSize){
  errs() << "We need to widen the vector: " << numElems << ":\n";
  int widen_numElems = int(pow(2, nextPowerOf2(numElems)));
  errs() << "Widen_NumElement is: " << widen_numElems << ":\n";
  static LLVMContext TheContext;
  static IRBuilder<> Builder(TheContext);
  Value *vector_2 = Builder.CreateZExt(vector, VectorType::get(IntegerType::get(Builder.getContext(),elemSize), widen_numElems));
  return vector_2;
}

Value * LegalizerInfo_test::promote(Value * vector, const int numElems, const int elemSize){
  errs() << "Weed to promote the vector " << ":\n";
  int promotion_size = registerSize / numElems;
  errs() << "promotion_size: "<< promotion_size << ":\n";
  // auto sourceVector = dyn_cast<VectorType>(vector->getType());
  static LLVMContext TheContext;
  static IRBuilder<> Builder(TheContext);
  // Vector *sourcevect = VectorType::get(IntegerType::get(Builder.getContext(),elemSize), numElems));
  Value *lastValue;
  for( int i = 0; i < numElems; ++i){ 
    Value *elemValue = Builder.CreateExtractElement(vector, i);
    Value *zextElemValue = Builder.CreateZExt(vector, IntegerType::get(Builder.getContext(), promotion_size));
  
    if (i == 0) {
      lastValue = zextElemValue;
    }
    else {
      Value *shiftedValue = Builder.CreateShl(zextElemValue, i * elemSize);
      lastValue = Builder.CreateOr(lastValue, shiftedValue);
    }
  }
  Value *result = Builder.CreateTrunc(lastValue, IntegerType::get(Builder.getContext(), registerSize));
  
  return result;
}


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