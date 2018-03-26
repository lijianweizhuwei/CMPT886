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
  ;     112347  <011 011 011 011 011 011>
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
  %v1 = bitcast <6 x i3> <i3 4, i3 4, i3 4, i3 4, i3 1, i3 2> to i18
  %v2 = bitcast <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1> to i18

  %low_v1 = or i18 %v1, 149796 ;get low bits for every field
  ;i18 %v1:     < 4   4   4   4   1   2 >
  ;OR: %v1      <100 100 100 100 001 010> & 
  ;     149796  <100 100 100 100 100 100>
  ;i18 %low_v1: <100 100 100 100 101 110>
  %low_v2 = and i18 %v2, 112347
  ;i18 %v2:     < 3   4   2   1   0   1 >
  ;AND: %v2     <011 100 010 001 000 001> & 
  ;     112347  <011 011 011 011 011 011>
  ;i18 %low_v2: <011 000 010 001 000 001>
  %low_sub = sub i18 %low_v1, %low_v2 ; get the result of addition of two vectors' low bits.
  ;i18 %low_sub: <001 100 010 011 101 101>
  %_v2 = xor i18 %v2, 262143
  %overflow = xor i18 %v1, %_v2
  ;i18 %v1:  <100 100 100 100 001 010>
  ;i18 %_v2: <100 011 101 110 111 110>
  ;XOR i18:  <000 111 001 010 110 100>
  %high_sub = and i18 %overflow, 149796
  ;AND: %overflow: <000 111 001 010 110 100> &
  ;        149796: <100 100 100 100 100 100>
  ;i18  %high_sub: <000 100 000 000 100 100>
  %result = xor i18 %low_sub, %high_sub
  ;XOR:  %low_sub: <001 100 010 011 101 101> or
  ;     %high_sub: <000 100 000 000 100 100>
  ;       %result: <001 000 010 011 001 001>
  ;                < 4   4   4   4   1   2 >
  ;                < 3   4   2   1   0   1 >
  ;                < 1   0   2   3   1   1 >
  ret i32 0
}
```

#### Accomplishment
Original file: test.ll
```llvm
@.str = private constant [34 x i8] c"<6 x i3><%i, %i, %i, %i, %i, %i>\0A\00", align 1
declare i32 @printf(i8*, ...)

define i32 @main() {
  %a = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
  %a0 = extractelement <6 x i3> %a, i32 0
  %a1 = extractelement <6 x i3> %a, i32 1
  %a2 = extractelement <6 x i3> %a, i32 2
  %a3 = extractelement <6 x i3> %a, i32 3
  %a4 = extractelement <6 x i3> %a, i32 4
  %a5 = extractelement <6 x i3> %a, i32 5
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str, i32 0, i32 0), i3 %a0, i3 %a1, i3 %a2, i3 %a3, i3 %a4, i3 %a5)
  ret i32 0
}
```
After pass:
![Alt text](../image//swar_update21.jpg)

Running time:
![Alt text](../image//swar_update22.jpg)

#### Difficulties
* bitcast
* negative numbers

<6 x i3> <i3 -1, i3 0, i3 -2, i3 -3, i3 -1, i3 -1>

<6 x i3> <7, 0, 6, 5, 7, 7>
