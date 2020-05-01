// regs.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "defs.h"

namespace instrad::x64
{
	struct Register
	{
		constexpr Register(int idx, int width, const char* name) : m_index(idx), m_width(width), m_name(name) { }

		constexpr const char* name() const { return this->m_name; }
		constexpr int width() const { return this->m_width; }

		constexpr bool present() const { return this->m_index >= 0; }

	private:
		int m_index;
		int m_width;
		const char* m_name;
	};

	namespace regs
	{
		// 0=ax, 1=cx, 2=dx, 3=bx, 4=sp, 5=bp, 6=si, 7=di, 8=r8, etc, the rest are sane.

		constexpr int REG_FLAG_IP       = 0x80;
		constexpr int REG_FLAG_SEGMENT  = 0x100;
		constexpr int REG_FLAG_CONTROL  = 0x200;
		constexpr int REG_FLAG_DEBUG    = 0x400;

		constexpr auto  AL      = Register(0,  8,  "al");
		constexpr auto  AH      = Register(0,  8,  "ah");
		constexpr auto  AX      = Register(0, 16,  "ax");
		constexpr auto EAX      = Register(0, 32, "eax");
		constexpr auto RAX      = Register(0, 64, "rax");

		constexpr auto  CL      = Register(1,  8,  "cl");
		constexpr auto  CH      = Register(1,  8,  "ch");
		constexpr auto  CX      = Register(1, 16,  "cx");
		constexpr auto ECX      = Register(1, 32, "ecx");
		constexpr auto RCX      = Register(1, 64, "rcx");

		constexpr auto  DL      = Register(2,  8,  "dl");
		constexpr auto  DH      = Register(2,  8,  "dh");
		constexpr auto  DX      = Register(2, 16,  "dx");
		constexpr auto EDX      = Register(2, 32, "edx");
		constexpr auto RDX      = Register(2, 64, "rdx");

		constexpr auto  BL      = Register(3,  8,  "bl");
		constexpr auto  BH      = Register(3,  8,  "bh");
		constexpr auto  BX      = Register(3, 16,  "bx");
		constexpr auto EBX      = Register(3, 32, "ebx");
		constexpr auto RBX      = Register(3, 64, "rbx");

		constexpr auto  SPL     = Register(4,  8,  "spl");
		constexpr auto  SP      = Register(4, 16,  "sp");
		constexpr auto ESP      = Register(4, 32, "esp");
		constexpr auto RSP      = Register(4, 64, "rsp");

		constexpr auto  BPL     = Register(5,  8,  "bpl");
		constexpr auto  BP      = Register(5, 16,  "bp");
		constexpr auto EBP      = Register(5, 32, "ebp");
		constexpr auto RBP      = Register(5, 64, "rbp");

		constexpr auto  SIL     = Register(6,  8,  "sil");
		constexpr auto  SI      = Register(6, 16,  "si");
		constexpr auto ESI      = Register(6, 32, "esi");
		constexpr auto RSI      = Register(6, 64, "rsi");

		constexpr auto  DIL     = Register(7,  8,  "dil");
		constexpr auto  DI      = Register(7, 16,  "di");
		constexpr auto EDI      = Register(7, 32, "edi");
		constexpr auto RDI      = Register(7, 64, "rdi");

		constexpr auto R8B      = Register(8,  8, "r8b");
		constexpr auto R8W      = Register(8, 16, "r8w");
		constexpr auto R8D      = Register(8, 32, "r8d");
		constexpr auto R8       = Register(8, 64, "r8");

		constexpr auto R9B      = Register(9,  8, "r9b");
		constexpr auto R9W      = Register(9, 16, "r9w");
		constexpr auto R9D      = Register(9, 32, "r9d");
		constexpr auto R9       = Register(9, 64, "r9");

		constexpr auto R10B     = Register(10,  8, "r10b");
		constexpr auto R10W     = Register(10, 16, "r10w");
		constexpr auto R10D     = Register(10, 32, "r10d");
		constexpr auto R10      = Register(10, 64, "r10");

		constexpr auto R11B     = Register(11,  8, "r11b");
		constexpr auto R11W     = Register(11, 16, "r11w");
		constexpr auto R11D     = Register(11, 32, "r11d");
		constexpr auto R11      = Register(11, 64, "r11");

		constexpr auto R12B     = Register(12,  8, "r12b");
		constexpr auto R12W     = Register(12, 16, "r12w");
		constexpr auto R12D     = Register(12, 32, "r12d");
		constexpr auto R12      = Register(12, 64, "r12");

		constexpr auto R13B     = Register(13,  8, "r13b");
		constexpr auto R13W     = Register(13, 16, "r13w");
		constexpr auto R13D     = Register(13, 32, "r13d");
		constexpr auto R13      = Register(13, 64, "r13");

		constexpr auto R14B     = Register(14,  8, "r14b");
		constexpr auto R14W     = Register(14, 16, "r14w");
		constexpr auto R14D     = Register(14, 32, "r14d");
		constexpr auto R14      = Register(14, 64, "r14");

