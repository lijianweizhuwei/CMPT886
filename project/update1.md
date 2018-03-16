# SIMD Legalization and SWAR

## Project Flowcharts
![Alt text](../image//flowchart.png)

The picture above is a brief flowchart of our project. In our project, we must first analyze the input instructions. Then we need to determine the most appropriate process according to our cost table. In the end, We will output the comparsion between our optimize process cost and original process cost.

Therefore, we have four main areas need to be implemented in our projects. 

## Instruction Analysis and Cost table calculation
### Brief description

First, we need to identify the operation and vectors. It can help us decide if the vector need to be legalized and if SWAR instructions can be used. 

### Current Progress

### Next to do


## Type Legalization Optimize
When we read LLVM source code, we find that we can optimize this part to improve efficiency.

### Brief description
### Current Progress

### Next to do

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



