; ModuleID = 'matmul.cpp'
source_filename = "matmul.cpp"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.14.0"

@__func__.main = private unnamed_addr constant [5 x i8] c"main\00", align 1
@.str = private unnamed_addr constant [11 x i8] c"matmul.cpp\00", align 1
@.str.1 = private unnamed_addr constant [10 x i8] c"argc == 2\00", align 1
@.str.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 1
@.memset_pattern = private unnamed_addr constant [4 x i32] [i32 -559038737, i32 -559038737, i32 -559038737, i32 -559038737], align 16
@.memset_pattern.3 = private unnamed_addr constant [4 x i32] [i32 -1091576147, i32 -1091576147, i32 -1091576147, i32 -1091576147], align 16

; Function Attrs: norecurse nounwind ssp uwtable
define i32 @_Z3fooiPiS_S_(i32, i32* nocapture readonly, i32* nocapture readonly, i32* nocapture) local_unnamed_addr #0 {
  %5 = sext i32 %0 to i64
  br label %8

; <label>:6:                                      ; preds = %12
  %7 = load i32, i32* %3, align 4, !tbaa !3
  ret i32 %7

; <label>:8:                                      ; preds = %12, %4
  %9 = phi i64 [ 0, %4 ], [ %13, %12 ]
  %10 = shl nuw nsw i64 %9, 1
  %11 = mul nuw nsw i64 %9, %5
  br label %15

; <label>:12:                                     ; preds = %15
  %13 = add nuw nsw i64 %9, 1
  %14 = icmp eq i64 %13, 2
  br i1 %14, label %6, label %8

; <label>:15:                                     ; preds = %15, %8
  %16 = phi i64 [ 0, %8 ], [ %25, %15 ]
  %17 = add nuw nsw i64 %16, %10
  %18 = getelementptr inbounds i32, i32* %1, i64 %17
  %19 = load i32, i32* %18, align 4, !tbaa !3
  %20 = add nsw i64 %16, %11
  %21 = getelementptr inbounds i32, i32* %2, i64 %20
  %22 = load i32, i32* %21, align 4, !tbaa !3
  %23 = add nsw i32 %22, %19
  %24 = getelementptr inbounds i32, i32* %3, i64 %20
  store i32 %23, i32* %24, align 4, !tbaa !3
  %25 = add nuw nsw i64 %16, 1
  %26 = icmp eq i64 %25, 2
  br i1 %26, label %12, label %15
}

; Function Attrs: norecurse ssp uwtable
define i32 @main(i32, i8** nocapture readonly) local_unnamed_addr #1 {
  %3 = icmp eq i32 %0, 2
  br i1 %3, label %5, label %4, !prof !7

; <label>:4:                                      ; preds = %2
  tail call void @__assert_rtn(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @__func__.main, i64 0, i64 0), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @.str, i64 0, i64 0), i32 24, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.1, i64 0, i64 0)) #8
  unreachable

; <label>:5:                                      ; preds = %2
  %6 = getelementptr inbounds i8*, i8** %1, i64 1
  %7 = load i8*, i8** %6, align 8, !tbaa !8
  %8 = tail call i32 @atoi(i8* %7)
  %9 = mul nsw i32 %8, %8
  %10 = zext i32 %9 to i64
  %11 = shl nuw nsw i64 %10, 2
  %12 = tail call i8* @_Znam(i64 %11) #9
  %13 = bitcast i8* %12 to i32*
  %14 = tail call i8* @_Znam(i64 %11) #9
  %15 = bitcast i8* %14 to i32*
  %16 = tail call i8* @_Znam(i64 %11) #9
  %17 = bitcast i8* %16 to i32*
  %18 = icmp sgt i32 %8, 0
  br i1 %18, label %19, label %27

; <label>:19:                                     ; preds = %5
  %20 = zext i32 %8 to i64
  %21 = shl nuw nsw i64 %20, 2
  %22 = zext i32 %8 to i64
  %23 = shl nuw nsw i64 %22, 2
  %24 = zext i32 %8 to i64
  %25 = shl nuw nsw i64 %24, 2
  %26 = zext i32 %8 to i64
  br label %30

; <label>:27:                                     ; preds = %30, %5
  %28 = tail call i32 @_Z3fooiPiS_S_(i32 2, i32* nonnull %13, i32* nonnull %15, i32* nonnull %17)
  %29 = tail call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str.2, i64 0, i64 0), i32 %28)
  ret i32 0

; <label>:30:                                     ; preds = %19, %30
  %31 = phi i64 [ 0, %19 ], [ %47, %30 ]
  %32 = trunc i64 %31 to i32
  %33 = mul i32 %8, %32
  %34 = sext i32 %33 to i64
  %35 = shl nsw i64 %34, 2
  %36 = getelementptr i8, i8* %16, i64 %35
  %37 = trunc i64 %31 to i32
  %38 = mul i32 %8, %37
  %39 = sext i32 %38 to i64
  %40 = shl nsw i64 %39, 2
  %41 = getelementptr i8, i8* %12, i64 %40
  %42 = trunc i64 %31 to i32
  %43 = mul i32 %8, %42
  %44 = sext i32 %43 to i64
  %45 = shl nsw i64 %44, 2
  %46 = getelementptr i8, i8* %14, i64 %45
  call void @llvm.memset.p0i8.i64(i8* %36, i8 0, i64 %21, i32 4, i1 false)
  call void @memset_pattern16(i8* %41, i8* bitcast ([4 x i32]* @.memset_pattern to i8*), i64 %23)
  call void @memset_pattern16(i8* %46, i8* bitcast ([4 x i32]* @.memset_pattern.3 to i8*), i64 %25)
  %47 = add nuw nsw i64 %31, 1
  %48 = icmp eq i64 %47, %26
  br i1 %48, label %27, label %30
}

; Function Attrs: noreturn
declare void @__assert_rtn(i8*, i8*, i32, i8*) local_unnamed_addr #2

; Function Attrs: nounwind readonly
declare i32 @atoi(i8* nocapture) local_unnamed_addr #3

; Function Attrs: nobuiltin
declare noalias nonnull i8* @_Znam(i64) local_unnamed_addr #4

; Function Attrs: nounwind
declare i32 @printf(i8* nocapture readonly, ...) local_unnamed_addr #5

; Function Attrs: argmemonly nounwind
declare void @llvm.memset.p0i8.i64(i8* nocapture writeonly, i8, i64, i32, i1) #6

; Function Attrs: argmemonly
declare void @memset_pattern16(i8* nocapture, i8* nocapture readonly, i64) local_unnamed_addr #7

attributes #0 = { norecurse nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { norecurse ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { noreturn "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="true" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nobuiltin "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #5 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #6 = { argmemonly nounwind }
attributes #7 = { argmemonly }
attributes #8 = { noreturn }
attributes #9 = { builtin }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 6.0.1 (https://github.com/llvm-mirror/clang.git 190a149b7db2c403feb656d879c8e32536981a6d) (https://github.com/llvm-mirror/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C++ TBAA"}
!7 = !{!"branch_weights", i32 2000, i32 1}
!8 = !{!9, !9, i64 0}
!9 = !{!"any pointer", !5, i64 0}
