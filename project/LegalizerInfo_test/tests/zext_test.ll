@.str = private constant [34 x i8] c"<8 x i4><%i, %i, %i, %i, %i, %i, %i, %i>\0A\00", align 1
declare i32 @printf(i8*, ...)

define i32 @main() {
  %a = zext <8 x i3> <i3 2, i3 1, i3 4, i3 5, i3 1, i3 7, i3 1, i3 7> to <8 x i4>
  %a0 = extractelement <8 x i4> %a, i32 0
  %a1 = extractelement <8 x i4> %a, i32 1
  %a2 = extractelement <8 x i4> %a, i32 2
  %a3 = extractelement <8 x i4> %a, i32 3
  %a4 = extractelement <8 x i4> %a, i32 4
  %a5 = extractelement <8 x i4> %a, i32 5
  %a6 = extractelement <8 x i4> %a, i32 6
  %a7 = extractelement <8 x i4> %a, i32 7
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str, i32 0, i32 0), i4 %a0, i4 %a1, i4 %a2, i4 %a3, i4 %a4, i4 %a5, i4 %a6, i4 %a7)

  ret i32 0
}