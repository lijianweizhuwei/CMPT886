
# Type Legalization with Oddball <6 x i3> Vector Operation

### Algorithm



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
  %v1 = widen <8 x i3> <i3 0,i3 0, i3 3, i3 4, i3 2, i3 1, i3 0, i3 1> 
  %v2 = widen <8 x i3> <i3 0,i3 0, i3 4, i3 4, i3 4, i3 4, i3 1, i3 2>
  
  %v1 = promote <8 x i16> <i16 0, i16 0, i16 3, i16 4, i16 2, i16 1, i16 0, i16 1> 
  %v2 = promote <8 x i16> <i16 0, i16 0, i16 4, i16 4, i16 4, i16 4, i16 1, i16 2>
 
  %a = add <8 x i16> <i16 0, i16 0, i16 3, i16 4, i16 2, i16 1, i16 0, i16 1>
                  , <i16 0, i16 0, i16 4, i16 4, i16 4, i16 4, i16 1, i16 2>
  ret i32 0
  
  add (i16 extractelement (<8 x i16> zext (<6 x i3> <i3 3, i3 -4, i3 2, i3 1, i3 0, i3 1> to <8 x i16>), i32 1), 
       i16 extractelement (<8 x i16> zext (<6 x i3> <i3 3, i3 -4, i3 2, i3 1, i3 0, i3 1> to <8 x i16>), i32 1)):
}
```

### Next to do
1. support oddball types like <25 x i5>
2. support operations which might have more than one type, such as *icmp*