		constexpr auto R15B     = Register(15,  8, "r15b");
		constexpr auto R15W     = Register(15, 16, "r15w");
		constexpr auto R15D     = Register(15, 32, "r15d");
		constexpr auto R15      = Register(15, 64, "r15");

		// not a real register that you can use, but we need to be able to represent this.
		constexpr auto  IP      = Register(REG_FLAG_IP | 0x00, 16,  "ip");
		constexpr auto EIP      = Register(REG_FLAG_IP | 0x00, 32, "eip");
		constexpr auto RIP      = Register(REG_FLAG_IP | 0x00, 64, "rip");

		constexpr auto CS       = Register(REG_FLAG_SEGMENT | 0x00, 16, "cs");
		constexpr auto DS       = Register(REG_FLAG_SEGMENT | 0x01, 16, "ds");
		constexpr auto ES       = Register(REG_FLAG_SEGMENT | 0x02, 16, "es");
		constexpr auto FS       = Register(REG_FLAG_SEGMENT | 0x03, 16, "fs");
		constexpr auto GS       = Register(REG_FLAG_SEGMENT | 0x04, 16, "gs");
		constexpr auto SS       = Register(REG_FLAG_SEGMENT | 0x05, 16, "ss");

		constexpr auto CR0      = Register(REG_FLAG_CONTROL | 0x00, 64, "cr0");
		constexpr auto CR1      = Register(REG_FLAG_CONTROL | 0x01, 64, "cr1");
		constexpr auto CR2      = Register(REG_FLAG_CONTROL | 0x02, 64, "cr2");
		constexpr auto CR3      = Register(REG_FLAG_CONTROL | 0x03, 64, "cr3");
		constexpr auto CR4      = Register(REG_FLAG_CONTROL | 0x04, 64, "cr4");
		constexpr auto CR5      = Register(REG_FLAG_CONTROL | 0x05, 64, "cr5");
		constexpr auto CR6      = Register(REG_FLAG_CONTROL | 0x06, 64, "cr6");
		constexpr auto CR7      = Register(REG_FLAG_CONTROL | 0x07, 64, "cr7");
		constexpr auto CR8      = Register(REG_FLAG_CONTROL | 0x08, 64, "cr8");
		constexpr auto CR9      = Register(REG_FLAG_CONTROL | 0x09, 64, "cr9");
		constexpr auto CR10     = Register(REG_FLAG_CONTROL | 0x0A, 64, "cr10");
		constexpr auto CR11     = Register(REG_FLAG_CONTROL | 0x0B, 64, "cr11");
		constexpr auto CR12     = Register(REG_FLAG_CONTROL | 0x0C, 64, "cr12");
		constexpr auto CR13     = Register(REG_FLAG_CONTROL | 0x0D, 64, "cr13");
		constexpr auto CR14     = Register(REG_FLAG_CONTROL | 0x0E, 64, "cr14");
		constexpr auto CR15     = Register(REG_FLAG_CONTROL | 0x0F, 64, "cr15");

		constexpr auto DR0      = Register(REG_FLAG_DEBUG | 0x00, 64, "dr0");
		constexpr auto DR1      = Register(REG_FLAG_DEBUG | 0x01, 64, "dr1");
		constexpr auto DR2      = Register(REG_FLAG_DEBUG | 0x02, 64, "dr2");
		constexpr auto DR3      = Register(REG_FLAG_DEBUG | 0x03, 64, "dr3");
		constexpr auto DR4      = Register(REG_FLAG_DEBUG | 0x04, 64, "dr4");
		constexpr auto DR5      = Register(REG_FLAG_DEBUG | 0x05, 64, "dr5");
		constexpr auto DR6      = Register(REG_FLAG_DEBUG | 0x06, 64, "dr6");
		constexpr auto DR7      = Register(REG_FLAG_DEBUG | 0x07, 64, "dr7");
		constexpr auto DR8      = Register(REG_FLAG_DEBUG | 0x08, 64, "dr8");
		constexpr auto DR9      = Register(REG_FLAG_DEBUG | 0x09, 64, "dr9");
		constexpr auto DR10     = Register(REG_FLAG_DEBUG | 0x0A, 64, "dr10");
		constexpr auto DR11     = Register(REG_FLAG_DEBUG | 0x0B, 64, "dr11");
		constexpr auto DR12     = Register(REG_FLAG_DEBUG | 0x0C, 64, "dr12");
		constexpr auto DR13     = Register(REG_FLAG_DEBUG | 0x0D, 64, "dr13");
		constexpr auto DR14     = Register(REG_FLAG_DEBUG | 0x0E, 64, "dr14");
		constexpr auto DR15     = Register(REG_FLAG_DEBUG | 0x0F, 64, "dr15");


		// none is none, perfectly valid
		constexpr auto NONE     = Register(-1, 0, "");

		// invalid is a poison value, not valid anywhere.
		constexpr auto INVALID  = Register(-99, 0, "");
	}
}
