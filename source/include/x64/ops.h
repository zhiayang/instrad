// ops.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "defs.h"
#include <string.h>

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
		constexpr auto ADD      = Op("add");
		constexpr auto ADC      = Op("adc");
		constexpr auto AND      = Op("and");
		constexpr auto XOR      = Op("xor");
		constexpr auto INC      = Op("inc");
		constexpr auto PUSH     = Op("push");
		constexpr auto PUSHA    = Op("pusha");
		constexpr auto PUSHAD   = Op("pushad");
		constexpr auto POPA     = Op("popa");
		constexpr auto POPAD    = Op("popad");
		constexpr auto TEST     = Op("test");
		constexpr auto XCHG     = Op("xchg");
		constexpr auto MOV      = Op("mov");
		constexpr auto MOVSB    = Op("movsb");
		constexpr auto MOVSW    = Op("movsw");
		constexpr auto MOVSD    = Op("movsd");
		constexpr auto MOVSQ    = Op("movsq");
		constexpr auto CMPSB    = Op("cmpsb");
		constexpr auto CMPSW    = Op("cmpsw");
		constexpr auto CMPSD    = Op("cmpsd");
		constexpr auto CMPSQ    = Op("cmpsq");
		constexpr auto RET      = Op("ret");
		constexpr auto RETF     = Op("retf");
		constexpr auto LOOPNZ   = Op("loopnz");
		constexpr auto LOOPZ    = Op("loopz");
		constexpr auto LOOP     = Op("loop");
		constexpr auto IN       = Op("in");
		constexpr auto OUT      = Op("out");
		constexpr auto OR       = Op("or");
		constexpr auto SBB      = Op("sbb");
		constexpr auto SUB      = Op("sub");
		constexpr auto CMP      = Op("cmp");
		constexpr auto DEC      = Op("dec");
		constexpr auto POP      = Op("pop");
		constexpr auto INSB     = Op("insb");
		constexpr auto INSW     = Op("insw");
		constexpr auto INSD     = Op("insd");
		constexpr auto OUTSB    = Op("outsb");
		constexpr auto OUTSW    = Op("outsw");
		constexpr auto OUTSD    = Op("outsd");
		constexpr auto JS       = Op("js");
		constexpr auto JNS      = Op("jns");
		constexpr auto JP       = Op("jp");
		constexpr auto JNP      = Op("jnp");
		constexpr auto JL       = Op("jl");
		constexpr auto JNL      = Op("jnl");
		constexpr auto JLE      = Op("jle");
		constexpr auto JNLE     = Op("jnle");
		constexpr auto JGE      = Op("jge");
		constexpr auto JG       = Op("jg");
		constexpr auto LEA      = Op("lea");
		constexpr auto CBW      = Op("cbw");
		constexpr auto CWDE     = Op("cwde");
		constexpr auto CDQE     = Op("cdqe");
		constexpr auto CWD      = Op("cwd");
		constexpr auto CDQ      = Op("cdq");
		constexpr auto CQO      = Op("cqo");
		constexpr auto CALL     = Op("call");
		constexpr auto CALLF    = Op("callf");
		constexpr auto FWAIT    = Op("fwait");
		constexpr auto PUSHF    = Op("pushf");
		constexpr auto POPF     = Op("popf");
		constexpr auto SAHF     = Op("sahf");
		constexpr auto LAHF     = Op("lahf");
		constexpr auto STOSB    = Op("stosb");
		constexpr auto STOSW    = Op("stosw");
		constexpr auto STOSD    = Op("stosd");
		constexpr auto STOSQ    = Op("stosq");
		constexpr auto LODSB    = Op("lodsb");
		constexpr auto LODSW    = Op("lodsw");
		constexpr auto LODSD    = Op("lodsd");
		constexpr auto LODSQ    = Op("lodsq");
		constexpr auto SCASB    = Op("scasb");
		constexpr auto SCASW    = Op("scasw");
		constexpr auto SCASD    = Op("scasd");
		constexpr auto SCASQ    = Op("scasq");
		constexpr auto ENTER    = Op("enter");
		constexpr auto LEAVE    = Op("leave");
		constexpr auto INT      = Op("int");
		constexpr auto INT3     = Op("int3");
		constexpr auto IRET     = Op("iret");
		constexpr auto JMP      = Op("jmp");

		constexpr auto PAUSE    = Op("pause");

		constexpr auto IMUL     = Op("imul");
		constexpr auto IDIV     = Op("idiv");


		constexpr auto MUL      = Op("mul");
		constexpr auto DIV      = Op("div");

		constexpr auto DAA      = Op("daa");
		constexpr auto AAA      = Op("aaa");
		constexpr auto DAS      = Op("das");
		constexpr auto AAS      = Op("aas");
		constexpr auto AAM      = Op("aam");
		constexpr auto AAD      = Op("aad");
		constexpr auto BOUND    = Op("bound");
		constexpr auto INTO     = Op("into");

		constexpr auto SYSCALL  = Op("syscall");
		constexpr auto SYSRET   = Op("sysret");
		constexpr auto CLTS     = Op("clts");
		constexpr auto XLAT     = Op("xlat");
		constexpr auto JECXZ    = Op("jecxz");

		constexpr auto RDMSR    = Op("rdmsr");
		constexpr auto WRMSR    = Op("wrmsr");
		constexpr auto RDTSC    = Op("rdtsc");
		constexpr auto RDPMC    = Op("rdpmc");

		constexpr auto JO       = Op("jo");
		constexpr auto JNO      = Op("jno");
		constexpr auto JB       = Op("jb");
		constexpr auto JNB      = Op("jnb");
		constexpr auto JBO      = Op("jbo");
		constexpr auto JZ       = Op("jz");
		constexpr auto JNZ      = Op("jnz");
		constexpr auto JA       = Op("ja");
		constexpr auto JNA      = Op("jna");
		constexpr auto SETO     = Op("seto");
		constexpr auto SETNO    = Op("setno");
		constexpr auto SETB     = Op("setb");
		constexpr auto SETNB    = Op("setnb");
		constexpr auto SETZ     = Op("setz");
		constexpr auto SETNZ    = Op("setnz");
		constexpr auto SETBE    = Op("setbe");
		constexpr auto SETNBE   = Op("setnbe");
		constexpr auto SETS     = Op("sets");
		constexpr auto SETNS    = Op("setns");
		constexpr auto SETNA    = Op("setna");
		constexpr auto SETA     = Op("seta");
		constexpr auto SETP     = Op("setp");
		constexpr auto SETNP    = Op("setnp");
		constexpr auto SETL     = Op("setl");
		constexpr auto SETNL    = Op("setnl");
		constexpr auto SETGE    = Op("setge");
		constexpr auto SETG     = Op("setg");
		constexpr auto SETLE    = Op("setle");
		constexpr auto SETNLE   = Op("setnle");

		constexpr auto CPUID    = Op("cpuid");
		constexpr auto CMPXCHG  = Op("cmpxchg");
		constexpr auto MOVZX    = Op("movzx");
		constexpr auto MOVSXD   = Op("movsxd");
		constexpr auto ICEBP    = Op("icebp");
		constexpr auto HLT      = Op("hlt");
		constexpr auto CMC      = Op("cmc");
		constexpr auto CLC      = Op("clc");
		constexpr auto STC      = Op("stc");
		constexpr auto CLI      = Op("cli");
		constexpr auto STI      = Op("sti");
		constexpr auto CLD      = Op("cld");
		constexpr auto STD      = Op("std");

		constexpr auto BT       = Op("bt");
		constexpr auto BSWAP    = Op("bswap");
		constexpr auto BTC      = Op("btc");
		constexpr auto BTR      = Op("btr");
		constexpr auto BTS      = Op("bts");
		constexpr auto BSF      = Op("bsf");
		constexpr auto BSR      = Op("bsr");
		constexpr auto NOT      = Op("not");
		constexpr auto NEG      = Op("neg");

		constexpr auto ROL      = Op("rol");
		constexpr auto ROR      = Op("ror");
		constexpr auto RCL      = Op("rcl");
		constexpr auto RCR      = Op("rcr");
		constexpr auto SHL      = Op("shl");
		constexpr auto SHR      = Op("shr");
		constexpr auto SAL      = Op("sal");
		constexpr auto SAR      = Op("sar");
		constexpr auto SHLD     = Op("shld");
		constexpr auto SHRD     = Op("shrd");

		constexpr auto LAR      = Op("lar");
		constexpr auto LSL      = Op("lsl");

		constexpr auto INVD     = Op("invd");
		constexpr auto WBINVD   = Op("wbinvd");
		constexpr auto UD1      = Op("ud1");
		constexpr auto UD2      = Op("ud2");
		constexpr auto PREFETCH = Op("prefetch");
		constexpr auto FEMMS    = Op("femms");
		constexpr auto NOP      = Op("nop");
		constexpr auto SYSENTER = Op("sysenter");
		constexpr auto SYSEXIT  = Op("sysexit");
		constexpr auto RSM      = Op("rsm");
		constexpr auto SMSW     = Op("smsw");
		constexpr auto LMSW     = Op("lmsw");

		constexpr auto SWAPGS   = Op("swapgs");
		constexpr auto RDTSCP   = Op("rdtscp");

		constexpr auto INVLPG   = Op("invlpg");
		constexpr auto INVLPGA  = Op("invlpga");

		constexpr auto MONITOR  = Op("monitor");
		constexpr auto MONITORX = Op("monitorx");
		constexpr auto MWAIT    = Op("mwait");
		constexpr auto MWAITX   = Op("mwaitx");

		constexpr auto XGETBV   = Op("xgetbv");
		constexpr auto XSETBV   = Op("xsetbv");

		constexpr auto VMCALL   = Op("vmcall");
		constexpr auto VMLOAD   = Op("vmload");
		constexpr auto VMSAVE   = Op("vmsave");
		constexpr auto VMRUN    = Op("vmrun");

		constexpr auto STGI     = Op("stgi");
		constexpr auto CLGI     = Op("clgi");
		constexpr auto SKINIT   = Op("skinit");

		constexpr auto FXSAVE   = Op("fxsave");
		constexpr auto FXRSTOR  = Op("fxrstor");
		constexpr auto LDMXCSR  = Op("ldmxcsr");
		constexpr auto STMXCSR  = Op("stmxcsr");
		constexpr auto XSAVE    = Op("xsave");
		constexpr auto XRSTOR   = Op("xrstor");
		constexpr auto XSAVEOPT = Op("xsaveopt");

		constexpr auto LFENCE   = Op("lfence");
		constexpr auto SFENCE   = Op("sfence");
		constexpr auto MFENCE   = Op("mfence");
		constexpr auto CLFLUSH  = Op("clflush");

		constexpr auto RDFSBASE = Op("rdfsbase");
		constexpr auto RDGSBASE = Op("rdgsbase");
		constexpr auto WRFSBASE = Op("wrfsbase");
		constexpr auto WRGSBASE = Op("wrgsbase");

		constexpr auto LDS      = Op("lds");
		constexpr auto LES      = Op("les");
		constexpr auto LFS      = Op("lfs");
		constexpr auto LGS      = Op("lgs");
		constexpr auto LSS      = Op("lss");

		constexpr auto SLDT     = Op("sldt");
		constexpr auto STR      = Op("str");
		constexpr auto LLDT     = Op("lldt");
		constexpr auto LTR      = Op("ltr");

		constexpr auto SGDT     = Op("sgdt");
		constexpr auto SIDT     = Op("sidt");
		constexpr auto LGDT     = Op("lgdt");
		constexpr auto LIDT     = Op("lidt");

		constexpr auto VERR     = Op("verr");
		constexpr auto VERW     = Op("verw");

		constexpr auto CMOVO    = Op("cmovo");
		constexpr auto CMOVNO   = Op("cmovno");
		constexpr auto CMOVB    = Op("cmovb");
		constexpr auto CMOVNB   = Op("cmovnb");
		constexpr auto CMOVZ    = Op("cmovz");
		constexpr auto CMOVNZ   = Op("cmovnz");
		constexpr auto CMOVBE   = Op("cmovbe");
		constexpr auto CMOVNBE  = Op("cmovnbe");
		constexpr auto CMOVS    = Op("cmovs");
		constexpr auto CMOVNS   = Op("cmovns");
		constexpr auto CMOVP    = Op("cmovp");
		constexpr auto CMOVNP   = Op("cmovnp");
		constexpr auto CMOVA    = Op("cmova");
		constexpr auto CMOVNA   = Op("cmovna");
		constexpr auto CMOVL    = Op("cmovl");
		constexpr auto CMOVLE   = Op("cmovle");
		constexpr auto CMOVG    = Op("cmovg");
		constexpr auto CMOVGE   = Op("cmovge");


		constexpr auto NONE     = Op("");
		constexpr auto INVALID  = Op("??");
	}
}
