# SWAR

#### Addition Algorithm
```
SWAR add z = x + y

z = ((x &~H) + (y &~H)) ^ ((x ^ y) & H)
```

```llvm
define i32 @main() {
  ; Original code
  %a = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1>, <i3 4, i3 4, i3 4, i3 4, i3 1, i3 2>
  ret i32 0
}
```

```llvm
define i32 @main() {
  ; After optimizatioin
  %v1 = bitcast <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1> to i18
  %v2 = bitcast <6 x i3> <i3 4, i3 4, i3 4, i3 4, i3 1, i3 2> to i18

  %low_v1 = and i18 %v1, 112347 ;get low bits for every field
  %low_v2 = and i18 %v2, 112347
  ;decimal 112347 = binary 011 011 011 011 011 011
  ;i18 %v1:     < 3   4   2   1   0   1 >
  ;AND: %v1     <011 100 010 001 000 001> & 
  ;	    112347  <011 011 011 011 011 011>
  ;i18 %low_v1: <011 000 010 001 000 001>
  ;i18 %low_v2: <000 000 000 000 001 010>
  %low_add = add i18 %low_v1, %low_v2 ; get the result of addition of two vectors' low bits.
  ;i18 %low_add: <011 000 010 001 001 011>
  %overflow = xor i18 %v1, %v2
  ;i18 %v1: <011 100 010 001 000 001>
  ;i18 %v2: <100 100 100 100 001 010>
  ;XOR i18: <111 000 110 101 001 011>
  %high_add = and i18 %overflow, 149796
  ;decimal 149796 = binary 100 100 100 100 100 100
  ;AND: %overflow: <111 000 110 101 001 011> &
  ;        149796: <100 100 100 100 100 100>
  ;i18  %high_add: <100 000 100 100 000 000>
  %result = xor i18 %low_add, %high_add
  ;XOR:  %low_add: <011 000 010 001 001 011> or
  ;     %high_add: <100 000 100 100 000 000>
  ;       %result: <111 000 110 101 001 011>
  ;                < 3   4   2   1   0   1 >
  ;                < 4   4   4   4   1   2 >
  ;                < 7   0   6   5   1   3 >
  ret i32 0
}
```

#### Subtraction Algorithm
```
SWAR sub z = x - y;

z = ((x | H) - (y &~H)) ^ ((x ^~y) & H)
```

```llvm
define i32 @main() {
  ; Original code
  %a = sub <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1>, <i3 4, i3 4, i3 4, i3 4, i3 1, i3 2>
  ret i32 0
}
```

```llvm
define i32 @main() {
  ; After optimizatioin
  %v1 = bitcast <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1> to i18
  %v2 = bitcast <6 x i3> <i3 4, i3 4, i3 4, i3 4, i3 1, i3 2> to i18

  %low_v1 = or i18 %v1, 149796 ;get low bits for every field
  ;i18 %v1:     < 3   4   2   1   0   1 >
  ;OR: %v1      <011 100 010 001 000 001> & 
  ;	    149796  <100 100 100 100 100 100>
  ;i18 %low_v1: <111 100 110 101 100 101>
  %low_v2 = and i18 %v2, 112347
  ;i18 %v1:     < 4   4   4   4   1   2 >
  ;AND: %v1     <100 100 100 100 001 010> & 
  ;	    112347  <011 011 011 011 011 011>
  ;i18 %low_v2: <000 000 000 000 001 010>
  %low_add = sub i18 %low_v1, %low_v2 ; get the result of addition of two vectors' low bits.
  ;i18 %low_add: <011 000 010 001 001 011>
  %not = xor i18 %v2, 262143
  %overflow = xor i18 %v1, %not
  ;i18 %v1: <011 100 010 001 000 001>
  ;i18 %v2: <100 100 100 100 001 010>
  ;XOR i18: <111 000 110 101 001 011>
  %high_add = and i18 %overflow, 149796
  ;decimal 149796 = binary 100 100 100 100 100 100
  ;AND: %overflow: <111 000 110 101 001 011> &
  ;        149796: <100 100 100 100 100 100>
  ;i18  %high_add: <100 000 100 100 000 000>
  %result = xor i18 %low_add, %high_add
  ;XOR:  %low_add: <011 000 010 001 001 011> or
  ;     %high_add: <100 000 100 100 000 000>
  ;       %result: <111 000 110 101 001 011>
  ;                < 3   4   2   1   0   1 >
  ;                < 4   4   4   4   1   2 >
  ;                < 7   0   6   5   1   3 >
  ret i32 0
}
```

