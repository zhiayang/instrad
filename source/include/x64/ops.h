// ops.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace instrad::x64
{
	struct Op
	{
		constexpr Op(const char* m) : m_mnemonic(m) { }

		constexpr const char* mnemonic() const { return this->m_mnemonic; }
		constexpr bool operator == (const Op& other) const
		{
			// strcmp, lmao
			auto l = this->m_mnemonic;
			auto r = other.m_mnemonic;

			while(*l && *r)
				if(*l++ != *r++)
					return false;

			return true;
		}

		constexpr bool operator != (const Op& other) const { return !(*this == other); }

	private:
		const char* m_mnemonic;
	};

	namespace ops
	{
		constexpr auto NONE         = Op("");
		constexpr auto INVALID      = Op("??");

		constexpr auto ADD          = Op("add");
		constexpr auto ADC          = Op("adc");
		constexpr auto AND          = Op("and");
		constexpr auto XOR          = Op("xor");
		constexpr auto INC          = Op("inc");
		constexpr auto PUSH         = Op("push");
		constexpr auto PUSHA        = Op("pusha");
		constexpr auto PUSHAD       = Op("pushad");
		constexpr auto POPA         = Op("popa");
		constexpr auto POPAD        = Op("popad");
		constexpr auto TEST         = Op("test");
		constexpr auto XCHG         = Op("xchg");
		constexpr auto MOV          = Op("mov");
		constexpr auto RET          = Op("ret");
		constexpr auto RETF         = Op("retf");
		constexpr auto LOOPNZ       = Op("loopnz");
		constexpr auto LOOPZ        = Op("loopz");
		constexpr auto LOOP         = Op("loop");
		constexpr auto IN           = Op("in");
		constexpr auto OUT          = Op("out");
		constexpr auto OR           = Op("or");
		constexpr auto SBB          = Op("sbb");
		constexpr auto SUB          = Op("sub");
		constexpr auto CMP          = Op("cmp");
		constexpr auto DEC          = Op("dec");
		constexpr auto POP          = Op("pop");
		constexpr auto INS          = Op("ins");
		constexpr auto INSB         = Op("insb");
		constexpr auto INSW         = Op("insw");
		constexpr auto INSD         = Op("insd");
		constexpr auto OUTS         = Op("outs");
		constexpr auto OUTSB        = Op("outsb");
		constexpr auto OUTSW        = Op("outsw");
		constexpr auto OUTSD        = Op("outsd");
		constexpr auto JS           = Op("js");
		constexpr auto JNS          = Op("jns");
		constexpr auto JP           = Op("jp");
		constexpr auto JNP          = Op("jnp");
		constexpr auto JL           = Op("jl");
		constexpr auto JNL          = Op("jnl");
		constexpr auto JLE          = Op("jle");
		constexpr auto JNLE         = Op("jnle");
		constexpr auto JGE          = Op("jge");
		constexpr auto JG           = Op("jg");
		constexpr auto LEA          = Op("lea");
		constexpr auto CBW          = Op("cbw");
		constexpr auto CWDE         = Op("cwde");
		constexpr auto CDQE         = Op("cdqe");
		constexpr auto CWD          = Op("cwd");
		constexpr auto CDQ          = Op("cdq");
		constexpr auto CQO          = Op("cqo");
		constexpr auto CALL         = Op("call");
		constexpr auto CALLF        = Op("callf");
		constexpr auto FWAIT        = Op("fwait");
		constexpr auto PUSHF        = Op("pushf");
		constexpr auto POPF         = Op("popf");
		constexpr auto SAHF         = Op("sahf");
		constexpr auto LAHF         = Op("lahf");
		constexpr auto STOS         = Op("stos");
		constexpr auto STOSB        = Op("stosb");
		constexpr auto STOSW        = Op("stosw");
		constexpr auto STOSD        = Op("stosd");
		constexpr auto STOSQ        = Op("stosq");
		constexpr auto LODS         = Op("lods");
		constexpr auto LODSB        = Op("lodsb");
		constexpr auto LODSW        = Op("lodsw");
		constexpr auto LODSD        = Op("lodsd");
		constexpr auto LODSQ        = Op("lodsq");
		constexpr auto SCAS         = Op("scas");
		constexpr auto SCASB        = Op("scasb");
		constexpr auto SCASW        = Op("scasw");
		constexpr auto SCASD        = Op("scasd");
		constexpr auto SCASQ        = Op("scasq");
		constexpr auto ENTER        = Op("enter");
		constexpr auto LEAVE        = Op("leave");
		constexpr auto INT          = Op("int");
		constexpr auto INT3         = Op("int3");
		constexpr auto IRET         = Op("iret");
		constexpr auto JMP          = Op("jmp");

		constexpr auto PAUSE        = Op("pause");

		constexpr auto IMUL         = Op("imul");
		constexpr auto IDIV         = Op("idiv");

		constexpr auto MOVS         = Op("movs");
		constexpr auto MOVSB        = Op("movsb");
		constexpr auto MOVSW        = Op("movsw");
		constexpr auto MOVSD        = Op("movsd");      // SSE also has a movsd instruction, god dammit intel
		constexpr auto MOVSQ        = Op("movsq");

		constexpr auto CMPS         = Op("cmps");
		constexpr auto CMPSB        = Op("cmpsb");
		constexpr auto CMPSW        = Op("cmpsw");
		constexpr auto CMPSD        = Op("cmpsd");      // SSE also has a cmpsd instruction, god dammit intel
		constexpr auto CMPSQ        = Op("cmpsq");

		constexpr auto MUL          = Op("mul");
		constexpr auto DIV          = Op("div");

		constexpr auto DAA          = Op("daa");
		constexpr auto AAA          = Op("aaa");
		constexpr auto DAS          = Op("das");
		constexpr auto AAS          = Op("aas");
		constexpr auto AAM          = Op("aam");
		constexpr auto AAD          = Op("aad");
		constexpr auto BOUND        = Op("bound");
		constexpr auto INTO         = Op("into");

		constexpr auto SYSCALL      = Op("syscall");
		constexpr auto SYSRET       = Op("sysret");
		constexpr auto CLTS         = Op("clts");
		constexpr auto XLAT         = Op("xlat");
		constexpr auto JECXZ        = Op("jecxz");

		constexpr auto RDRAND       = Op("rdrand");
		constexpr auto RDSEED       = Op("rdseed");
		constexpr auto RDMSR        = Op("rdmsr");
		constexpr auto WRMSR        = Op("wrmsr");
		constexpr auto RDTSC        = Op("rdtsc");
		constexpr auto RDPMC        = Op("rdpmc");

		constexpr auto JO           = Op("jo");
		constexpr auto JNO          = Op("jno");
		constexpr auto JB           = Op("jb");
		constexpr auto JNB          = Op("jnb");
		constexpr auto JBO          = Op("jbo");
		constexpr auto JZ           = Op("jz");
		constexpr auto JNZ          = Op("jnz");
		constexpr auto JA           = Op("ja");
		constexpr auto JNA          = Op("jna");
		constexpr auto SETO         = Op("seto");
		constexpr auto SETNO        = Op("setno");
		constexpr auto SETB         = Op("setb");
		constexpr auto SETNB        = Op("setnb");
		constexpr auto SETZ         = Op("setz");
		constexpr auto SETNZ        = Op("setnz");
		constexpr auto SETBE        = Op("setbe");
		constexpr auto SETNBE       = Op("setnbe");
		constexpr auto SETS         = Op("sets");
		constexpr auto SETNS        = Op("setns");
		constexpr auto SETNA        = Op("setna");
		constexpr auto SETA         = Op("seta");
		constexpr auto SETP         = Op("setp");
		constexpr auto SETNP        = Op("setnp");
		constexpr auto SETL         = Op("setl");
		constexpr auto SETNL        = Op("setnl");
		constexpr auto SETGE        = Op("setge");
		constexpr auto SETG         = Op("setg");
		constexpr auto SETLE        = Op("setle");
		constexpr auto SETNLE       = Op("setnle");

		constexpr auto CPUID        = Op("cpuid");
		constexpr auto CMPXCHG      = Op("cmpxchg");
		constexpr auto CMPXCHG8B    = Op("cmpxchg8b");
		constexpr auto CMPXCHG16B   = Op("cmpxchg16b");
		constexpr auto XADD         = Op("xadd");
		constexpr auto MOVZX        = Op("movzx");
		constexpr auto MOVSXD       = Op("movsxd");
		constexpr auto MOVSX        = Op("movsx");
		constexpr auto ICEBP        = Op("icebp");
		constexpr auto HLT          = Op("hlt");
		constexpr auto CMC          = Op("cmc");
		constexpr auto CLC          = Op("clc");
		constexpr auto STC          = Op("stc");
		constexpr auto CLI          = Op("cli");
		constexpr auto STI          = Op("sti");
		constexpr auto CLD          = Op("cld");
		constexpr auto STD          = Op("std");

		constexpr auto BT           = Op("bt");
		constexpr auto BSWAP        = Op("bswap");
		constexpr auto BTC          = Op("btc");
		constexpr auto BTR          = Op("btr");
		constexpr auto BTS          = Op("bts");
		constexpr auto BSF          = Op("bsf");
		constexpr auto BSR          = Op("bsr");
		constexpr auto NOT          = Op("not");
		constexpr auto NEG          = Op("neg");

		constexpr auto ROL          = Op("rol");
		constexpr auto ROR          = Op("ror");
		constexpr auto RCL          = Op("rcl");
		constexpr auto RCR          = Op("rcr");
		constexpr auto SHL          = Op("shl");
		constexpr auto SHR          = Op("shr");
		constexpr auto SAL          = Op("sal");
		constexpr auto SAR          = Op("sar");
		constexpr auto SHLD         = Op("shld");
		constexpr auto SHRD         = Op("shrd");

		constexpr auto LAR          = Op("lar");
		constexpr auto LSL          = Op("lsl");

		constexpr auto INVD         = Op("invd");
		constexpr auto WBINVD       = Op("wbinvd");
		constexpr auto UD0          = Op("ud0");
		constexpr auto UD1          = Op("ud1");
		constexpr auto UD2          = Op("ud2");
		constexpr auto PREFETCH     = Op("prefetch");
		constexpr auto FEMMS        = Op("femms");
		constexpr auto NOP          = Op("nop");
		constexpr auto SYSENTER     = Op("sysenter");
		constexpr auto SYSEXIT      = Op("sysexit");
		constexpr auto RSM          = Op("rsm");
		constexpr auto SMSW         = Op("smsw");
		constexpr auto LMSW         = Op("lmsw");

		constexpr auto SWAPGS       = Op("swapgs");
		constexpr auto RDTSCP       = Op("rdtscp");

		constexpr auto INVLPG       = Op("invlpg");
		constexpr auto INVLPGA      = Op("invlpga");

		constexpr auto MONITOR      = Op("monitor");
		constexpr auto MONITORX     = Op("monitorx");
		constexpr auto MWAIT        = Op("mwait");
		constexpr auto MWAITX       = Op("mwaitx");

		constexpr auto XGETBV       = Op("xgetbv");
		constexpr auto XSETBV       = Op("xsetbv");

		constexpr auto VMCALL       = Op("vmcall");
		constexpr auto VMLOAD       = Op("vmload");
		constexpr auto VMSAVE       = Op("vmsave");
		constexpr auto VMRUN        = Op("vmrun");

		constexpr auto STGI         = Op("stgi");
		constexpr auto CLGI         = Op("clgi");
		constexpr auto SKINIT       = Op("skinit");

		constexpr auto FXSAVE       = Op("fxsave");
		constexpr auto FXRSTOR      = Op("fxrstor");
		constexpr auto LDMXCSR      = Op("ldmxcsr");
		constexpr auto STMXCSR      = Op("stmxcsr");
		constexpr auto XSAVE        = Op("xsave");
		constexpr auto XRSTOR       = Op("xrstor");
		constexpr auto XSAVEOPT     = Op("xsaveopt");

		constexpr auto LFENCE       = Op("lfence");
		constexpr auto SFENCE       = Op("sfence");
		constexpr auto MFENCE       = Op("mfence");
		constexpr auto CLFLUSH      = Op("clflush");

		constexpr auto RDFSBASE     = Op("rdfsbase");
		constexpr auto RDGSBASE     = Op("rdgsbase");
		constexpr auto WRFSBASE     = Op("wrfsbase");
		constexpr auto WRGSBASE     = Op("wrgsbase");

		constexpr auto LDS          = Op("lds");
		constexpr auto LES          = Op("les");
		constexpr auto LFS          = Op("lfs");
		constexpr auto LGS          = Op("lgs");
		constexpr auto LSS          = Op("lss");

		constexpr auto SLDT         = Op("sldt");
		constexpr auto STR          = Op("str");
		constexpr auto LLDT         = Op("lldt");
		constexpr auto LTR          = Op("ltr");

		constexpr auto SGDT         = Op("sgdt");
		constexpr auto SIDT         = Op("sidt");
		constexpr auto LGDT         = Op("lgdt");
		constexpr auto LIDT         = Op("lidt");

		constexpr auto VERR         = Op("verr");
		constexpr auto VERW         = Op("verw");

		constexpr auto CMOVO        = Op("cmovo");
		constexpr auto CMOVNO       = Op("cmovno");
		constexpr auto CMOVB        = Op("cmovb");
		constexpr auto CMOVNB       = Op("cmovnb");
		constexpr auto CMOVZ        = Op("cmovz");
		constexpr auto CMOVNZ       = Op("cmovnz");
		constexpr auto CMOVBE       = Op("cmovbe");
		constexpr auto CMOVNBE      = Op("cmovnbe");
		constexpr auto CMOVS        = Op("cmovs");
		constexpr auto CMOVNS       = Op("cmovns");
		constexpr auto CMOVP        = Op("cmovp");
		constexpr auto CMOVNP       = Op("cmovnp");
		constexpr auto CMOVA        = Op("cmova");
		constexpr auto CMOVNA       = Op("cmovna");
		constexpr auto CMOVL        = Op("cmovl");
		constexpr auto CMOVLE       = Op("cmovle");
		constexpr auto CMOVG        = Op("cmovg");
		constexpr auto CMOVGE       = Op("cmovge");

		constexpr auto MOVUPS       = Op("movups");
		constexpr auto MOVLPS       = Op("movlps");
		constexpr auto MOVHLPS      = Op("movhlps");
		constexpr auto UNPCKLPS     = Op("unpcklps");
		constexpr auto UNPCKHPS     = Op("unpckhps");
		constexpr auto MOVHPS       = Op("movhps");
		constexpr auto MOVLHPS      = Op("movlhps");

		constexpr auto MOVSS        = Op("movss");
		constexpr auto MOVSLDUP     = Op("movsldup");
		constexpr auto MOVSHDUP     = Op("movshdup");

		constexpr auto MOVUPD       = Op("movupd");
		constexpr auto MOVLPD       = Op("movlpd");
		constexpr auto UNPCKLPD     = Op("unpcklpd");
		constexpr auto UNPCKHPD     = Op("unpckhpd");
		constexpr auto MOVHPD       = Op("movhpd");

		constexpr auto MOVDDUP      = Op("movddup");

		constexpr auto MOVMSKPS     = Op("movmskps");
		constexpr auto SQRTPS       = Op("sqrtps");
		constexpr auto RSQRTPS      = Op("rsqrtps");
		constexpr auto RCPPS        = Op("rcpps");
		constexpr auto ANDPS        = Op("andps");
		constexpr auto ANDNPS       = Op("andnps");
		constexpr auto ORPS         = Op("orps");
		constexpr auto XORPS        = Op("xorps");

		constexpr auto SQRTSS       = Op("sqrtss");
		constexpr auto RSQRTSS      = Op("rsqrtss");
		constexpr auto RCPSS        = Op("rcpss");

		constexpr auto MOVMSKPD     = Op("movmskpd");
		constexpr auto SQRTPD       = Op("sqrtpd");
		constexpr auto ANDPD        = Op("andpd");
		constexpr auto ANDNPD       = Op("andnpd");
		constexpr auto ORPD         = Op("orpd");
		constexpr auto XORPD        = Op("xorpd");
		constexpr auto SQRTSD       = Op("sqrtsd");

		constexpr auto PUNPCKLBW    = Op("punpcklbw");
		constexpr auto PUNPCKLWD    = Op("punpcklwd");
		constexpr auto PUNPCKLDQ    = Op("punpckldq");
		constexpr auto PACKSSWB     = Op("packsswb");
		constexpr auto PCMPGTB      = Op("pcmpgtb");
		constexpr auto PCMPGTW      = Op("pcmpgtw");
		constexpr auto PCMPGTD      = Op("pcmpgtd");
		constexpr auto PACKUSWB     = Op("packuswb");

		constexpr auto PSHUFW       = Op("pshufw");
		constexpr auto PSHUFHW      = Op("pshufhw");
		constexpr auto PSHUFD       = Op("pshufd");
		constexpr auto PSHUFLW      = Op("pshuflw");
		constexpr auto PCMPEQB      = Op("pcmpeqb");
		constexpr auto PCMPEQW      = Op("pcmpeqw");
		constexpr auto PCMPEQD      = Op("pcmpeqd");
		constexpr auto EMMS         = Op("emms");

		constexpr auto CMPPS        = Op("cmpps");
		constexpr auto CMPSS        = Op("cmpss");
		constexpr auto CMPPD        = Op("cmppd");
		constexpr auto MOVNTI       = Op("movnti");
		constexpr auto PINSRW       = Op("pinsrw");
		constexpr auto PINSRQ       = Op("pinsrq");
		constexpr auto PEXTRW       = Op("pextrw");
		constexpr auto SHUFPS       = Op("pinsrw");
		constexpr auto SHUFPD       = Op("pinsrw");
		constexpr auto PSRLW        = Op("psrlw");
		constexpr auto PSRLD        = Op("psrld");
		constexpr auto PSRLQ        = Op("psrlq");
		constexpr auto PSRLDQ       = Op("psrldq");
		constexpr auto PADDQ        = Op("paddq");
		constexpr auto PMULLW       = Op("pmullw");
		constexpr auto PMOVMSKB     = Op("pmovmskb");
		constexpr auto MOVQ2DQ      = Op("movq2dq");;
		constexpr auto ADDSUBPD     = Op("addsubpd");
		constexpr auto MOVQ         = Op("movq");
		constexpr auto ADDSUBPS     = Op("addsubps");
		constexpr auto MOVDQ2Q      = Op("movdq2q");
		constexpr auto PAVGB        = Op("pavgb");
		constexpr auto PSRAW        = Op("psraw");
		constexpr auto PSRAD        = Op("psrad");
		constexpr auto PAVGW        = Op("pavgw");
		constexpr auto PMULHUW      = Op("pmulhuw");
		constexpr auto PMULHW       = Op("pmulhw");
		constexpr auto MOVNTQ       = Op("movntq");
		constexpr auto CVTDQ2PS     = Op("cvtdq2ps");
		constexpr auto CVTDQ2PD     = Op("cvtdq2pd");
		constexpr auto CVTTPD2DQ    = Op("cvttpd2dq");
		constexpr auto MOVNTDQ      = Op("movntdq");
		constexpr auto CVTPD2DQ     = Op("cvtpd2dq");
		constexpr auto PSLLW        = Op("psllw");
		constexpr auto PSLLD        = Op("pslld");
		constexpr auto PSLLQ        = Op("psllq");
		constexpr auto PSLLDQ       = Op("pslldq");
		constexpr auto PMULUDQ      = Op("pmuludq");
		constexpr auto PMADDWD      = Op("pmaddwd");
		constexpr auto PSADBW       = Op("psadbw");
		constexpr auto MASKMOVQ     = Op("maskmovq");
		constexpr auto MASKMOVDQU   = Op("maskmovdqu");
		constexpr auto LDDQU        = Op("lddqu");
		constexpr auto MOVAPS       = Op("movaps");
		constexpr auto MOVAPD       = Op("movapd");
		constexpr auto CVTPI2PS     = Op("cvtpi2ps");
		constexpr auto MOVNTPS      = Op("movntps");
		constexpr auto CVTTPS2PI    = Op("cvttps2pi");
		constexpr auto CVTPS2PI     = Op("cvtps2pi");
		constexpr auto UCOMISS      = Op("ucomiss");
		constexpr auto COMISS       = Op("comiss");
		constexpr auto CVTSI2SS     = Op("cvtsi2ss");
		constexpr auto MOVNTSS      = Op("movntss");
		constexpr auto CVTTSS2SI    = Op("cvttss2si");
		constexpr auto CVTSS2SI     = Op("cvtss2si");
		constexpr auto CVTPI2PD     = Op("cvtpi2pd");
		constexpr auto MOVNTPD      = Op("movntpd");
		constexpr auto CVTTPD2PI    = Op("cvttpd2pi");
		constexpr auto CVTPD2PI     = Op("cvtpd2pi");
		constexpr auto UCOMISD      = Op("ucomisd");
		constexpr auto COMISD       = Op("comisd");
		constexpr auto CVTSI2SD     = Op("cvtsi2sd");
		constexpr auto MOVNTSD      = Op("movntsd");
		constexpr auto CVTTSD2SI    = Op("cvttsd2si");
		constexpr auto CVTSD2SI     = Op("cvtsd2si");
		constexpr auto ADDPS        = Op("addps");
		constexpr auto MULPS        = Op("mulps");
		constexpr auto CVTPS2PD     = Op("cvtps2pd");
		constexpr auto CVTPQ2PS     = Op("cvtpq2ps");
		constexpr auto SUBPS        = Op("subps");
		constexpr auto MINPS        = Op("minps");
		constexpr auto DIVPS        = Op("divps");
		constexpr auto MAXPS        = Op("maxps");
		constexpr auto ADDSS        = Op("addss");
		constexpr auto MULSS        = Op("mulss");
		constexpr auto CVTSS2SD     = Op("cvtss2sd");
		constexpr auto CVTTPS2DQ    = Op("cvttps2dq");
		constexpr auto SUBSS        = Op("subss");
		constexpr auto MINSS        = Op("minss");
		constexpr auto DIVSS        = Op("divss");
		constexpr auto MAXSS        = Op("maxss");
		constexpr auto ADDPD        = Op("addpd");
		constexpr auto MULPD        = Op("mulpd");
		constexpr auto CVTPD2PS     = Op("cvtpd2ps");
		constexpr auto CVTPS2DQ     = Op("cvtps2dq");
		constexpr auto SUBPD        = Op("subpd");
		constexpr auto MINPD        = Op("minpd");
		constexpr auto DIVPD        = Op("divpd");
		constexpr auto MAXPD        = Op("maxpd");
		constexpr auto ADDSD        = Op("addsd");
		constexpr auto MULSD        = Op("mulsd");
		constexpr auto CVTSD2SS     = Op("cvtsd2ss");
		constexpr auto SUBSD        = Op("subsd");
		constexpr auto MINSD        = Op("minsd");
		constexpr auto DIVSD        = Op("divsd");
		constexpr auto MAXSD        = Op("maxsd");
		constexpr auto PUNPCKHBW    = Op("punpckhbw");
		constexpr auto PUNPCKHWD    = Op("punpckhwd");
		constexpr auto PUNPCKHDQ    = Op("punpckhdq");
		constexpr auto PACKSSDW     = Op("packssdw");
		constexpr auto MOVD         = Op("movd");
		constexpr auto MOVDQU       = Op("movdqu");
		constexpr auto PUNPCKLQDQ   = Op("punpcklqdq");
		constexpr auto PUNPCKHQDQ   = Op("punpckhqdq");
		constexpr auto MOVDQA       = Op("movdqa");
		constexpr auto EXTRQ        = Op("extrq");
		constexpr auto HADDPD       = Op("haddpd");
		constexpr auto HSUBPD       = Op("hsubpd");
		constexpr auto INSERTQ      = Op("insertq");
		constexpr auto HADDPS       = Op("haddps");
		constexpr auto HSUBPS       = Op("hsubps");
		constexpr auto TZCNT        = Op("tzcnt");
		constexpr auto LZCNT        = Op("lzcnt");
		constexpr auto POPCNT       = Op("popcnt");
		constexpr auto PSUBUSB      = Op("psubusb");
		constexpr auto PSUBUSW      = Op("psubusw");
		constexpr auto PMINUB       = Op("pminub");
		constexpr auto PAND         = Op("pand");
		constexpr auto PADDUSB      = Op("paddusb");
		constexpr auto PADDUSW      = Op("paddusw");
		constexpr auto PMAXUB       = Op("pmaxub");
		constexpr auto PANDN        = Op("pandn");
		constexpr auto PSUBSB       = Op("psubsb");
		constexpr auto PSUBSW       = Op("psubsw");
		constexpr auto PMINSW       = Op("pminsw");
		constexpr auto POR          = Op("por");
		constexpr auto PADDSB       = Op("paddsb");
		constexpr auto PADDSW       = Op("paddsw");
		constexpr auto PMAXSW       = Op("pmaxsw");
		constexpr auto PXOR         = Op("pxor");
		constexpr auto PSUBB        = Op("psubb");
		constexpr auto PSUBW        = Op("psubw");
		constexpr auto PSUBD        = Op("psubd");
		constexpr auto PSUBQ        = Op("psubq");
		constexpr auto PADDB        = Op("paddb");
		constexpr auto PADDW        = Op("paddw");
		constexpr auto PADDD        = Op("paddd");

		constexpr auto PSHUFB       = Op("pshufb");
		constexpr auto PHADDW       = Op("phaddw");
		constexpr auto PHADDD       = Op("phaddd");
		constexpr auto PHADDSW      = Op("phaddsw");
		constexpr auto PMADDUBSW    = Op("pmaddubsw");
		constexpr auto PHSUBW       = Op("phsubw");
		constexpr auto PHSUBD       = Op("phsubd");
		constexpr auto PHSUBSW      = Op("phsubsw");

		constexpr auto PSIGNB       = Op("psignb");
		constexpr auto PSIGNW       = Op("psignw");
		constexpr auto PSIGND       = Op("psignd");
		constexpr auto PMULHRSW     = Op("pmulhrsw");

		constexpr auto PBLENDVB     = Op("pblendvb");
		constexpr auto BLENDVPS     = Op("blendvps");
		constexpr auto BLENDVPD     = Op("blendvpd");
		constexpr auto PTEST        = Op("ptest");
		constexpr auto PMOVSXBW     = Op("pmovsxbw");
		constexpr auto PMOVSXBD     = Op("pmovsxbd");
		constexpr auto PMOVSXBQ     = Op("pmovsxbq");
		constexpr auto PMOVSXWD     = Op("pmovsxwd");
		constexpr auto PMOVSXWQ     = Op("pmovsxwq");
		constexpr auto PMOVSXDQ     = Op("pmovsxdq");
		constexpr auto PMOVZXBW     = Op("pmovzxbw");
		constexpr auto PMOVZXBD     = Op("pmovzxbd");
		constexpr auto PMOVZXBQ     = Op("pmovzxbq");
		constexpr auto PMOVZXWD     = Op("pmovzxwd");
		constexpr auto PMOVZXWQ     = Op("pmovzxwq");
		constexpr auto PMOVZXDQ     = Op("pmovzxdq");
		constexpr auto PCMPGTQ      = Op("pcmpgtq");
		constexpr auto PMULLD       = Op("pmulld");
		constexpr auto PHMINPOSUW   = Op("phminposuw");
		constexpr auto MOVBE        = Op("movbe");
		constexpr auto CRC32        = Op("crc32");
		constexpr auto PABSB        = Op("pabsb");
		constexpr auto PABSW        = Op("pabsw");
		constexpr auto PABSD        = Op("pabsd");
		constexpr auto PMULDQ       = Op("pmuldq");
		constexpr auto PCMPEQQ      = Op("pcmpeqq");
		constexpr auto MOVNTDQA     = Op("movntdqa");
		constexpr auto PACKUSDW     = Op("packusdw");
		constexpr auto PMINSB       = Op("pminsb");
		constexpr auto PMINSD       = Op("pminsd");
		constexpr auto PMINUW       = Op("pminuw");
		constexpr auto PMINUD       = Op("pminud");
		constexpr auto PMAXSB       = Op("pmaxsb");
		constexpr auto PMAXSD       = Op("pmaxsd");
		constexpr auto PMAXUW       = Op("pmaxuw");
		constexpr auto PMAXUD       = Op("pmaxud");
		constexpr auto AESIMC       = Op("aesimc");
		constexpr auto AESENC       = Op("aesenc");
		constexpr auto AESENCLAST   = Op("aesenclast");
		constexpr auto AESDEC       = Op("aesdec");
		constexpr auto AESDECLAST   = Op("aesdeclast");
		constexpr auto PEXTRB       = Op("pextrb");
		constexpr auto PEXTRD       = Op("pextrd");
		constexpr auto EXTRACTPS    = Op("extractps");
		constexpr auto PINSRB       = Op("pinsrb");
		constexpr auto INSERTPS     = Op("insertps");
		constexpr auto PINSRD       = Op("pinsrd");
		constexpr auto DPPS         = Op("dpps");
		constexpr auto DPPD         = Op("dppd");
		constexpr auto MPSADBW      = Op("mpsadbw");
		constexpr auto PCLMULQDQ    = Op("pclmulqdq");
		constexpr auto PCMPESTRM    = Op("pcmpestrm");
		constexpr auto PCMPESTRI    = Op("pcmpestri");
		constexpr auto PCMPISTRM    = Op("pcmpistrm");
		constexpr auto PCMPISTRI    = Op("pcmpistri");
		constexpr auto PALIGNR      = Op("palignr");
		constexpr auto ROUNDPS      = Op("roundps");
		constexpr auto ROUNDPD      = Op("roundpd");
		constexpr auto ROUNDSS      = Op("roundss");
		constexpr auto ROUNDSD      = Op("roundsd");
		constexpr auto BLENDPS      = Op("blendps");
		constexpr auto BLENDPD      = Op("blendpd");
		constexpr auto PBLENDW      = Op("pblendw");

		// uwu
		constexpr auto AESKEYGENASSIST  = Op("aeskeygenassist");


		// 3dnow
		constexpr auto PFCMPGE      = Op("pfcmpge");
		constexpr auto PFMIN        = Op("pfmin");
		constexpr auto PFRCP        = Op("pfrcp");
		constexpr auto PFRSQRT      = Op("pfrsqrt");
		constexpr auto PFCMPGT      = Op("pfcmpgt");
		constexpr auto PFMAX        = Op("pfmax");
		constexpr auto PFRCPIT1     = Op("pfrcpit1");
		constexpr auto PFRSQIT1     = Op("pfrsqit1");
		constexpr auto PFCMPEQ      = Op("pfcmpeq");
		constexpr auto PFMUL        = Op("pfmul");
		constexpr auto PFRCPIT2     = Op("pfrcpit2");
		constexpr auto PMULHRW      = Op("pmulhrw");
		constexpr auto PI2FW        = Op("pi2fw");
		constexpr auto PI2FD        = Op("pi2fd");
		constexpr auto PF2IW        = Op("pf2iw");
		constexpr auto PF2ID        = Op("pf2id");
		constexpr auto PFNACC       = Op("pfnacc");
		constexpr auto PFPNACC      = Op("pfpnacc");
		constexpr auto PFSUB        = Op("pfsub");
		constexpr auto PFADD        = Op("pfadd");
		constexpr auto PFSUBR       = Op("pfsubr");
		constexpr auto PFACC        = Op("pfacc");
		constexpr auto PSWAPD       = Op("pswapd");
		constexpr auto PAVGUSB      = Op("pavgusb");


		// x87
		constexpr auto FLD          = Op("fld");
		constexpr auto FST          = Op("fst");
		constexpr auto FSTP         = Op("fstp");
		constexpr auto FLDENV       = Op("fldenv");
		constexpr auto FLDCW        = Op("fldcw");
		constexpr auto FNSTENV      = Op("fnstenv");
		constexpr auto FNSTCW       = Op("fnstcw");
		constexpr auto FXCH         = Op("fxch");
		constexpr auto FNOP         = Op("fnop");
		constexpr auto FCHS         = Op("fchs");
		constexpr auto FABS         = Op("fabs");
		constexpr auto FTST         = Op("ftst");
		constexpr auto FXAM         = Op("fxam");
		constexpr auto FLD1         = Op("fld1");
		constexpr auto FLDL2T       = Op("fldl2t");
		constexpr auto FLDL2E       = Op("fldl2e");
		constexpr auto FLDPI        = Op("fldpi");
		constexpr auto FLDLG2       = Op("fldlg2");
		constexpr auto FLDLN2       = Op("fldln2");
		constexpr auto FLDZ         = Op("fldz");
		constexpr auto F2XM1        = Op("f2xm1");
		constexpr auto FYL2X        = Op("fyl2x");
		constexpr auto FPTAN        = Op("fptan");
		constexpr auto FPATAN       = Op("fpatan");
		constexpr auto FXTRACT      = Op("fxtract");
		constexpr auto FPREM1       = Op("fprem1");
		constexpr auto FDECSTP      = Op("fdecstp");
		constexpr auto FINCSTP      = Op("fincstp");
		constexpr auto FPREM        = Op("fprem");
		constexpr auto FYL2XP1      = Op("fyl2xp1");
		constexpr auto FSQRT        = Op("fsqrt");
		constexpr auto FSINCOS      = Op("fsincos");
		constexpr auto FRNDINT      = Op("frndint");
		constexpr auto FSCALE       = Op("fscale");
		constexpr auto FSIN         = Op("fsin");
		constexpr auto FCOS         = Op("fcos");
		constexpr auto FIADD        = Op("fiadd");
		constexpr auto FIMUL        = Op("fimul");
		constexpr auto FICOM        = Op("ficom");
		constexpr auto FICOMP       = Op("ficomp");
		constexpr auto FISUB        = Op("fisub");
		constexpr auto FISUBR       = Op("fisubr");
		constexpr auto FIDIV        = Op("fidiv");
		constexpr auto FIDIVR       = Op("fidivr");
		constexpr auto FCMOVB       = Op("fcmovb");
		constexpr auto FCMOVE       = Op("fcmove");
		constexpr auto FCMOVBE      = Op("fcmovbe");
		constexpr auto FCMOVU       = Op("fcmovu");
		constexpr auto FUCOMPP      = Op("fucompp");
		constexpr auto FILD         = Op("fild");
		constexpr auto FISTTP       = Op("fisttp");
		constexpr auto FIST         = Op("fist");
		constexpr auto FISTP        = Op("fistp");
		constexpr auto FCMOVNB      = Op("fcmovnb");
		constexpr auto FCMOVNE      = Op("fcmovne");
		constexpr auto FCMOVNBE     = Op("fcmovnbe");
		constexpr auto FCMOVNU      = Op("fcmovnu");
		constexpr auto FNCLEX       = Op("fnclex");
		constexpr auto FNINIT       = Op("fninit");
		constexpr auto FUCOMI       = Op("fucomi");
		constexpr auto FCOMI        = Op("fcomi");
		constexpr auto FADD         = Op("fadd");
		constexpr auto FMUL         = Op("fmul");
		constexpr auto FCOM         = Op("fcom");
		constexpr auto FCOMP        = Op("fcomp");
		constexpr auto FSUB         = Op("fsub");
		constexpr auto FSUBR        = Op("fsubr");
		constexpr auto FDIV         = Op("fdiv");
		constexpr auto FDIVR        = Op("fdivr");
		constexpr auto FRSTOR       = Op("frstor");
		constexpr auto FFREE        = Op("ffree");
		constexpr auto FUCOM        = Op("fucom");
		constexpr auto FUCOMP       = Op("fucomp");
		constexpr auto FNSAVE       = Op("fnsave");
		constexpr auto FNSTSW       = Op("fnstsw");
		constexpr auto FADDP        = Op("faddp");
		constexpr auto FMULP        = Op("fmulp");
		constexpr auto FCOMPP       = Op("fcompp");
		constexpr auto FSUBRP       = Op("fsubrp");
		constexpr auto FSUBP        = Op("fsubp");
		constexpr auto FDIVRP       = Op("fdivrp");
		constexpr auto FDIVP        = Op("fdivp");
		constexpr auto FBLD         = Op("fbld");
		constexpr auto FBSTP        = Op("fbstp");
		constexpr auto FUCOMIP      = Op("fucomip");
		constexpr auto FCOMIP       = Op("fcomip");
	}
}
