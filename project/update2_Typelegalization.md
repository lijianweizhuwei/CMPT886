
# Type Legalization with Oddball Vector Operation

### Algorithm

## <6 x i3>

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
  elemNum, elemSize = 6, 3
  TotalSize = elemNum * elemSize = 18 
  widenNum = nextPowerof(6) = 8
  if (widenNum * elemSize <= register_size){
    %v1 = widen <8 x i3> <i3 0,i3 0, i3 3, i3 4, i3 2, i3 1, i3 0, i3 1> 
    %v2 = widen <8 x i3> <i3 0,i3 0, i3 4, i3 4, i3 4, i3 4, i3 1, i3 2>
    eleNum = widenNum
   }
  if (elemSize * eleNum != register_size){
  promotesize = register_size / eleNum = 16
  %v1 = promote <8 x i16> <i16 0, i16 0, i16 3, i16 4, i16 2, i16 1, i16 0, i16 1> 
  %v2 = promote <8 x i16> <i16 0, i16 0, i16 4, i16 4, i16 4, i16 4, i16 1, i16 2>
  }
  %a = add <8 x i16> <i16 0, i16 0, i16 3, i16 4, i16 2, i16 1, i16 0, i16 1>
                  , <i16 0, i16 0, i16 4, i16 4, i16 4, i16 4, i16 1, i16 2>

  
  add (i16 extractelement (<8 x i16> zext (<6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1> to <8 x i16>), i32 1), 
       i16 extractelement (<8 x i16> zext (<6 x i3> <i3 4, i3 4, i3 4, i3 4, i3 1, i3 2> to <8 x i16>), i32 1)):
  
  ret i32 0
}
```

## <25 x i5>

```llvm
define i32 @main() {
  ; Original code
  %a = add <25 x i5> 
       <i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 1>,
       <i5 0, i5 1, i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 1, i5 3, i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 3, i5 4, i5 2, i5 1, i5 4, i5 2, i5 1, i5 0, i5 1>
  ret i32 0
}
```

<25 x i5>
```llvm
define i32 @main() {
  ; After optimizatioin
  elemNum, elemSize = 25, 5
  TotalSize = elemNum * elemSize = 125
  widenNum = nextPowerof(25) = 32
  if (widenNum * elemSize > register_size){
    maxelemNum = (register_size / nextPowerof(elemSize)) = 16
    %v1_1 = <16 x i5> <i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 3, i5 4, i5 2, i5 1> 
    %v1_2 = <9 x i5> <i5 0, i5 1, i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 1>
    %v2_1 = <16 x i5> <i5 0, i5 1, i5 3, i5 4, i5 2, i5 1, i5 0, i5 1, i5 1, i5 3, i5 3, i5 4, i5 2, i5 1, i5 0, i5 1>
    %v2_2 = <9 x i5> <i5 3, i5 4, i5 2, i5 1, i5 4, i5 2, i5 1, i5 0, i5 1>
   }

  // Then we can do the same step as the above <6 x i3>.
  
  ret i32 0
}
```


### Next to do
Debugging and Optimizing

