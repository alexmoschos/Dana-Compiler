; ModuleID = 'arx.ll'
target triple = "x86_64-pc-linux-gnu"

%struct.asthenoforo = type { %struct.kremmudi*, i16, i16, i16 }
%struct.kremmudi = type { %struct.dummy*, i16, i16, i16 }
%struct.dummy = type opaque

define i16 @main() {
entry:
  ret i16 504
}

define i16 @asthenoforo() {
  %my_frame = alloca %struct.asthenoforo
  ret i16 42
}

define i16 @kremmudi() {
  %my_frame = alloca %struct.kremmudi
  ret i16 42
}
