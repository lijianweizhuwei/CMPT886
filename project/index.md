# SIMD Legalization and SWAR
<<<<<<< HEAD
## Alternative SIMD Legalization Approaches

### introduction
![Alt text](./SelectionDAG.png)
LLVM uses a SelectionDAG-based instruction selector, which translates the LLVM IR code to target machine instructions.
Our project focus on ‘SelectionDAG LegalizeTypes Phase’, which in charge of converting a DAG to only use the types that are natively supported by the target.

### The ways of legalization
The type legalization phase transforms the program so that all of the calculations in the program operate on legal types, meaning that they can fit into a machine register. After type legalization, the operation legalization phase ensures that all of the operations in the program can be implemented by a single machine instruction.
Typical legalization phase has three ways to legalize vector types: Scalarization, Vector Widening and Vector Element Promotion.

#### Notation
In this article we use the notation <N × T> to describe vector types. The N variable
denotes the number of vector elements and T denotes the element type. The element
type iXX indicates an integer which is XX bits wide.

#### Scalarization
Splits the vector into multiple scalars. It is often used for the transformation from

#### Vector Widening

#### Vector Element Promotion

#### What is type legalization?

Types are considered legal on an architecture if values of that type are **directly** supported by a register class on that architecture and if the instruction set provides explicit support for operations on the type.
Vector types are considered legal on an architecture when:
* the total vector size in bits is equal to the size of SIMD registers on the architecture
* the scalar size of vector elements is supported by specific SIMD operations on that architecture.

**illegal example**
* vector type `<3 x i8>` is considered illegal on practical architectures, because they have no 24-bit registers.
* vector type `<3 x i6>` is considered illegal on practical architectures, because they have neither 18-bit registers, nor SIMD operations that support 6-bit field widths.
