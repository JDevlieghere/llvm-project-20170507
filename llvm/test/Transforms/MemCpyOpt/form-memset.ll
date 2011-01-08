; RUN: opt < %s -memcpyopt -S | FileCheck %s

; All the stores in this example should be merged into a single memset.

target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:128:128"
target triple = "i386-apple-darwin8"

define void @test1(i8 signext  %c) nounwind  {
entry:
	%x = alloca [19 x i8]		; <[19 x i8]*> [#uses=20]
	%tmp = getelementptr [19 x i8]* %x, i32 0, i32 0		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp, align 1
	%tmp5 = getelementptr [19 x i8]* %x, i32 0, i32 1		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp5, align 1
	%tmp9 = getelementptr [19 x i8]* %x, i32 0, i32 2		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp9, align 1
	%tmp13 = getelementptr [19 x i8]* %x, i32 0, i32 3		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp13, align 1
	%tmp17 = getelementptr [19 x i8]* %x, i32 0, i32 4		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp17, align 1
	%tmp21 = getelementptr [19 x i8]* %x, i32 0, i32 5		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp21, align 1
	%tmp25 = getelementptr [19 x i8]* %x, i32 0, i32 6		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp25, align 1
	%tmp29 = getelementptr [19 x i8]* %x, i32 0, i32 7		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp29, align 1
	%tmp33 = getelementptr [19 x i8]* %x, i32 0, i32 8		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp33, align 1
	%tmp37 = getelementptr [19 x i8]* %x, i32 0, i32 9		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp37, align 1
	%tmp41 = getelementptr [19 x i8]* %x, i32 0, i32 10		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp41, align 1
	%tmp45 = getelementptr [19 x i8]* %x, i32 0, i32 11		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp45, align 1
	%tmp49 = getelementptr [19 x i8]* %x, i32 0, i32 12		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp49, align 1
	%tmp53 = getelementptr [19 x i8]* %x, i32 0, i32 13		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp53, align 1
	%tmp57 = getelementptr [19 x i8]* %x, i32 0, i32 14		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp57, align 1
	%tmp61 = getelementptr [19 x i8]* %x, i32 0, i32 15		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp61, align 1
	%tmp65 = getelementptr [19 x i8]* %x, i32 0, i32 16		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp65, align 1
	%tmp69 = getelementptr [19 x i8]* %x, i32 0, i32 17		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp69, align 1
	%tmp73 = getelementptr [19 x i8]* %x, i32 0, i32 18		; <i8*> [#uses=1]
	store i8 %c, i8* %tmp73, align 1
	%tmp76 = call i32 (...)* @bar( [19 x i8]* %x ) nounwind
	ret void
; CHECK: @test1
; CHECK-NOT: store
; CHECK: call void @llvm.memset.p0i8.i64
; CHECK-NOT: store
; CHECK: ret
}

declare i32 @bar(...)


	%struct.MV = type { i16, i16 }

define void @test2() nounwind  {
entry:
	%ref_idx = alloca [8 x i8]		; <[8 x i8]*> [#uses=8]
	%left_mvd = alloca [8 x %struct.MV]		; <[8 x %struct.MV]*> [#uses=17]
	%up_mvd = alloca [8 x %struct.MV]		; <[8 x %struct.MV]*> [#uses=17]
	%tmp20 = getelementptr [8 x i8]* %ref_idx, i32 0, i32 7		; <i8*> [#uses=1]
	store i8 -1, i8* %tmp20, align 1
	%tmp23 = getelementptr [8 x i8]* %ref_idx, i32 0, i32 6		; <i8*> [#uses=1]
	store i8 -1, i8* %tmp23, align 1
	%tmp26 = getelementptr [8 x i8]* %ref_idx, i32 0, i32 5		; <i8*> [#uses=1]
	store i8 -1, i8* %tmp26, align 1
	%tmp29 = getelementptr [8 x i8]* %ref_idx, i32 0, i32 4		; <i8*> [#uses=1]
	store i8 -1, i8* %tmp29, align 1
	%tmp32 = getelementptr [8 x i8]* %ref_idx, i32 0, i32 3		; <i8*> [#uses=1]
	store i8 -1, i8* %tmp32, align 1
	%tmp35 = getelementptr [8 x i8]* %ref_idx, i32 0, i32 2		; <i8*> [#uses=1]
	store i8 -1, i8* %tmp35, align 1
	%tmp38 = getelementptr [8 x i8]* %ref_idx, i32 0, i32 1		; <i8*> [#uses=1]
	store i8 -1, i8* %tmp38, align 1
	%tmp41 = getelementptr [8 x i8]* %ref_idx, i32 0, i32 0		; <i8*> [#uses=2]
	store i8 -1, i8* %tmp41, align 1
	%tmp43 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 7, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp43, align 2
	%tmp46 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 7, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp46, align 2
	%tmp57 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 6, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp57, align 2
	%tmp60 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 6, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp60, align 2
	%tmp71 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 5, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp71, align 2
	%tmp74 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 5, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp74, align 2
	%tmp85 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 4, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp85, align 2
	%tmp88 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 4, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp88, align 2
	%tmp99 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 3, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp99, align 2
	%tmp102 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 3, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp102, align 2
	%tmp113 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 2, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp113, align 2
	%tmp116 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 2, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp116, align 2
	%tmp127 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 1, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp127, align 2
	%tmp130 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 1, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp130, align 2
	%tmp141 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 0, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp141, align 8
	%tmp144 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 0, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp144, align 2
	%tmp148 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 7, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp148, align 2
	%tmp151 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 7, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp151, align 2
	%tmp162 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 6, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp162, align 2
	%tmp165 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 6, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp165, align 2
	%tmp176 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 5, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp176, align 2
	%tmp179 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 5, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp179, align 2
	%tmp190 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 4, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp190, align 2
	%tmp193 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 4, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp193, align 2
	%tmp204 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 3, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp204, align 2
	%tmp207 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 3, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp207, align 2
	%tmp218 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 2, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp218, align 2
	%tmp221 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 2, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp221, align 2
	%tmp232 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 1, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp232, align 2
	%tmp235 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 1, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp235, align 2
	%tmp246 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 0, i32 0		; <i16*> [#uses=1]
	store i16 0, i16* %tmp246, align 8
	%tmp249 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 0, i32 1		; <i16*> [#uses=1]
	store i16 0, i16* %tmp249, align 2
	%up_mvd252 = getelementptr [8 x %struct.MV]* %up_mvd, i32 0, i32 0		; <%struct.MV*> [#uses=1]
	%left_mvd253 = getelementptr [8 x %struct.MV]* %left_mvd, i32 0, i32 0		; <%struct.MV*> [#uses=1]
	call void @foo( %struct.MV* %up_mvd252, %struct.MV* %left_mvd253, i8* %tmp41 ) nounwind 
	ret void
        
; CHECK: @test2
; CHECK-NOT: store
; CHECK: call void @llvm.memset.p0i8.i64(i8* %tmp41, i8 -1, i64 8, i32 1, i1 false)
; CHECK-NOT: store
; CHECK: call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 32, i32 8, i1 false)
; CHECK-NOT: store
; CHECK: call void @llvm.memset.p0i8.i64(i8* %1, i8 0, i64 32, i32 8, i1 false)
; CHECK-NOT: store
; CHECK: ret
}

declare void @foo(%struct.MV*, %struct.MV*, i8*)


define void @test3(i32* nocapture %P) nounwind ssp {
entry:
  %arrayidx = getelementptr inbounds i32* %P, i64 1
  store i32 0, i32* %arrayidx, align 4
  %add.ptr = getelementptr inbounds i32* %P, i64 2
  %0 = bitcast i32* %add.ptr to i8*
  tail call void @llvm.memset.p0i8.i64(i8* %0, i8 0, i64 11, i32 1, i1 false)
  ret void
; CHECK: @test3
; CHECK-NOT: store
; CHECK: call void @llvm.memset.p0i8.i64(i8* %1, i8 0, i64 15, i32 4, i1 false)
}

declare void @llvm.memset.p0i8.i64(i8* nocapture, i8, i64, i32, i1) nounwind


