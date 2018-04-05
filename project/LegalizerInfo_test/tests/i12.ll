@.str = private constant [34 x i8] c"<6 x i3><%i, %i, %i, %i, %i, %i>\0A\00", align 1
declare i32 @printf(i8*, ...)

define i32 @main() {
  %a = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
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