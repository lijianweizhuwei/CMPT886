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
#include "Swar.h"
#include <stdlib.h>

using namespace SWAR_TL;
using namespace llvm;

#define HIGHMASK false
#define LOWMASK true

Value * Swar::operate() {

    llvm::LLVMContext & context = llvm::getGlobalContext();
    errs() << "  found binary instruction\n";

    auto vectorType = dyn_cast<VectorType>(vector1->getType());
    auto elemSize = vectorType->getScalarSizeInBits();
    auto numElems = vectorType->getNumElements();

    errs() << "    from vector <" << numElems << " x i" << elemSize << ">\n";

    auto vectorSize = elemSize * numElems;
    Type * castTy;
    if (vectorSize > 128) { // larger than 128
        errs() << "Length larger than 128\n";
        return nullptr;
    } else if (vectorSize == 128 && elemSize % 8 == 0) {
        errs() << "No need to type legalization";
        return nullptr;
    } else {
        castTy = llvm::IntegerType::get(context, vectorSize);
    }

    Value * result;
    if (inst->getOpcode() == Instruction::Add) { // addition
        Value * a1 = bitcast(vector1, inst);
        Value * a2 = bitcast(vector2, inst);

        result = swar_add(a1, a2, vectorType, inst);
    } else if (inst->getOpcode() == Instruction::Sub) {
        Value * a1 = bitcast(vector1, inst);
        Value * a2 = bitcast(vector2, inst);

        result = swar_sub(a1, a2, vectorType, inst);
    } else if (inst->getOpcode() == Instruction::Xor ||
               inst->getOpcode() == Instruction::And
            ) { // binary operation
        Value * a1 = bitcast(vector1, inst);
        Value * a2 = bitcast(vector2, inst);
        result = (Value *) BinaryOperator::Create((llvm::Instruction::BinaryOps)inst->getOpcode(), a1, a2, "", inst);
    } else {
        result = nullptr;
    }

    return result;
}


/**
 * Swar addition instructions
 */
Value * Swar::swar_add(Value * a1, Value * a2, VectorType * vectorType, Instruction * addInst) {

    LLVMContext & context = llvm::getGlobalContext();
    auto elementType = Type::getInt8Ty(context);
    auto legalType = VectorType::get(elementType, 16);
    BitCastInst * result;
    Value * intResult;

    if (a1->getType()->getScalarSizeInBits() == 1) {
        intResult = (Value *) BinaryOperator::Create(Instruction::Xor, (Value *)a1, (Value *)a2, "", addInst);
    } else {
        Value * lmask = generateMask(vectorType, LOWMASK);
        Value * hmask = generateMask(vectorType, HIGHMASK);

        BinaryOperator * m1 = BinaryOperator::Create(Instruction::And, (Value *)a1, (Value *)lmask, "", addInst);
        BinaryOperator * m2 = BinaryOperator::Create(Instruction::And, (Value *)a2, (Value *)lmask, "", addInst);
        BinaryOperator * r1 = BinaryOperator::Create(Instruction::Add, (Value *)m1, (Value *)m2, "", addInst);
        BinaryOperator * r2 = BinaryOperator::Create(Instruction::Xor, (Value *)a1, (Value *)a2, "", addInst);
        BinaryOperator * r3 = BinaryOperator::Create(Instruction::And, (Value *)r2, (Value *)hmask, "", addInst);
        intResult = (Value *) BinaryOperator::Create(Instruction::Xor, (Value *)r1, (Value *)r3, "", addInst);
    }

    if (intResult->getType()->getScalarSizeInBits() < 128) {
        ZExtInst * r4 = new ZExtInst((Value *)intResult, Type::getInt128Ty(context), "", addInst);
        result = new llvm::BitCastInst(r4, legalType, "", addInst);
    } else {
        result = new llvm::BitCastInst(intResult, legalType, "", addInst);
    }

    return (Value *)result;
}

/**
 * Swar addition instructions
 */
