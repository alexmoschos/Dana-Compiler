; ModuleID = '1.ll'
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.dummy = type opaque
%struct.main = type { %struct.dummy*, [80 x i8] }
%struct.printBoxed = type { %struct.main*, i8*, i16, i16 }

@0 = private unnamed_addr constant [2 x i8] c"\0A\00"
@1 = private unnamed_addr constant [5 x i8] c"****\00"
@2 = private unnamed_addr constant [3 x i8] c"*\0A\00"
@3 = private unnamed_addr constant [3 x i8] c"* \00"
@4 = private unnamed_addr constant [2 x i8] c" \00"
@5 = private unnamed_addr constant [3 x i8] c"*\0A\00"
@6 = private unnamed_addr constant [5 x i8] c"****\00"
@7 = private unnamed_addr constant [4 x i8] c"*\0A\0A\00"
@8 = private unnamed_addr constant [22 x i8] c"Please, give a word: \00"
@9 = private unnamed_addr constant [6 x i8] c"peace\00"

declare void @writeChar(i8)

declare void @writeByte(i8)

declare void @writeInteger(i16)

declare void @writeString(i8*)

declare i16 @strlen(i8*)

declare i16 @strcmp(i8*, i8*)

declare i16 @strcpy(i8*, i8*)

declare void @strcat(i8*, i8*)

declare i16 @readInteger()

declare i8 @readByte()

declare i8 @readChar()

declare void @readString(i16, i8*)

define i16 @extend(i8) {
  %2 = sext i8 %0 to i16
  ret i16 %2
}

define i8 @shrink(i16) {
  %2 = trunc i16 %0 to i8
  ret i8 %2
}

define void @main(%struct.dummy* %link) {
  %1 = alloca %struct.main
  %2 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 0
  store %struct.dummy* %link, %struct.dummy** %2
  br label %4

; <label>:3                                       ; preds = %15
  ret void

; <label>:4                                       ; preds = %19, %0
  call void @writeString(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @8, i32 0, i32 0))
  %5 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 1
  %6 = load [80 x i8], [80 x i8]* %5
  %7 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 1
  %8 = getelementptr [80 x i8], [80 x i8]* %7, i64 0, i64 0
  call void @readString(i16 80, i8* %8)
  %9 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 1
  %10 = load [80 x i8], [80 x i8]* %9
  %11 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 1
  %12 = getelementptr [80 x i8], [80 x i8]* %11, i64 0, i64 0
  %13 = call i16 @strcmp(i8* %12, i8* getelementptr inbounds ([6 x i8], [6 x i8]* @9, i32 0, i32 0))
  %14 = icmp eq i16 %13, 0
  br i1 %14, label %16, label %18

; <label>:15                                      ; preds = %16
  br label %3

; <label>:16                                      ; preds = %4
  br label %15
                                                  ; No predecessors!
  br label %19

; <label>:18                                      ; preds = %4
  br label %19

; <label>:19                                      ; preds = %18, %17
  %20 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 1
  %21 = load [80 x i8], [80 x i8]* %20
  %22 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 1
  %23 = getelementptr [80 x i8], [80 x i8]* %22, i64 0, i64 0
  call void @printBoxed(%struct.main* %1, i8* %23)
  br label %4
}

define void @printBoxed(%struct.main* %link, i8* %word) {
  %1 = alloca %struct.printBoxed
  %2 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 0
  store %struct.main* %link, %struct.main** %2
  %3 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 1
  store i8* %word, i8** %3
  %4 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 2
  %5 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 1
  %6 = load i8*, i8** %5
  %7 = call i16 @strlen(i8* %6)
  store i16 %7, i16* %4
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @0, i32 0, i32 0))
  %8 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  store i16 0, i16* %8
  br label %10

; <label>:9                                       ; preds = %107
  ret void

; <label>:10                                      ; preds = %25, %0
  call void @writeString(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @1, i32 0, i32 0))
  %11 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %12 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %13 = load i16, i16* %12
  %14 = add i16 %13, 1
  store i16 %14, i16* %11
  %15 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %16 = load i16, i16* %15
  %17 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 2
  %18 = load i16, i16* %17
  %19 = icmp sge i16 %16, %18
  br i1 %19, label %22, label %24

; <label>:20                                      ; preds = %22
  call void @writeString(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @2, i32 0, i32 0))
  %21 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  store i16 0, i16* %21
  br label %26

