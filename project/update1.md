# SIMD Legalization and SWAR

## Project Flowcharts
![Alt text](../image//flowchart.png)

The picture above is a brief flowchart of our project which is far away from our original idea. In our project, we must first analyze the input instructions. Then we need to determine the most appropriate process according to our cost table. In the end, We will output the comparsion between our optimize process cost and original process cost.

Therefore, we have four main areas need to be implemented in our projects. 

## Instruction Analysis and Cost table calculation
### Brief description

First, we need to identify the operation and vectors. It can help us decide if the vector need to be legalized and if SWAR instructions can be used. 

### Current Progress

### Next to do


## Type Legalization Optimize

### Brief description
When we read LLVM source code, we find that we can optimize this part to improve efficiency.

### Current Progress
We have done part of Legalization.cpp and related files based on llvm source code. We are trying to override the logic for type legalization to improve efficiency and support more vector types.


``` 
LegalizerInfo::findAction(const SizeAndActionsVec &Vec, const uint32_t Size) {
  
  ...
  
  //VecIdx stands for the current element size
  //action stands for the legalization operation need to be done to this element type
  
  int VecIdx = VecIt - Vec.begin();
  LegalizeAction Action = Vec[VecIdx].action;
  
  switch (Action) {
  
  case Legal:
  // Legal means this type can be supported directly or with some libraries.
  return {Size, Action};
  
  case Promotion:
  case Widen: {
    for (std::size_t i = VecIdx + 1; i < Vec.size(); ++i)
      if (!needsLegalizingToDifferentSize(Vec[i].action) && Vec[i].action != Unsupported)
        return {Vec[i].size, widen};
    llvm_unreachable("");
  }
  
  case Narrow:
  // Only the scalar type can be supported by now
    if (Vec == SizeAndActionsVec({{1, Narrow}}))
      return {1, Narrow};
    llvm_unreachable("");
  
  case Demotion: {
    for (int i = VecIdx - 1; i >= 0; --i)
      if (!needsLegalizingToDifferentSize(Vec[i].action) && Vec[i].action != Unsupported)
        return {Vec[i].size, Demotion};
    llvm_unreachable("");
  }
  
  case Unsupported:
    return {Size, Unsupported};
  
  }
  llvm_unreachable("Action has an unknown enum value");
}

``` 
### Next to do
The current implementation has the following problems:
1. Can't support oddball types like <5 x i3>, <25 x i5> ... The reason is that the Promotion action and the Widen action  have the same implementation to get the legal element number, which means the Promotion action is only part done. Actually 

## SWAR
### Brief description

### Current Progress

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



