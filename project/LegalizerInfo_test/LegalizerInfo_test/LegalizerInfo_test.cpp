//===- lib/CodeGen/GlobalISel/LegalizerInfo.cpp - Legalizer ---------------===//
//
//
//===----------------------------------------------------------------------===//
//
// Implement an interface to specify and query how an illegal operation on a
// given type should be expanded.
//
// Issues to be resolved:
//   + Make it fast.
//   + Support weird types like i3, <7 x i3>, ...
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
    // static LLVMContext TheContext;
    // static IRBuilder<> Builder(TheContext);
    LegalizerInfo_test() : FunctionPass(ID) {}

    //*********** function declaration ************** //
    bool LegalizeTypes();
    Value * widen(Value * vector, const int numElems, const int elemSize);
    Value * promote(Value * vector, const int numElems, const int elemSize);
    bool isPowerOf2(unsigned num);
    int nextPowerOf2(unsigned num);
    Value * binaryOp(Value * vector1, Value * vector2, Instruction * );
    //***********  ending ********************* //

    bool runOnFunction(Function &function) override {
      errs() << "*******function name*********" << ":\n";
      errs() << function.getName() << ":\n";

      bool modified = false;

      for (auto &block : function) {
        std::vector<BinaryOperator *> binInstsToErase;

        for (auto &inst : block) {
          BinaryOperator * binaryOpInst = dyn_cast<BinaryOperator>(&inst);

          if ((!inst.isBinaryOp()) || inst.getOpcode() != Instruction::Add) {  // check   || 
            errs() << "go to continue " << ":\n";
            continue;
          }

          //*********** get Vector infomation **************//

          Value * vector1 = inst.getOperand(0);
          Value * vector2 = inst.getOperand(1);
          auto vectorType_1 = dyn_cast<VectorType>(vector1->getType());
          auto vectorType_2 = dyn_cast<VectorType>(vector2->getType());
          auto origin_elemSize = vectorType_1->getScalarSizeInBits();
          auto origin_numElems = vectorType_1->getNumElements();
          auto origin_vectorSize = origin_elemSize * origin_numElems;

          errs() << vectorType_1 << ":\n";
          errs() << "Element Size: " << llvm::VectorType::getInteger(vectorType_1) << ":\n";
          errs() << "Element Size: " << origin_elemSize << ":\n";
          errs() << "num of element: " << origin_numElems << ":\n";
          errs() << "vectorSize: " << origin_vectorSize << ":\n";
          errs() << "isVectorTy: " << vectorType_1->isVectorTy() << ":\n";
          errs() << "elementtype is Int: " << vectorType_1->getVectorElementType()->isIntegerTy() << ":\n";
          errs() << "nextPowerOf2 for elementSize: " << nextPowerOf2(origin_elemSize) << ":\n";
          errs() << "nextPowerOf2 for numElems: " << nextPowerOf2(origin_numElems) << ":\n";
          
          if (origin_elemSize >= 8 && isPowerOf2(origin_elemSize) && origin_vectorSize == registerSize){
            continue;   //there is no need to type legalization
          }

          bool modified = false;
          bool promote_modified = false;
          bool widen_flag = false;

          // step 1: widen the vector
          int numElems = origin_numElems;
          int elemSize = origin_elemSize;
          if (numElems < 8 || !isPowerOf2(numElems)){
            errs() << "We need to widen the vector: " << numElems << ":\n";
            // *******   call widen vector **************
            vector1 = widen(vector1, origin_numElems, origin_elemSize);
            vector2 = widen(vector2, origin_numElems, origin_elemSize);
            // check result
            auto vectorType_1 = dyn_cast<VectorType>(vector1->getType());
            errs() << "After Widen_NumElements: " << vectorType_1->getNumElements() << ":\n"; 
            errs() << "After elementsize: " << vectorType_1->getScalarSizeInBits() << ":\n";

            numElems = int(vectorType_1->getNumElements());
            modified = true;
            widen_flag = true;
          }

          // step 2: promote the vector
          int vectorSize;
          if (widen_flag == false){
            vectorSize = origin_numElems * origin_elemSize;
          }else{
            vectorSize = numElems * origin_elemSize;
          }
          
          if (vectorSize < registerSize){
            errs() << "Weed to promote the vector " << ":\n";
            // *******   call promotion vector **************
            vector1 = promote(vector1, numElems, elemSize);
            vector2 = promote(vector2, numElems, elemSize);
            // check result
            auto vectorType_2 = dyn_cast<VectorType>(vector1->getType());
            errs() << "After promote NumElements: " << vectorType_2->getNumElements() << ":\n"; 
            errs() << "After promote elementsize: " << vectorType_2->getScalarSizeInBits() << ":\n";
            promote_modified = true;
            modified = true;
          }


          // step3 call Binary Op
          if (promote_modified == true){
            Value *result = binaryOp(vector1, vector2, binaryOpInst);
            auto vectorType_1 = dyn_cast<VectorType>(result->getType());
            errs() << "Element Size: " << vectorType_1->getScalarSizeInBits() << ":\n";
            errs() << "num of element: " << vectorType_1->getNumElements() << ":\n";

            binaryOpInst->replaceAllUsesWith(result);
          }



          

          // if (promote_modified == true){
          //   static LLVMContext TheContext;
          //   static IRBuilder<> Builder(TheContext);
          //   vector1 = builder.CreateShl(vector, (widen_numElems - origin_numElems) * elemSize);
          // }

          // binaryOpInst->replaceAllUsesWith(result);
          binInstsToErase.push_back(binaryOpInst);
        }
        errs() << "skip " << ":\n";
        for (auto &instr : binInstsToErase) {
          errs() << "After promote NumElements: " << ":\n";
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

  // auto vectorType_1 = dyn_cast<VectorType>(vector->getType());
  // auto origin_elemSize = vectorType_1->getScalarSizeInBits();
  // auto origin_numElems = vectorType_1->getNumElements();
  // errs() << "Element Size vector: " << origin_elemSize << ":\n";
  // errs() << "num of element vector: " << origin_numElems << ":\n";

  //*** version1 ***
  // errs() << "numElems " << numElems << ":\n";
  // errs() << "Widen Vector " << widen_numElems << ":\n";
  // Value *vector_2 = Builder.CreateZExt(vector, VectorType::get(IntegerType::get(Builder.getContext(),elemSize), widen_numElems));
  // errs() << "Widen Vector " << ":\n";


  //*** version2 ***
  Type* elemunTy = IntegerType::get(Builder.getContext(),numElems);
  Type* vecTy = VectorType::get(elemunTy, widen_numElems);
  Value* widen_vec = UndefValue::get(vecTy);
  auto vectorType_3 = dyn_cast<VectorType>(widen_vec->getType());
  errs() << "33333" << ":\n";
  auto origin_elemSize_ = vectorType_3->getScalarSizeInBits();
  errs() << "4" << ":\n";
  auto origin_numElems_ = vectorType_3->getNumElements();
  errs() << "Element Size: " << origin_elemSize_ << ":\n";
  errs() << "num of element: " << origin_numElems_ << ":\n";

  // errs() << "after createVector " << ":\n";
  for (int i = 0; i < numElems; ++i){
    Value *elemValue = Builder.CreateExtractElement(vector, i);
    errs() << "extract element" << ":\n";
    widen_vec = Builder.CreateInsertElement(widen_vec, elemValue, i);
  }
  // auto Elt1 = Builder.getInt8(0);
  // errs() << "InsertVector " << ":\n";
  // vector = Builder.CreateInsertElement(vector, Elt1, Builder.getInt8(0));

  // errs() << "InsertVector2 " << ":\n";
  // vector = Builder.CreateInsertElement(vector, Elt1, Builder.getInt8(1));
  return widen_vec;
}

Value * LegalizerInfo_test::promote(Value * vector, const int numElems, const int elemSize){
  int promotion_size = registerSize / numElems;
  // auto sourceVector = dyn_cast<VectorType>(vector->getType());
  static LLVMContext TheContext;
  static IRBuilder<> Builder(TheContext);
  Value *return_value = Builder.CreateZExt(vector, VectorType::get(IntegerType::get(Builder.getContext(),promotion_size), numElems));

  return return_value;
}

Value * LegalizerInfo_test::binaryOp(Value * vector1, Value * vector2, Instruction * InsertBefore=nullptr){
  static LLVMContext TheContext;
  static IRBuilder<> Builder(TheContext);
  Value *return_value;

  if (InsertBefore->getOpcode() == Instruction::Add){
    // errs() << "We are trying to add vector" << ":\n";
    return_value = Builder.CreateAdd(vector1, vector2);
    // Value *vector_1 = Builder.CreateExtractElement(return_value, 1);
    // errs() << "We are trying to add vector"<<":\n" << *vector_1 <<":\n";
    // BinaryOperator * r1 = BinaryOperator::Create(Instruction::Add, vector1, vector2, "", InsertBefore);   
  }
  return return_value;
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

