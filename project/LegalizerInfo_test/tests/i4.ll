@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
declare i32 @printf(i8*, ...) #1

define i32 @main() {
    ; result should be 105
    %c = bitcast <2 x i4> <i4 9, i4 6> to i8
    %result = zext i8 %c to i32

    call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %result)

    ret i32 0
}
