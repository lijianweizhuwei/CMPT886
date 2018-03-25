
@veca = global <6 x i3> <i3 3, i3 4, i3 2, i3 1, i3 0, i3 -1>
@vecb = global <6 x i3> <i3 4, i3 4, i3 4, i3 4, i3 -1, i3 -2>

define i32 @main() {
    %a1 = load <6 x i3>, <6 x i3>* @veca
    %a2 = load <6 x i3>, <6 x i3>* @vecb
    %r = add <6 x i3> %a1, %a2
    ret i32 0
}