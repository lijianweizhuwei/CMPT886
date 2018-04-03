@.str = private constant [34 x i8] c"<17 x i5><%i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i, %i>\0A\00", align 1
declare i32 @printf(i8*, ...)

define i32 @main() {
  %a = add <17 x i5> <i5 3, i5 7, i5 11, i5 0, i5 12, i5 14, i5 3, i5 7, i5 11, i5 0, i5 12, i5 14, i5 3, i5 7, i5 11, i5 0, i5 12>, 
                     <i5 4, i5 3, i5 5, i5 6, i5 2, i5 8, i5 14, i5 17, i5 12, i5 18, i5 13, i5 11, i5 6, i5 6, i5 -1, i5 15, i5 4>
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str, i32 0, i32 0), i5 %a0, i5 %a1, i5 %a2, i5 %a3, i5 %a4, i5 %a5, i5 %a6, i5 %a7, i5 %a8, i5 %a9, i5 %a10, i5 %a11, i5 %a12, i5 %a13, i5 %a14, i5 %a15, i5 %a16)
  ret i32 0
}