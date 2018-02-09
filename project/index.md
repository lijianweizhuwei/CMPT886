# SIMD Legalization and SWAR

## Project Introduction

Our project aims to handle arbitrary SIMD instruction size, we plan to introduce a new method for handling arbitrary vectors with a combination of type legalization technology and the SWAR technology.
However, while LLVM's grammar allows arbitrary vectors, implementations rarely
support them.
Vectors are implemented by LLVM backends as direct uses of the processor's SIMD registers. The
SIMD registers are of fixed length, and have variable amounts of partitioning available. This means that
common vectors (containing eg i4's, i8's, i16's) are well supported. However, if you have a vector
containing, for example <12x3i>, for 36 bytes of storage, that is unlikely to fit in a SIMD register, and the
partitioning offered by the register will be illsuited
to your 3byte
values.
Our goal is to automatically convert your <12x3i> vector and operations using it into similarly efficient
code using registers to store your values, and modified operations that preserve the speed of SIMD.

## What is SIMD type legalization?

In the LLVM infrastructure, type legalization is the process of transforming IR code to replace all illegal types and operations with legal ones.

Types are considered legal on an architecture if values of that type are *directly* supported by a register class on that architecture and if the instruction set provides *explicit support for operations on the type*. Vector types are considered legal on an architecture when:
* the total vector size in bits is equal to the size of SIMD registers on the architecture
* the scalar size of vector elements is supported by specific SIMD operations on that architecture.

### Illegal example
* vector type `<3 x i8>` is considered illegal on practical architectures, because they have no 24-bit registers.
* vector type `<3 x i6>` is considered illegal on practical architectures, because they have neither 18-bit registers, nor SIMD operations that support 6-bit field widths.

### Possible Methods of Legalization
* **Scalarization:** Which won't be considered in our project, because it will severely decrease the efficience.
* **splitting:** If a vector is too big for architectural registers, *splitting* breaks up the vector into multiple shorter vectors that fit the architecture.
* **Vector Widening:** 通过widen达到right register size
* **Vector Element Promotion:** 通过element promotion达到right register size
* **Combination of Vector Widening and Vector Element Promotion:**. For a special case like a vector of type `<6 x i3>`, it could be widen and promoted to `<8 x i4>` (if the i32 sized vector is efficiently supported) or a typical `<16 x i8>` (why not `<32 x i4>`? because it's not typically supported on most CPUs).

## What is SWAR?

## Why our project want the combination of Legalization and SWAR?
**Example 1:**
For a vector of type `<25 x i5>` and register size 128-bit, if we use a type legalization technology, we can promote and wide the vector to `<32 x i8>`, the split it into two vecters of type `<16 x i8>`. This type legalization approach need two registers. But somehow the vector `<25 x i5>` can fit in one 128-bit register, which can be achieved by SWAR.

**Example 2:**
For a vector of type `<15 x i5>` and register size 128-bit, if we use a type legalization technology, we can promote and wide the vector to `<16 x i8>`, this approach needs only one register, which doesn't increase the number of register comparing to SWAR. As the SIMD vector type `<16 x i8>` is well supported by the LLVM/Parabix framework, we prefer to use the Type Legalization method.

The above examples imply that in some situations, SWAR technology works better than a Type Legalization one, such as when the register resource is a limitation (example 1). In some other situations (example 2), Type Legalization is a better choice.
