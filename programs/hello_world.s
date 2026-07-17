.globl _start

.text
_start:
  li  a0,1        # stdout = 1
  la  a1,message  # buff ptr
  li  a2,12       # buff length
  li  a7,64       # code for write
  ecall

  li a0, 42       # exit status
  li a7, 93       # Linux exit syscall
  ecall

message: .ascii "Hello World\n"
