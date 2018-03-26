@.str = private constant [34 x i8] c"<6 x i3><%i, %i, %i, %i, %i, %i>\0A\00", align 1
declare i32 @printf(i8*, ...)

define <6 x i3> @main() {
  %a = add <6 x i3> <i3 1, i3 0, i3 1, i3 1, i3 1, i3 1>, <i3 0, i3 1, i3 1, i3 1, i3 1, i3 1>
  
  ret <6 x i3> %a
}