Value * Swar::swar_sub(Value * a1, Value * a2, VectorType * vectorType, Instruction * subInst) {

    LLVMContext & context = llvm::getGlobalContext();
    auto elementType = Type::getInt8Ty(context);
    auto legalType = VectorType::get(elementType, 16);
    BitCastInst * result;
    Value * intResult;

    if (a1->getType()->getScalarSizeInBits() == 1) {
        intResult = (Value *) BinaryOperator::Create(Instruction::Xor, (Value *)a1, (Value *)a2, "", subInst);
    } else {

        Value * lmask = generateMask(vectorType, LOWMASK);
        Value * hmask = generateMask(vectorType, HIGHMASK);

        BinaryOperator * m1 = BinaryOperator::Create(Instruction::Or, (Value *)a1, (Value *)hmask, "", subInst);
        BinaryOperator * m2 = BinaryOperator::Create(Instruction::And, (Value *)a2, (Value *)lmask, "", subInst);
        BinaryOperator * r1 = BinaryOperator::Create(Instruction::Sub, (Value *)m1, (Value *)m2, "", subInst);

        BinaryOperator * r2 = BinaryOperator::CreateNot((Value *)a2, "", subInst);
        BinaryOperator * r3 = BinaryOperator::Create(Instruction::Xor, (Value *)a1, (Value *)r2, "", subInst);
        BinaryOperator * r4 = BinaryOperator::Create(Instruction::Xor, (Value *)a2, (Value *)r3, "", subInst);

        BinaryOperator * r5 = BinaryOperator::Create(Instruction::And, (Value *)r4, (Value *)hmask, "", subInst);
        intResult = (Value *) BinaryOperator::Create(Instruction::Xor, (Value *)r1, (Value *)r5, "", subInst);
    }

    if (intResult->getType()->getScalarSizeInBits() < 128) {
        ZExtInst * r6 = new ZExtInst((Value *)intResult, Type::getInt128Ty(context), "", subInst);
        result = new llvm::BitCastInst(r6, legalType, "", subInst);
    } else {
        result = new llvm::BitCastInst(intResult, legalType, "", subInst);
    }

    return (Value *)result;
}

/**
 * Bitcast Legalization
 * Ex: <6 * i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1>
 * To: <i18> Octal: 101243 Decimal: 33443
 */
Value * Swar::bitcast(Value * vector, BinaryOperator * addInst) {
    auto vectorType = cast<VectorType>(vector->getType());
    auto elemSize = vectorType->getScalarSizeInBits();
    auto numElems = vectorType->getNumElements();
    auto totalSize = elemSize * numElems;

    unsigned int legalizedTotalSize = 8;
    while (legalizedTotalSize < totalSize) {
        legalizedTotalSize *= 2;
    }

    IRBuilder<> builder(addInst);

    Value * lastValue;
    for (unsigned int i = 0; i < numElems; ++i) {
        Value * elemValue = builder.CreateExtractElement(vector, i);
        Value * zextElemValue = builder.CreateZExt(elemValue, IntegerType::get(builder.getContext(), legalizedTotalSize));

        if (i == 0) {
            lastValue = zextElemValue;
        }
        else {
            Value * shiftedValue = builder.CreateShl(zextElemValue, i * elemSize);
            lastValue = builder.CreateOr(lastValue, shiftedValue);
        }
    }

    Value * result = builder.CreateTrunc(lastValue, IntegerType::get(builder.getContext(), legalizedTotalSize));
    return result;
}

Value * Swar::generateMask(VectorType * vectorType, bool isLow = false) {
    LLVMContext & context = llvm::getGlobalContext();
    auto elemSize = vectorType->getScalarSizeInBits();
    auto numElems = vectorType->getNumElements();
    unsigned int legalizedTotalSize = 8;
    while (legalizedTotalSize < elemSize * numElems) {
        legalizedTotalSize *= 2;
    }
    Type * castTy = llvm::IntegerType::get(context, legalizedTotalSize);

    IRBuilder<> builder(context);

    unsigned int j;
    char * ptr;
    char unit[128];
    for (j = 0; j < elemSize; j ++) {
        if (j == 0) {
            unit[j] = (isLow) ? '0' : '1';
            continue;
        }
        unit[j] = (isLow) ? '1' : '0';
    }
    unit[j] = '\0';
    uint64_t parsed = strtol(unit, &ptr, 2);
    Constant * unit_mask = llvm::ConstantInt::get(llvm::IntegerType::get(context, elemSize), parsed);
    Value * zext_unit_mask = builder.CreateZExt(unit_mask, castTy);

    Value * lastValue = llvm::UndefValue::get(castTy);
    for (j = 0; j < numElems; j ++) {
        lastValue = builder.CreateShl(lastValue, elemSize);
        lastValue = builder.CreateOr(lastValue, zext_unit_mask);
    }

    return lastValue;
}