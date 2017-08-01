; ModuleID = 'arx.ll'
target triple = "x86_64-pc-linux-gnu"

%struct.a = type { i16 }

define i16 @main() {
entry:
  %a = alloca %struct.a
  %a1 = alloca %struct.a
  ret i16 1
}
