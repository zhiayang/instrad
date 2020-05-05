## instrad ##
###### (name subject to change) ######


### what is this ###
A completely (completely) dependency-free decoder/disassembler for the x86 and x86_64 instruction set, including x87, 3dnow, SSE1-SSE4, AVX, and AVX2. Doesn't support XOP, because neither does AMD any more.

Also, it's 100% `constexpr`.


### why is this ###
For fun, of course. Sure, intel xed exists, but that's not exciting.



### how to use this ###
While this is primarily meant for use as a library, there's some example code in `main.cpp` that demonstrates how to use this. You get information about the instruction and the operands, which *should* be enough to re-construct the original instruction (ie. like an assembler), if you are so inclined. This functionality is not provided by the library currently (if ever).



### how is this ###
Basically the entire thing is table-driven; it's just a matter of choosing the right tables. AMD and Intel made a real good show of stuffing every possible hole in the instruction space with stuff. (WHY TF DOES VEX ENCODING EVEN HAVE ESCAPE GROUPS?! THERE'S SO MUCH SPACE)

But I digress. The entire structure is essentially a series of nested tables, and the decoder chooses the right one based on the current set of prefixes.

The tables are in `source/include/x64/tables/`. They were all transcribed by hand from the AMD manual's opcode table, so there might be some mistakes. For the non-SIMD instructions, I think they should be all correct.


### see ###
```
    48 8d 46 10         lea rax, [rsi + 0x10]
    48 89 06            mov QWORD PTR [rsi], rax
    48 8b 44 24 10      mov rax, QWORD PTR [rsp + 0x10]
    48 39 d8            cmp rax, rbx
    75 86               jnz 0x86
    66 0f 6f 4c 24 20   movdqa xmm1, XMMWORD PTR [rsp + 0x20]
    0f 11 4e 10         movups XMMWORD PTR [rsi + 0x10], xmm1
    eb 86               jmp 0x86
    65 f3 a4            rep movsb BYTE PTR es:[rdi], BYTE PTR gs:[rsi]
    c5 fb 10 07         vmovsd xmm8, QWORD PTR [rdi]
    c5 fb 10 c1         vmovsd xmm8, xmm0
    c5 f8 ae 10         vldmxcsr QWORD PTR [rax]
    c4 e2 79 18 00      vbroadcastss xmm0, DWORD PTR [rax]
    c4 e2 79 18 c0      vbroadcastss xmm0, xmm0
```


### disclaimer ###

There's no guarantee that any of the output is correct, but there's two main disclaimers:

1. I mainly wrote this targeting x86_64, so some of the decoding might be wrong if you are deconstructing a 32-bit (compat mode) bitstream.
2. Pretty sure my constants are wonky, and I'm not handling sign extension properly in most (or all?) cases.
3. Offsets for `jmp` (and gang) and `call` are not displayed properly, but this is the fault of the driver code (`main.cpp`), and not of the library (basically, it shows the relative offset as if it were an absolute address).