; <label>:22                                      ; preds = %10
  br label %20
                                                  ; No predecessors!
  br label %25

; <label>:24                                      ; preds = %10
  br label %25

; <label>:25                                      ; preds = %24, %23
  br label %10

; <label>:26                                      ; preds = %96, %20
  call void @writeString(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @3, i32 0, i32 0))
  %27 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 1
  %28 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %29 = load i16, i16* %28
  %30 = sext i16 %29 to i64
  %31 = load i8*, i8** %27
  %32 = getelementptr inbounds i8, i8* %31, i64 %30
  %33 = load i8, i8* %32
  %34 = icmp sge i8 %33, 65
  %35 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 1
  %36 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %37 = load i16, i16* %36
  %38 = sext i16 %37 to i64
  %39 = load i8*, i8** %35
  %40 = getelementptr inbounds i8, i8* %39, i64 %38
  %41 = load i8, i8* %40
  %42 = icmp sle i8 %41, 90
  %43 = and i1 %34, %42
  br i1 %43, label %46, label %54

; <label>:44                                      ; preds = %93
  call void @writeString(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @5, i32 0, i32 0))
  %45 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  store i16 0, i16* %45
  br label %97

; <label>:46                                      ; preds = %26
  %47 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 1
  %48 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %49 = load i16, i16* %48
  %50 = sext i16 %49 to i64
  %51 = load i8*, i8** %47
  %52 = getelementptr inbounds i8, i8* %51, i64 %50
  %53 = load i8, i8* %52
  call void @writeChar(i8 %53)
  br label %83

; <label>:54                                      ; preds = %26
  %55 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 1
  %56 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %57 = load i16, i16* %56
  %58 = sext i16 %57 to i64
  %59 = load i8*, i8** %55
  %60 = getelementptr inbounds i8, i8* %59, i64 %58
  %61 = load i8, i8* %60
  %62 = icmp sge i8 %61, 97
  %63 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 1
  %64 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %65 = load i16, i16* %64
  %66 = sext i16 %65 to i64
  %67 = load i8*, i8** %63
  %68 = getelementptr inbounds i8, i8* %67, i64 %66
  %69 = load i8, i8* %68
  %70 = icmp sle i8 %69, 122
  %71 = and i1 %62, %70
  br i1 %71, label %72, label %82

; <label>:72                                      ; preds = %54
  %73 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 1
  %74 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %75 = load i16, i16* %74
  %76 = sext i16 %75 to i64
  %77 = load i8*, i8** %73
  %78 = getelementptr inbounds i8, i8* %77, i64 %76
  %79 = load i8, i8* %78
  %80 = add i8 %79, 65
  %81 = sub i8 %80, 97
  call void @writeChar(i8 %81)
  br label %83

; <label>:82                                      ; preds = %54
  br label %83

; <label>:83                                      ; preds = %82, %72, %46
  call void @writeString(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @4, i32 0, i32 0))
  %84 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %85 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %86 = load i16, i16* %85
  %87 = add i16 %86, 1
  store i16 %87, i16* %84
  %88 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %89 = load i16, i16* %88
  %90 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 2
  %91 = load i16, i16* %90
  %92 = icmp sge i16 %89, %91
  br i1 %92, label %93, label %95

; <label>:93                                      ; preds = %83
  br label %44
                                                  ; No predecessors!
  br label %96

; <label>:95                                      ; preds = %83
  br label %96

; <label>:96                                      ; preds = %95, %94
  br label %26

; <label>:97                                      ; preds = %111, %44
  call void @writeString(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @6, i32 0, i32 0))
  %98 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %99 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %100 = load i16, i16* %99
  %101 = add i16 %100, 1
  store i16 %101, i16* %98
  %102 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 3
  %103 = load i16, i16* %102
  %104 = getelementptr %struct.printBoxed, %struct.printBoxed* %1, i32 0, i32 2
  %105 = load i16, i16* %104
  %106 = icmp sge i16 %103, %105
  br i1 %106, label %108, label %110

; <label>:107                                     ; preds = %108
  call void @writeString(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @7, i32 0, i32 0))
  br label %9

; <label>:108                                     ; preds = %97
  br label %107
                                                  ; No predecessors!
  br label %111

; <label>:110                                     ; preds = %97
  br label %111

; <label>:111                                     ; preds = %110, %109
  br label %97
}
