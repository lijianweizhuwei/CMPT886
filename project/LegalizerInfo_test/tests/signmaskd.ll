define i32 @signmaskd(<2 x i64> %a) {
    %b = icmp slt <2 x i64> %a, zeroinitializer
    %c = bitcast <2 x i1> %b to i2
    %result = zext i2 %c to i32
    ret i32 %result
}

define i32 @movmsk(<2 x i64> %a) {
    %b = bitcast <2 x i64> %a to <2 x double>
    %result = tail call i32 @llvm.x86.sse2.movmsk.pd(<2 x double> %b)
    ret i32 %result
}

@.str = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
declare i32 @printf(i8*, ...) #1
declare i32 @llvm.x86.sse2.movmsk.pd(<2 x double>)

define i32 @main() {
    %a = bitcast <2 x double> <double 0xF000000000000000, double 0xF000000000000000> to <2 x i64>
    %result = call i32 @signmaskd(<2 x i64> %a)
    ; %result = call i32 @movmsk(<2 x i64> %a)

    call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i32 0, i32 0), i32 %result)

    ret i32 0
}
