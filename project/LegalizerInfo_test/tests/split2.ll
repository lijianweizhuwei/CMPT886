@.str = private constant [80 x i8] c"<17 x i15><%i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i>\0A\00", align 1
declare i32 @printf(i8*, ...)

define i32 @main() {
  %a = add <17 x i15> <i15 3, i15 7, i15 11, i15 0, i15 12, i15 14, i15 3, i15 7, i15 11, i15 0, i15 12, i15 14, i15 3, i15 7, i15 11, i15 0, i15 12>, 
                      <i15 4, i15 13, i15 15, i15 6, i15 22, i15 18, i15 14, i15 17, i15 12, i15 18, i15 13, i15 11, i15 61, i15 76, i15 -21, i15 15, i15 44>
  %b = add <17 x i15> %a, 
                      <i15 4, i15 13, i15 15, i15 6, i15 22, i15 18, i15 14, i15 17, i15 12, i15 18, i15 13, i15 11, i15 61, i15 76, i15 -21, i15 15, i15 44>
  %a0 = extractelement <17 x i15> %a, i32 0
  %a1 = extractelement <17 x i15> %a, i32 1
  %a2 = extractelement <17 x i15> %a, i32 2
  %a3 = extractelement <17 x i15> %a, i32 3
  %a4 = extractelement <17 x i15> %a, i32 4
  %a5 = extractelement <17 x i15> %a, i32 5
  %a6 = extractelement <17 x i15> %a, i32 6
  %a7 = extractelement <17 x i15> %a, i32 7
  %a8 = extractelement <17 x i15> %a, i32 8
  %a9 = extractelement <17 x i15> %a, i32 9
  %a10 = extractelement <17 x i15> %a, i32 10
  %a11 = extractelement <17 x i15> %a, i32 11
  %a12 = extractelement <17 x i15> %a, i32 12
  %a13 = extractelement <17 x i15> %a, i32 13
  %a14 = extractelement <17 x i15> %a, i32 14
  %a15 = extractelement <17 x i15> %a, i32 15
  %a16 = extractelement <17 x i15> %a, i32 16

  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([80 x i8], [80 x i8]* @.str, i32 0, i32 0), i15 %a0, i15 %a1, i15 %a2, i15 %a3, i15 %a4, i15 %a5, i15 %a6, i15 %a7, i15 %a8, i15 %a9, i15 %a10, i15 %a11, i15 %a12, i15 %a13, i15 %a14, i15 %a15, i15 %a16)
  ret i32 0
}