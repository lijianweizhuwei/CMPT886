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
const int split_max_num = 10;
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
    Value * binaryOp(Value * vector1, Value * vector2, Instruction * binaryOpInst);
    //***********  ending ********************* //

    bool runOnFunction(Function &function) override {
        errs() << "*******function name*********" << ":\n";
        errs() << function.getName() << ":\n";

        bool modified = false;

        for (auto &block : function) {
            std::vector<BinaryOperator *> binInstsToErase;

            for (auto &inst : block) {
                BinaryOperator * binaryOpInst = dyn_cast<BinaryOperator>(&inst);

                if (!inst.isBinaryOp()) {  // check    || inst.getOpcode() != Instruction::Add
                    // errs() << "go to continue " << ":\n";   //check whether we should go to continue or not
                    continue;
                }

                //*********** get Vector infomation **************//

                Value * vector1 = inst.getOperand(0);
                Value * vector2 = inst.getOperand(1);
                bool split_flag = false;
                bool promote_modified = false;
                bool widen_flag = false;
                bool split_flag_1 = false;
                bool split_flag_2 = false;

                for(int vec_num=1; vec_num<=2; vec_num++){
            
                    bool vec_typelegalize = true;
                    // auto vectorType;

                    Value * vector = inst.getOperand(vec_num - 1);;
                    auto vectorType = dyn_cast<VectorType>(vector->getType());


                    auto origin_elemSize = vectorType->getScalarSizeInBits();
                    auto origin_numElems = vectorType->getNumElements();
                    auto origin_vectorSize = origin_elemSize * origin_numElems;

                    errs() << vectorType << ":\n";
                    errs() << "Element Size vector " << vec_num << ": " <<origin_elemSize << ":\n";
                    errs() << "num of element vector " << vec_num << ": " << origin_numElems << ":\n";
                    errs() << "vectorSize vector " << vec_num << ": " << origin_vectorSize << ":\n";
                    errs() << "nextPowerOf2 for elementSize vector " << vec_num << ": "  << nextPowerOf2(origin_elemSize) << ":\n";
                    errs() << "nextPowerOf2 for numElems vector " << vec_num << ": "  << nextPowerOf2(origin_numElems) << ":\n";
                    errs() << "************************************"<< ":\n";

                    if(origin_elemSize >= 8 && isPowerOf2(origin_numElems) && origin_vectorSize == registerSize){
                        errs() << "there is no need to type legalization vector " << vec_num << ":\n";   //there is no need to type legalization
                        continue;
                    }


                    static LLVMContext TheContext;
                    static IRBuilder<> Builder(TheContext);

                    // step 0: split the vector
                    // if the vector need to be splitted, directrly return result in this step
                    int split_num = 0;
                    int split_vect_maxelenum = 0;
                    int split_vect_num = 0;
                    int total_size = pow(2,nextPowerOf2(origin_elemSize)) * pow(2, nextPowerOf2(origin_numElems));    // pow(2, nextPowerOf2(numElems))
                    Value * new_vector[split_max_num];

                    // check whether we need split or not
                    if (total_size > registerSize){
                        
                        if (vec_num == 1){
                            split_flag_1 = true;
                        }else{
                            split_flag_2 = true;
                        }

                        bool widen_element_flag = isPowerOf2(origin_elemSize);
                        int after_split_eleSize = pow(2, nextPowerOf2(origin_elemSize));          
                        int after_split_numElems = pow(2, nextPowerOf2(origin_numElems));
                        split_vect_maxelenum = registerSize / after_split_eleSize;
                        split_vect_num = ceil(float(origin_numElems) / split_vect_maxelenum);
                        errs() << "after_split_eleSize: " << after_split_eleSize <<":\n";
                        errs() << "after_split_numElems: " << after_split_numElems <<":\n";
                        errs() << "split_vect_maxelenum: " << split_vect_maxelenum <<":\n";
                        errs() << "split_vect_num: " << split_vect_num <<":\n";
                        Type* split_elemunTy = IntegerType::get(Builder.getContext(),after_split_eleSize);  //after_split_eleSize
                        Type* split_vecTy = VectorType::get(split_elemunTy, split_vect_maxelenum);   //split_vect_maxelenum
                        Value *elemValue;

                        //create split vector array
                        for(int i = 0; i < split_vect_num; ++i){
                            new_vector[i] = UndefValue::get(split_vecTy);
                            auto vectorType_i = dyn_cast<VectorType>(new_vector[i]->getType());  // auto vectorType_2 = dyn_cast<VectorType>(vector2->getType());
                            auto split_eleSize_i = vectorType_i->getScalarSizeInBits();
                            auto split_numElems_i = vectorType_i->getNumElements();
                            auto split_vectorSize_i = split_eleSize_i * split_numElems_i;
                            errs() << "*******************************************" << ":\n"; 
                            errs() << "after_split_eleSize: " << split_eleSize_i << ":\n"; 
                            errs() << "after_split_numElems: " << split_numElems_i << ":\n";
                            errs() << "after_split_vectorSize: " << split_vectorSize_i << ":\n"; 
                            errs() << "return: " << ":\n";
                            errs() << "*******************************************" << ":\n";
                            for (int j = 0; j < split_vect_maxelenum; ++j){
                                // errs() << "break_num: " << (j + i * (split_vect_maxelenum)) <<":\n";      // chenck break status
                                if((j + i * (split_vect_maxelenum)) == origin_numElems){
                                    break;
                                }

                                // insert value for the split vector
                                elemValue = Builder.CreateExtractElement(vector, j + i * (split_vect_maxelenum));
                                if (!widen_element_flag){   // check whether need widen each element
                                // errs() << "we should widen the element: " << origin_elemSize << ":\n";
                                    elemValue = Builder.CreateZExt(elemValue, IntegerType::get(Builder.getContext(), after_split_eleSize));
                                }
                
                                new_vector[i] = Builder.CreateInsertElement(new_vector[i], elemValue, j);
                            }

                        }
                    }




                    // step 1: widen the vector
                    int numElems = origin_numElems;
                    int elemSize = origin_elemSize;
                    if (split_flag == false && (origin_numElems < 8 || !isPowerOf2(origin_numElems))){
                        vector = widen(vector, origin_numElems, origin_elemSize);
                        auto widen_vectorType = dyn_cast<VectorType>(vector->getType());
                        
                        errs() << "After Widen_NumElements vector: " << widen_vectorType->getNumElements() << ":\n"; 
                        errs() << "After elementsize vector: " << widen_vectorType->getScalarSizeInBits() << ":\n";
                        numElems = int(widen_vectorType->getNumElements());
                    }


                    // step 2: promote the vector
                    int vectorSize;
                    if (widen_flag == false){
                        vectorSize = origin_numElems * origin_elemSize;
                    }else{
                        vectorSize = numElems * origin_elemSize;
                    }
          
                    if ((split_flag == false) && (vectorSize < registerSize)){
                        errs() << "Weed to promote the vector " << ":\n";
                        // *******   call promotion vector **************
                        vector = promote(vector, numElems, elemSize);
                        auto promote_vectorType = dyn_cast<VectorType>(vector->getType());
                        
                        // check result             
                        errs() << "After promote NumElements: " << promote_vectorType->getNumElements() << ":\n"; 
                        errs() << "After promote elementsize: " << promote_vectorType->getScalarSizeInBits() << ":\n";
                        promote_modified = true;
                        modified = true;
                    }


                    if (vec_num == 1){
                        if (split_flag_1 == false){
                            vector1 = vector;
                        }else{
                            vector1 = new_vector[0];
                        }
                    }
                        
                    if (vec_num == 2){
                        if (split_flag_2 == false){
                            vector2 = vector;
                        }else{
                            vector2 = new_vector[0];
                        }
                    }


                }


                Value *result;
                // step3 call Binary Op
                if (split_flag_1 == true && split_flag_2 == true){
                    result = binaryOp(vector1, vector2, binaryOpInst);
                }else if (promote_modified == true){
                    result = binaryOp(vector1, vector2, binaryOpInst);
                }else{
                    result = binaryOp(vector1, vector2, binaryOpInst);
                }

                auto vectorType_1 = dyn_cast<VectorType>(result->getType());
                errs() << "Element Size: " << vectorType_1->getScalarSizeInBits() << ":\n";
                errs() << "num of element: " << vectorType_1->getNumElements() << ":\n"; 

                binInstsToErase.push_back(binaryOpInst);
                binaryOpInst->replaceAllUsesWith(result);
          
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

    //*** version1 ***
    // errs() << "numElems " << numElems << ":\n";
    // errs() << "Widen Vector " << widen_numElems << ":\n";
    // Value *vector_2 = Builder.CreateZExt(vector, VectorType::get(IntegerType::get(Builder.getContext(),elemSize), widen_numElems));
    // errs() << "Widen Vector " << ":\n";


    //*** version2 ***
    Type* elemunTy = IntegerType::get(Builder.getContext(),elemSize);
    Type* vecTy = VectorType::get(elemunTy, widen_numElems);
    Value* widen_vec = UndefValue::get(vecTy);

    for (int i = 0; i < numElems; ++i){
        Value *elemValue = Builder.CreateExtractElement(vector, i);
        widen_vec = Builder.CreateInsertElement(widen_vec, elemValue, i);   //BinaryOperator::Create(Instruction::And, (Value *)a1, (Value *)lmask, "", addInst);
    }

    return widen_vec;
}

Value * LegalizerInfo_test::promote(Value * vector, const int numElems, const int elemSize){
    int promotion_size = registerSize / numElems;
    static LLVMContext TheContext;
    static IRBuilder<> Builder(TheContext);
    Value *return_value = Builder.CreateZExt(vector, VectorType::get(IntegerType::get(Builder.getContext(),promotion_size), numElems));   //promote Vector

    return return_value;
}

Value * LegalizerInfo_test::binaryOp(Value * vector1, Value * vector2, Instruction * binaryOpInst=nullptr){
    static LLVMContext TheContext;
    static IRBuilder<> Builder(TheContext);
    Value *return_value;
    BinaryOperator * instr;

    switch(binaryOpInst->getOpcode()){
        case Instruction::Add:
            return_value = Builder.CreateAdd(vector1, vector2);   //BinaryOperator * m1 = BinaryOperator::Create(Instruction::And, (Value *)a1, (Value *)lmask, "", addInst);
            instr = BinaryOperator::Create(Instruction::Add, vector1, vector2, "", binaryOpInst);
            break;

        case Instruction::Sub:
            return_value = Builder.CreateSub(vector1, vector2);
            instr = BinaryOperator::Create(Instruction::Sub, vector1, vector2, "", binaryOpInst);
            break;

        case Instruction::Mul:
            return_value = Builder.CreateMul(vector1, vector2);
            instr = BinaryOperator::Create(Instruction::Mul, vector1, vector2, "", binaryOpInst);
            break;

        default :
            return_value = Builder.CreateAdd(vector1, vector2);   // 默认暂时是Add
            instr = BinaryOperator::Create(Instruction::Add, vector1, vector2, "", binaryOpInst);
            break;
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

