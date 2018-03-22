@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
declare i32 @printf(i8*, ...) #1

define i32 @main() {
    ; result should be 0b0100000111 = 263
    %c = bitcast <2 x i5> <i5 7, i5 8> to i10
    %result = zext i10 %c to i32

    call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %result)

    ret i32 0
}
