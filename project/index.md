# SIMD Legalization and SWAR

## What is type legalization?

In the LLVM infrastructure, type legalization is the process of transforming IR code to replace all illegal types and operations with legal ones.

Types are considered legal on an architecture if values of that type are *directly* supported by a register class on that architecture and if the instruction set provides *explicit support for operations on the type*. Vector types are considered legal on an architecture when:
* the total vector size in bits is equal to the size of SIMD registers on the architecture
* the scalar size of vector elements is supported by specific SIMD operations on that architecture.

**Illegal example**
* vector type `<3 x i8>` is considered illegal on practical architectures, because they have no 24-bit registers.
* vector type `<3 x i6>` is considered illegal on practical architectures, because they have neither 18-bit registers, nor SIMD operations that support 6-bit field widths.

**Possible Methods of Legalization**
* **Scalarization:** Which won't be considered in our project, because it will severely decrease the efficience.
* **splitting:** If a vector is too big for architectural registers, *splitting* breaks up the vector into multiple shorter vectors that fit the architecture.
* **Vector Widening:** 通过widen达到right register size
* **Vector Element Promotion:** 通过element promotion达到right register size
* **Combination of Vector Widening and Vector Element Promotion:**. For a special case like a vector of type `<6 x i3>`, it could be widen and promoted to `<16 x i8>` (why not `<32 x i4>`, because it's not typically supported on most CPUs).
