; ModuleID = 'pr.cpp'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

$_Z14print_registerIDv2_xEvPKcT_ = comdat any

@.str = private unnamed_addr constant [6 x i8] c"temp1\00", align 1
@.str.1 = private unnamed_addr constant [6 x i8] c"temp2\00", align 1
@.str.2 = private unnamed_addr constant [26 x i8] c"simd<8>::eq(temp1, temp2)\00", align 1
@.str.3 = private unnamed_addr constant [34 x i8] c"  hsimd<64>::signmask(rslt) = %u\0A\00", align 1
@.str.4 = private unnamed_addr constant [8 x i8] c"%40s = \00", align 1
@.str.5 = private unnamed_addr constant [6 x i8] c"%02X \00", align 1

; Function Attrs: nounwind uwtable
define i32 @main(i32 %argc, i8** nocapture readnone %argv) #0 {
  %temp1 = alloca <2 x i64>, align 16
  %temp2 = alloca <2 x i64>, align 16
  %1 = bitcast <2 x i64>* %temp1 to i8*
  call void @llvm.lifetime.start(i64 16, i8* %1)
  store volatile <2 x i64> <i64 3689348814741910323, i64 3689348814741910323>, <2 x i64>* %temp1, align 16
  %2 = load volatile <2 x i64>, <2 x i64>* %temp1, align 16
  tail call void @_Z14print_registerIDv2_xEvPKcT_(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str, i64 0, i64 0), <2 x i64> %2)
  %3 = bitcast <2 x i64>* %temp2 to i8*
  call void @llvm.lifetime.start(i64 16, i8* %3)
  store volatile <2 x i64> <i64 3694133962361549636, i64 3694133962361549636>, <2 x i64>* %temp2, align 16
  %4 = load volatile <2 x i64>, <2 x i64>* %temp2, align 16
  tail call void @_Z14print_registerIDv2_xEvPKcT_(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.1, i64 0, i64 0), <2 x i64> %4)
  %5 = load volatile <2 x i64>, <2 x i64>* %temp1, align 16
  %6 = load volatile <2 x i64>, <2 x i64>* %temp2, align 16
  %7 = bitcast <2 x i64> %5 to <16 x i8>
  %8 = bitcast <2 x i64> %6 to <16 x i8>
  %9 = icmp eq <16 x i8> %7, %8
  %10 = sext <16 x i1> %9 to <16 x i8>
  %11 = bitcast <16 x i8> %10 to <2 x i64>
  tail call void @_Z14print_registerIDv2_xEvPKcT_(i8* getelementptr inbounds ([26 x i8], [26 x i8]* @.str.2, i64 0, i64 0), <2 x i64> %11)
  %12 = bitcast <16 x i8> %10 to <2 x double>
  %13 = tail call i32 @llvm.x86.sse2.movmsk.pd(<2 x double> %12) #1
  %14 = and i32 %13, 255
  %15 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @.str.3, i64 0, i64 0), i32 %14)
  call void @llvm.lifetime.end(i64 16, i8* %3)
  call void @llvm.lifetime.end(i64 16, i8* %1)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.lifetime.start(i64, i8* nocapture) #1

; Function Attrs: nounwind uwtable
define linkonce_odr void @_Z14print_registerIDv2_xEvPKcT_(i8* %var_name, <2 x i64> %v) #0 comdat {
  %1 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.4, i64 0, i64 0), i8* %var_name)
  %2 = bitcast <2 x i64> %v to i128
  %3 = lshr i128 %2, 120
  %4 = trunc i128 %3 to i32
  %5 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %4)
  %6 = lshr i128 %2, 112
  %7 = trunc i128 %6 to i32
  %8 = and i32 %7, 255
  %9 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %8)
  %10 = lshr i128 %2, 104
  %11 = trunc i128 %10 to i32
  %12 = and i32 %11, 255
  %13 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %12)
  %14 = lshr i128 %2, 96
  %15 = trunc i128 %14 to i32
  %16 = and i32 %15, 255
  %17 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %16)
  %18 = lshr i128 %2, 88
  %19 = trunc i128 %18 to i32
  %20 = and i32 %19, 255
  %21 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %20)
  %22 = lshr i128 %2, 80
  %23 = trunc i128 %22 to i32
  %24 = and i32 %23, 255
  %25 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %24)
  %26 = lshr i128 %2, 72
  %27 = trunc i128 %26 to i32
  %28 = and i32 %27, 255
  %29 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %28)
  %30 = lshr i128 %2, 64
  %31 = trunc i128 %30 to i32
  %32 = and i32 %31, 255
  %33 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %32)
  %34 = lshr i128 %2, 56
  %35 = trunc i128 %34 to i32
  %36 = and i32 %35, 255
  %37 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %36)
  %38 = lshr i128 %2, 48
  %39 = trunc i128 %38 to i32
  %40 = and i32 %39, 255
  %41 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %40)
  %42 = lshr i128 %2, 40
  %43 = trunc i128 %42 to i32
  %44 = and i32 %43, 255
  %45 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %44)
  %46 = lshr i128 %2, 32
  %47 = trunc i128 %46 to i32
  %48 = and i32 %47, 255
  %49 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %48)
  %50 = lshr i128 %2, 24
  %51 = trunc i128 %50 to i32
  %52 = and i32 %51, 255
  %53 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %52)
  %54 = lshr i128 %2, 16
  %55 = trunc i128 %54 to i32
  %56 = and i32 %55, 255
  %57 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %56)
  %58 = lshr i128 %2, 8
  %59 = trunc i128 %58 to i32
  %60 = and i32 %59, 255
  %61 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %60)
  %trunc = trunc i128 %2 to i32
  %62 = and i32 %trunc, 255
  %63 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.5, i64 0, i64 0), i32 %62)
  %putchar = tail call i32 @putchar(i32 10)
  ret void
}

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) #2

; Function Attrs: nounwind
declare void @llvm.lifetime.end(i64, i8* nocapture) #1

; Function Attrs: nounwind readnone
declare i32 @llvm.x86.sse2.movmsk.pd(<2 x double>) #3

; Function Attrs: nounwind
declare i32 @putchar(i32) #1

attributes #0 = { nounwind uwtable "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { nounwind "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+sse,+sse2" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readnone }

!llvm.ident = !{!0}

!0 = !{!"clang version 3.7.1 (tags/RELEASE_371/final)"}
