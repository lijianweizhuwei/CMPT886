@.str = private constant [34 x i8] c"<6 x i3><%i, %i, %i, %i, %i, %i>\0A\00", align 1
declare i32 @printf(i8*, ...)

define i32 @main() {
  %1 = and i32 131747, 112347
  %2 = and i32 108836, 112347
  %3 = add i32 %1, %2
  %4 = xor i32 131747, 108836
  %5 = and i32 %4, 149796
  %6 = xor i32 %3, %5
  %7 = zext i32 %6 to i128
  %8 = bitcast i128 %7 to <16 x i8>
  %a0 = extractelement <16 x i8> %8, i32 0
  %a1 = extractelement <16 x i8> %8, i32 1
  %a2 = extractelement <16 x i8> %8, i32 2
  %a3 = extractelement <16 x i8> %8, i32 3
  %a4 = extractelement <16 x i8> %8, i32 4
  %a5 = extractelement <16 x i8> %8, i32 5
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str, i32 0, i32 0), i8 %a0, i8 %a1, i8 %a2, i8 %a3, i8 %a4, i8 %a5)

  ret i32 0  
}