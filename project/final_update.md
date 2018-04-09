# Swar and Type Legalization decision model


## The Difference Between Swar and Type Legalization
We use some special vectors operation as example.

### <6 x i3> Vector Type
#### original code

```llvm
define i32 @main() {
  %1 = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1>, <i3 4, i3 3, i3 1, i3 4, i3 1, i3 2>
  ret i32 0
}
```

#### Type Legalization Code(Need widen and promote)
In the IR pass, we first widen the vector type from <6 x i3> to <8 x i3>. Then, we promote the vector from <8 x i3> to <8 x i16>. Finally, we replace the llvm code and get the following result.

```llvm

define i32 @main() {
  %a = add <8 x i16> <i16 0, i16 0, i16 3, i16 4, i16 2, i16 1, i16 0, i16 1>, 
                     <i16 0, i16 0, i16 4, i16 3, i16 1, i16 4, i16 1, i16 2>
  ret i32 0
}

```
#### Swar Code


```llvm

define i32 @main() {
  %1 = and i32 131747, 112347
  %2 = and i32 108836, 112347
  %3 = add i32 %1, %2
  %4 = xor i32 131747, 108836
  %5 = and i32 %4, 149796
  %6 = xor i32 %3, %5
  %7 = zext i32 %6 to i128
  %8 = bitcast i128 %7 to <16 x i8>
  ret i32 0  
}

```
### <5 x i25> Vector Type
#### original code

```llvm
define i32 @main() {
  %a = add <5 x i25> <i25 3, i25 7, i25 11, i25 0, i25 12>, 
                     <i25 4, i25 13, i25 15, i25 6, i25 22>
  ret i32 0
}
```
#### Type Legalization Code(Ideal)
Ideally, we need to split the vector at first. Then we will widen and promote each vector later. Here is the generate llvm code after IR pass. We can do that in one instruction.

```llvm
define i32 @main() {
  %a0 = add <4 x i32> <i32 7, i32 11, i32 0, i32 12>,
                      <i32 13, i32 15, i32 6, i32 22>
  %a1 = add <4 x i32> <i32 0, i32 0, i32 0, i32 3>,
                      <i32 0, i32 0, i32 0, i32 4>
ret i32 0
}
```
#### Type Legalization Code(Actual)
```llvm
define i32 @main() {
  %a = add <4 x i32> <i32 7, i32 11, i32 0, i32 12>,
                     <i32 13, i32 15, i32 6, i32 22>
ret i32 0
}
```

Why we only return the first vector in Type Legalization. The reason is that we have a problem if a variable is called multiple times in a series of instructions. The following llvm instruction is a simple example. The variable a is been used 2 times. However, it will be splitted in the first step. We cannot use the ideal type legalization method in the next line. Therefore, we only return the first vector(which means we only return the low bits data).

### Simple example
```llvm
define i32 @main() {
  %a = add <5 x i25> <i25 3, i25 7, i25 11, i25 0, i25 12>, 
                     <i25 4, i25 13, i25 15, i25 6, i25 22>
  %b = add <5 x i25> %a, <i25 3, i25 2, i25 17, i25 4, i25 7>                 
  ret i32 0
}
```

### <128 x i1> Vector Type
#### original code

```llvm
define i32 @main() {
  %a = add <128 x i1> <i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1>,
  
                     <i1 0, i1 0, i1 1, i1 0, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 0, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 0, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 0, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 0, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 1, i1 1>

  ret i32 0
}
```

#### Type Legalization code
```llvm
define i32 @main() {
  %a = add <16 x i8> < i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1>, 
                      <i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 1, i1 1>
                      
  ret i32 0
}
```

#### SWAR code
```llvm
define i32 @main() {
  %a = XOR <128 x i1> <i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1>,
  
                     <i1 0, i1 0, i1 1, i1 0, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 0, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 0, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 0, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 0, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 1, i1 1>

  ret i32 0
}
```
## The Decision model in a series of instructions
### 






