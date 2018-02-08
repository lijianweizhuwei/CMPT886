# SIMD Legalization and SWAR

#### What is type legalization?

Types are considered legal on an architecture if values of that type are **directly** supported by a register class on that architecture and if the instruction set provides explicit support for operations on the type.
Vector types are considered legal on an architecture when:
* the total vector size in bits is equal to the size of SIMD registers on the architecture
* the scalar size of vector elements is supported by specific SIMD operations on that architecture.

**illegal example**
* vector type `<3 x i8>` is considered illegal on practical architectures, because they have no 24-bit registers.
* vector type `<3 x i6>` is considered illegal on practical architectures, because they have neither 18-bit registers, nor SIMD operations that support 6-bit field widths.
