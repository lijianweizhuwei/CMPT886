# Swar and Type Legalization decision model


## The Difference Between Swar and Type Legalization
We use some special vectors operation as examples to show the difference generate code after Swar or Type Legalization IR pass.

### <6 x i3> Vector Type
#### original code

```llvm
define i32 @main() {
  %1 = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1>, <i3 4, i3 3, i3 1, i3 4, i3 1, i3 2>
  ret i32 0
}
```

#### Type Legalization Code(Need widen and promote)
In the IR pass, we first widen the vector type from <6 x i3> to <8 x i3>. Then, we promote the vector from <8 x i3> to <8 x i16>. Finally, we replace the LLVM code and get the following result.

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
Ideally, we need to split the vector at first. Then we will widen and promote each vector later. Here is the generate LLVM code after IR pass. We can do that in one instruction.

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

Why we only return the first vector through Type Legalization？ The reason is that we have a problem if a variable is been used several times in a series of instructions. The following LLVM instruction is a simple example. 
The variable a is been used 2 times. However, it will be split in the first step. We cannot use the ideal type legalization method in the next line. Therefore, we only return the first vector(which means we only return the low bits data).

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
### A sequence of instructions
Here is an example of a series of llvm instruction. From each line, we need to decide whether we should generate Type Legalization、 SWAR or remain the original instruction after our IR pass.
```llvm
define i32 @main() {
  %1 = add <6 x i5> <i5 7, i5 11, i5 0, i5 12, i5 7, i5 1>,
                    <i5 13, i5 15, i5 6, i5 22, i5 7, i5 1>
  %2 = sub <4 x i3> <i3 7, i3 11, i3 0, i3 12>,
                    <i3 13, i3 15, i3 6, i3 22>
  %3 = add <8 x i16> <i16 0, i16 0, i16 3, i16 4, i16 2, i16 1, i16 0, i16 1>, 
                     <i16 0, i16 0, i16 4, i16 3, i16 1, i16 4, i16 1, i16 2>
  %4 = sub <7 x i3> <i3 7, i3 11, i3 0, i3 12, i3 7, i3 11, i3 15>,
                    <i3 13, i3 15, i3 6, i3 22, i3 11, i3 0, i3 13>
  %5 = mul <15 x i5> <i5 7, i5 11, i5 0, i5 12, i5 13, i5 15, i5 6, i5 22, i5 13, i5 15, i5 6, i5 22, i5 15, i5 6, i5 22>,
                     <i5 13, i5 15, i5 6, i5 22, i5 0, i5 12, i5 13, i5 15, i5 6, i5 15, i5 6, i5 22, i5 15, i5 13, i5 15>
  %6 = add <128 x i1> <i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1>,
                     <i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1 
, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 1, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1, i1 0, i1 1>
  %7 = mul <3 x i5> <i5 7, i5 11, i5 0>, <i5 1, i5 5, i5 6>
  %8 = sub <7 x i4> <i4 7, i4 11, i4 0, i4 12, i4 0, i4 12, i4 12>, <i4 13, i4 15, i4 6, i4 22, i4 7, i4 11, i4 0>
ret i32 0
}

```
### IR Pass for each instruction decision
```C++
  if (decision_flag == 1) {
     Swar * swar = new Swar(vector1, vector2, addInst);
     result = swar->operate();
  }else if (decision_flag == 2){
     errs() << "We are trying to legalize " <<":\n";
     Type_Legalization * type_legalization = new Type_Legalization(vector1, vector2, addInst);
     result = type_legalization->legalize();
  }
```
### How to make the decision
We use the machine learning algorithm to make decision for each input instruction. The following is a brief introduction to our current algorithm.

First, we test a bunch of instructions through Type Legalization、 SWAR and original instruction to get the training data set. When a new instruction enter, we get several high similarity instructions from the training set. Finally, we can use our algorithm to make decision for this instruction.








