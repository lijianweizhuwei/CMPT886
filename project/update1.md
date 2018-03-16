# SIMD Legalization and SWAR

## Project Flowcharts
![Alt text](../image//flowchart.png)

The picture above is a brief flowchart of our project which is far away from our original idea. In our project, we must first analyze the input instructions. Then we need to determine the most appropriate process according to our cost table. In the end, We will output the comparsion between our optimize process cost and original process cost.

Therefore, we have four main areas need to be implemented in our projects. 

## Instruction Analysis and Cost table calculation
### Brief description

First, we need to identify the operation and vectors. It can help us decide if the vector need to be legalized and if SWAR instructions can be used. 

### Current Progress
Currently, we have identified each arithmetic operation. The SWAR operation still being developed. The following code is a simple example to identify "Add" operation.
``` c
if (!inst.isBinaryOp() || inst.getOpcode() != Instruction::Add) {
                        Operation;
                    }
```

### Next to do
1. Optimize the identify process and implement interface for Cost table calcuation.
2. Implement the SWAR operation. At lease, we need to achieve basic arithmetic operation.

## Type Legalization Optimize

### Brief description
When we read LLVM source code, we find that we can optimize this part to improve efficiency.

### Current Progress
We have read LegalizerInfo.cpp and related files from llvm source code. We are trying to override the logic for type legalization to improve efficiency and support more vector types.



``` c
 switch (Action) {
  case Legal:
  case Lower:
  case Libcall:
  case Custom:
    return {Size, Action};
  case FewerElements:
    if (Vec == SizeAndActionsVec({{1, FewerElements}}))
      return {1, FewerElements};
    LLVM_FALLTHROUGH;
  case NarrowScalar: {
    for (int i = VecIdx - 1; i >= 0; --i)
      if (!needsLegalizingToDifferentSize(Vec[i].second) &&
          Vec[i].second != Unsupported)
        return {Vec[i].first, Action};
    llvm_unreachable("");
  }
  case WidenScalar:
  case MoreElements: {
    for (std::size_t i = VecIdx + 1; i < Vec.size(); ++i)
      if (!needsLegalizingToDifferentSize(Vec[i].second) &&
          Vec[i].second != Unsupported)
        return {Vec[i].first, Action};
    llvm_unreachable("");
  }
  case Unsupported:
    return {Size, Unsupported};
  case NotFound:
  case UseLegacyRules:
    llvm_unreachable("NotFound");
  }
  llvm_unreachable("Action has an unknown enum value");
}

``` 
### Next to do

## SWAR
### Brief description

### Current Progress
1) Recognize the arithmetic instruction
2) Extract operands (i.e. 2 vectors) from the instruction
3) Change vectors into a single vector
4) Generate a mask for operating instruction
5) Operate AND, XOR, ADD instructions
6) Reverse the single vector to the original vector type


### Next to do




## Reference
1. [Hybrid Type Legalization for a Sparse SIMD Instruction Set](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/hybird.pdf)
2. [Sierra: A SIMD Extension for C++](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/Sierra.pdf)
3. [Auto-vectorization for Image Processing DSLs](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/DSLs.pdf)
4. [Systematic Support of Parallel Bit Streams in LLVM](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/Systematic.pdf)
5. [MoviCompile : An LLVM based compiler for heterogeneous SIMD code generation](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/MoviCompile.pdf)
6. [Compiling For SIMD Within A Register](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/Compiling.pdf)
7. [SIMD Within A Register (e.g., using MMX)](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/MoviCompile.pdf)
8. [LLVM SIMDtoSWAR Pass](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/LLVM.pdf)
9. [SWAR Support for LLVM](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/SWAR_S.pdf)
10. [General-Purpose SIMD Within A Register: Parallel Processing on Consumer Microprocessors](https://github.com/lijianweizhuwei/CMPT886/blob/master/file/swar.pdf)
11. [LLVM Code Generator](http://llvm.org/docs/CodeGenerator.html#selectiondag-legalizetypes-phase)


