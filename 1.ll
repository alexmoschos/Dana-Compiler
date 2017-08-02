; ModuleID = 'arx.ll'
target triple = "x86_64-pc-linux-gnu"

%struct.dummy = type opaque
%struct.main = type { %struct.dummy*, i16, i16, i16, i16, i16 }

define i16 @main2(%struct.dummy* %link) {
  %1 = alloca %struct.main
  %2 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 0
  store %struct.dummy* %link, %struct.dummy** %2
  %3 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 2
  store i16 14, i16* %3
  %4 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 1
  store i16 0, i16* %4
  %5 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 3
  store i16 22, i16* %5
  %6 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 4
  store i16 11, i16* %6
  %7 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 5
  store i16 1, i16* %7
  %8 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 1
  %9 = load i16, i16* %8
  %10 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 2
  %11 = load i16, i16* %10
  %12 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 3
  %13 = load i16, i16* %12
  %14 = add i16 %11, %13
  %15 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 4
  %16 = load i16, i16* %15
  %17 = add i16 %14, %16
  %18 = getelementptr %struct.main, %struct.main* %1, i32 0, i32 5
  %19 = load i16, i16* %18
  %20 = add i16 %17, %19
  %21 = mul i16 %9, %20
  ret i16 %21
}