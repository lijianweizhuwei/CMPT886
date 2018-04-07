@.str = private constant [34 x i8] c"<6 x i3><%i, %i, %i, %i, %i, %i>\0A\00", align 1
declare i32 @printf(i8*, ...)

define i32 @main() {
    %a = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
    %b = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
    %c = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
    %d = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
    %e = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
    %f = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
    %g = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
    %h = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
    %i = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>
    %j = add <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>, <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>

  ret i32 0
}