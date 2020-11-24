// regs.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include <stddef.h>
#include <stdint.h>

namespace instrad::x86
{
	struct Register
	{
		constexpr Register(short idx, short width, const char* name, short id) : m_index(idx), m_width(width),
			m_name(name), m_unique_id(id) { }

		constexpr const char* name() const { return this->m_name; }
		constexpr short width() const { return this->m_width; }

		constexpr bool present() const { return this->m_index >= 0; }
		constexpr short index() const { return this->m_index; }

		constexpr bool operator== (const Register& other) const { return this->m_unique_id == other.m_unique_id; }
		constexpr bool operator!= (const Register& other) const { return !(*this == other); }

	private:
		short m_index;
		short m_width;
		const char* m_name;
		short m_unique_id;
	};

	namespace regs
	{
		// 0=ax, 1=cx, 2=dx, 3=bx, 4=sp, 5=bp, 6=si, 7=di, 8=r8, etc, the rest are sane.

		constexpr int REG_FLAG_IP       = 0x0080;
		constexpr int REG_FLAG_SEGMENT  = 0x0100;
		constexpr int REG_FLAG_CONTROL  = 0x0200;
		constexpr int REG_FLAG_DEBUG    = 0x0400;
		constexpr int REG_FLAG_X87      = 0x0800;
		constexpr int REG_FLAG_MMX      = 0x1000;
		constexpr int REG_FLAG_SSE      = 0x2000;
		constexpr int REG_FLAG_HI_BYTE  = 0x4000;

		constexpr auto  AL      = Register(0,  8,  "al", 0);
		constexpr auto  AX      = Register(0, 16,  "ax", 1);
		constexpr auto EAX      = Register(0, 32, "eax", 2);
		constexpr auto RAX      = Register(0, 64, "rax", 3);
		constexpr auto  AH      = Register(REG_FLAG_HI_BYTE | 0,  8,  "ah", 4);

		constexpr auto  CL      = Register(1,  8,  "cl", 5);
		constexpr auto  CX      = Register(1, 16,  "cx", 6);
		constexpr auto ECX      = Register(1, 32, "ecx", 7);
		constexpr auto RCX      = Register(1, 64, "rcx", 8);
		constexpr auto  CH      = Register(REG_FLAG_HI_BYTE | 1,  8,  "ch", 9);

		constexpr auto  DL      = Register(2,  8,  "dl", 10);
		constexpr auto  DX      = Register(2, 16,  "dx", 11);
		constexpr auto EDX      = Register(2, 32, "edx", 12);
		constexpr auto RDX      = Register(2, 64, "rdx", 13);
		constexpr auto  DH      = Register(REG_FLAG_HI_BYTE | 2,  8,  "dh", 14);

		constexpr auto  BL      = Register(3,  8,  "bl", 15);
		constexpr auto  BX      = Register(3, 16,  "bx", 16);
		constexpr auto EBX      = Register(3, 32, "ebx", 17);
		constexpr auto RBX      = Register(3, 64, "rbx", 18);
		constexpr auto  BH      = Register(REG_FLAG_HI_BYTE | 3,  8,  "bh", 19);

		constexpr auto  SPL     = Register(4,  8,  "spl", 20);
		constexpr auto  SP      = Register(4, 16,  "sp", 21);
		constexpr auto ESP      = Register(4, 32, "esp", 22);
		constexpr auto RSP      = Register(4, 64, "rsp", 23);

		constexpr auto  BPL     = Register(5,  8,  "bpl", 24);
		constexpr auto  BP      = Register(5, 16,  "bp", 25);
		constexpr auto EBP      = Register(5, 32, "ebp", 26);
		constexpr auto RBP      = Register(5, 64, "rbp", 27);

		constexpr auto  SIL     = Register(6,  8,  "sil", 28);
		constexpr auto  SI      = Register(6, 16,  "si", 29);
		constexpr auto ESI      = Register(6, 32, "esi", 30);
		constexpr auto RSI      = Register(6, 64, "rsi", 31);

		constexpr auto  DIL     = Register(7,  8,  "dil", 32);
		constexpr auto  DI      = Register(7, 16,  "di", 33);
		constexpr auto EDI      = Register(7, 32, "edi", 34);
		constexpr auto RDI      = Register(7, 64, "rdi", 35);

		constexpr auto R8B      = Register(8,  8, "r8b", 36);
		constexpr auto R8W      = Register(8, 16, "r8w", 37);
		constexpr auto R8D      = Register(8, 32, "r8d", 38);
		constexpr auto R8       = Register(8, 64, "r8", 39);

		constexpr auto R9B      = Register(9,  8, "r9b", 40);
		constexpr auto R9W      = Register(9, 16, "r9w", 41);
		constexpr auto R9D      = Register(9, 32, "r9d", 42);
		constexpr auto R9       = Register(9, 64, "r9", 43);

		constexpr auto R10B     = Register(10,  8, "r10b", 44);
		constexpr auto R10W     = Register(10, 16, "r10w", 45);
		constexpr auto R10D     = Register(10, 32, "r10d", 46);
		constexpr auto R10      = Register(10, 64, "r10", 47);

		constexpr auto R11B     = Register(11,  8, "r11b", 48);
		constexpr auto R11W     = Register(11, 16, "r11w", 49);
		constexpr auto R11D     = Register(11, 32, "r11d", 50);
		constexpr auto R11      = Register(11, 64, "r11", 51);

		constexpr auto R12B     = Register(12,  8, "r12b", 52);
		constexpr auto R12W     = Register(12, 16, "r12w", 53);
		constexpr auto R12D     = Register(12, 32, "r12d", 54);
		constexpr auto R12      = Register(12, 64, "r12", 55);

		constexpr auto R13B     = Register(13,  8, "r13b", 56);
		constexpr auto R13W     = Register(13, 16, "r13w", 57);
		constexpr auto R13D     = Register(13, 32, "r13d", 58);
		constexpr auto R13      = Register(13, 64, "r13", 59);

		constexpr auto R14B     = Register(14,  8, "r14b", 60);
		constexpr auto R14W     = Register(14, 16, "r14w", 61);
		constexpr auto R14D     = Register(14, 32, "r14d", 62);
		constexpr auto R14      = Register(14, 64, "r14", 63);

		constexpr auto R15B     = Register(15,  8, "r15b", 64);
		constexpr auto R15W     = Register(15, 16, "r15w", 65);
		constexpr auto R15D     = Register(15, 32, "r15d", 66);
		constexpr auto R15      = Register(15, 64, "r15", 67);

		// not a real register that you can use, but we need to be able to represent this.
		constexpr auto  IP      = Register(REG_FLAG_IP | 0, 16,  "ip", 68);
		constexpr auto EIP      = Register(REG_FLAG_IP | 0, 32, "eip", 69);
		constexpr auto RIP      = Register(REG_FLAG_IP | 0, 64, "rip", 70);

		constexpr auto CS       = Register(REG_FLAG_SEGMENT | 0, 16, "cs", 71);
		constexpr auto DS       = Register(REG_FLAG_SEGMENT | 1, 16, "ds", 72);
		constexpr auto ES       = Register(REG_FLAG_SEGMENT | 2, 16, "es", 73);
		constexpr auto FS       = Register(REG_FLAG_SEGMENT | 3, 16, "fs", 74);
		constexpr auto GS       = Register(REG_FLAG_SEGMENT | 4, 16, "gs", 75);
		constexpr auto SS       = Register(REG_FLAG_SEGMENT | 5, 16, "ss", 76);

		constexpr auto CR0      = Register(REG_FLAG_CONTROL |  0, 64, "cr0", 77);
		constexpr auto CR1      = Register(REG_FLAG_CONTROL |  1, 64, "cr1", 78);
		constexpr auto CR2      = Register(REG_FLAG_CONTROL |  2, 64, "cr2", 79);
		constexpr auto CR3      = Register(REG_FLAG_CONTROL |  3, 64, "cr3", 80);
		constexpr auto CR4      = Register(REG_FLAG_CONTROL |  4, 64, "cr4", 81);
		constexpr auto CR5      = Register(REG_FLAG_CONTROL |  5, 64, "cr5", 82);
		constexpr auto CR6      = Register(REG_FLAG_CONTROL |  6, 64, "cr6", 83);
		constexpr auto CR7      = Register(REG_FLAG_CONTROL |  7, 64, "cr7", 84);
		constexpr auto CR8      = Register(REG_FLAG_CONTROL |  8, 64, "cr8", 85);
		constexpr auto CR9      = Register(REG_FLAG_CONTROL |  9, 64, "cr9", 86);
		constexpr auto CR10     = Register(REG_FLAG_CONTROL | 10, 64, "cr10", 87);
		constexpr auto CR11     = Register(REG_FLAG_CONTROL | 11, 64, "cr11", 88);
		constexpr auto CR12     = Register(REG_FLAG_CONTROL | 12, 64, "cr12", 89);
		constexpr auto CR13     = Register(REG_FLAG_CONTROL | 13, 64, "cr13", 90);
		constexpr auto CR14     = Register(REG_FLAG_CONTROL | 14, 64, "cr14", 91);
		constexpr auto CR15     = Register(REG_FLAG_CONTROL | 15, 64, "cr15", 92);

		constexpr auto DR0      = Register(REG_FLAG_DEBUG |  0, 64, "dr0", 93);
		constexpr auto DR1      = Register(REG_FLAG_DEBUG |  1, 64, "dr1", 94);
		constexpr auto DR2      = Register(REG_FLAG_DEBUG |  2, 64, "dr2", 95);
		constexpr auto DR3      = Register(REG_FLAG_DEBUG |  3, 64, "dr3", 96);
		constexpr auto DR4      = Register(REG_FLAG_DEBUG |  4, 64, "dr4", 97);
		constexpr auto DR5      = Register(REG_FLAG_DEBUG |  5, 64, "dr5", 98);
		constexpr auto DR6      = Register(REG_FLAG_DEBUG |  6, 64, "dr6", 99);
		constexpr auto DR7      = Register(REG_FLAG_DEBUG |  7, 64, "dr7", 100);
		constexpr auto DR8      = Register(REG_FLAG_DEBUG |  8, 64, "dr8", 101);
		constexpr auto DR9      = Register(REG_FLAG_DEBUG |  9, 64, "dr9", 102);
		constexpr auto DR10     = Register(REG_FLAG_DEBUG | 10, 64, "dr10", 103);
		constexpr auto DR11     = Register(REG_FLAG_DEBUG | 11, 64, "dr11", 104);
		constexpr auto DR12     = Register(REG_FLAG_DEBUG | 12, 64, "dr12", 105);
		constexpr auto DR13     = Register(REG_FLAG_DEBUG | 13, 64, "dr13", 106);
		constexpr auto DR14     = Register(REG_FLAG_DEBUG | 14, 64, "dr14", 107);
		constexpr auto DR15     = Register(REG_FLAG_DEBUG | 15, 64, "dr15", 108);

		constexpr auto ST0      = Register(REG_FLAG_X87 | 0, 80, "st0", 109);
		constexpr auto ST1      = Register(REG_FLAG_X87 | 1, 80, "st1", 110);
		constexpr auto ST2      = Register(REG_FLAG_X87 | 2, 80, "st2", 111);
		constexpr auto ST3      = Register(REG_FLAG_X87 | 3, 80, "st3", 112);
		constexpr auto ST4      = Register(REG_FLAG_X87 | 4, 80, "st4", 113);
		constexpr auto ST5      = Register(REG_FLAG_X87 | 5, 80, "st5", 114);
		constexpr auto ST6      = Register(REG_FLAG_X87 | 6, 80, "st6", 115);
		constexpr auto ST7      = Register(REG_FLAG_X87 | 7, 80, "st7", 116);

		constexpr auto MMX0     = Register(REG_FLAG_MMX | 0, 64, "mmx0", 117);
		constexpr auto MMX1     = Register(REG_FLAG_MMX | 1, 64, "mmx1", 118);
		constexpr auto MMX2     = Register(REG_FLAG_MMX | 2, 64, "mmx2", 119);
		constexpr auto MMX3     = Register(REG_FLAG_MMX | 3, 64, "mmx3", 120);
		constexpr auto MMX4     = Register(REG_FLAG_MMX | 4, 64, "mmx4", 121);
		constexpr auto MMX5     = Register(REG_FLAG_MMX | 5, 64, "mmx5", 122);
		constexpr auto MMX6     = Register(REG_FLAG_MMX | 6, 64, "mmx6", 123);
		constexpr auto MMX7     = Register(REG_FLAG_MMX | 7, 64, "mmx7", 124);

		constexpr auto XMM0     = Register(REG_FLAG_SSE |  0, 128, "xmm0", 125);
		constexpr auto XMM1     = Register(REG_FLAG_SSE |  1, 128, "xmm1", 126);
		constexpr auto XMM2     = Register(REG_FLAG_SSE |  2, 128, "xmm2", 127);
		constexpr auto XMM3     = Register(REG_FLAG_SSE |  3, 128, "xmm3", 128);
		constexpr auto XMM4     = Register(REG_FLAG_SSE |  4, 128, "xmm4", 129);
		constexpr auto XMM5     = Register(REG_FLAG_SSE |  5, 128, "xmm5", 130);
		constexpr auto XMM6     = Register(REG_FLAG_SSE |  6, 128, "xmm6", 131);
		constexpr auto XMM7     = Register(REG_FLAG_SSE |  7, 128, "xmm7", 132);
		constexpr auto XMM8     = Register(REG_FLAG_SSE |  8, 128, "xmm8", 133);
		constexpr auto XMM9     = Register(REG_FLAG_SSE |  9, 128, "xmm9", 134);
		constexpr auto XMM10    = Register(REG_FLAG_SSE | 10, 128, "xmm10", 135);
		constexpr auto XMM11    = Register(REG_FLAG_SSE | 11, 128, "xmm11", 136);
		constexpr auto XMM12    = Register(REG_FLAG_SSE | 12, 128, "xmm12", 137);
		constexpr auto XMM13    = Register(REG_FLAG_SSE | 13, 128, "xmm13", 138);
		constexpr auto XMM14    = Register(REG_FLAG_SSE | 14, 128, "xmm14", 139);
		constexpr auto XMM15    = Register(REG_FLAG_SSE | 15, 128, "xmm15", 140);

		constexpr auto YMM0     = Register(REG_FLAG_SSE |  0, 256, "ymm0", 141);
		constexpr auto YMM1     = Register(REG_FLAG_SSE |  1, 256, "ymm1", 142);
		constexpr auto YMM2     = Register(REG_FLAG_SSE |  2, 256, "ymm2", 143);
		constexpr auto YMM3     = Register(REG_FLAG_SSE |  3, 256, "ymm3", 144);
		constexpr auto YMM4     = Register(REG_FLAG_SSE |  4, 256, "ymm4", 145);
		constexpr auto YMM5     = Register(REG_FLAG_SSE |  5, 256, "ymm5", 146);
		constexpr auto YMM6     = Register(REG_FLAG_SSE |  6, 256, "ymm6", 147);
		constexpr auto YMM7     = Register(REG_FLAG_SSE |  7, 256, "ymm7", 148);
		constexpr auto YMM8     = Register(REG_FLAG_SSE |  8, 256, "ymm8", 149);
		constexpr auto YMM9     = Register(REG_FLAG_SSE |  9, 256, "ymm9", 150);
		constexpr auto YMM10    = Register(REG_FLAG_SSE | 10, 256, "ymm10", 151);
		constexpr auto YMM11    = Register(REG_FLAG_SSE | 11, 256, "ymm11", 152);
		constexpr auto YMM12    = Register(REG_FLAG_SSE | 12, 256, "ymm12", 153);
		constexpr auto YMM13    = Register(REG_FLAG_SSE | 13, 256, "ymm13", 154);
		constexpr auto YMM14    = Register(REG_FLAG_SSE | 14, 256, "ymm14", 155);
		constexpr auto YMM15    = Register(REG_FLAG_SSE | 15, 256, "ymm15", 156);





		// none is none, perfectly valid
		constexpr auto NONE     = Register(-1, 0, "", -1);

		// invalid is a poison value, not valid anywhere.
		constexpr auto INVALID  = Register(-99, 0, "", -2);



		constexpr Register RegisterTable_Legacy_8[] = {
			regs::AL, regs::CL, regs::DL, regs::BL, regs::AH, regs::CH, regs::DH, regs::BH
		};

		constexpr Register RegisterTable_8[] = {
			regs::AL, regs::CL, regs::DL, regs::BL, regs::SPL, regs::BPL, regs::SIL, regs::DIL,
			regs::R8B, regs::R9B, regs::R10B, regs::R11B, regs::R12B, regs::R13B, regs::R14B, regs::R15B
		};

		constexpr Register RegisterTable_16[] = {
			regs::AX, regs::CX, regs::DX, regs::BX, regs::SP, regs::BP, regs::SI, regs::DI,
			regs::R8W, regs::R9W, regs::R10W, regs::R11W, regs::R12W, regs::R13W, regs::R14W, regs::R15W
		};

		constexpr Register RegisterTable_32[] = {
			regs::EAX, regs::ECX, regs::EDX,  regs::EBX,  regs::ESP,  regs::EBP,  regs::ESI,  regs::EDI,
			regs::R8D, regs::R9D, regs::R10D, regs::R11D, regs::R12D, regs::R13D, regs::R14D, regs::R15D
		};

		constexpr Register RegisterTable_64[] = {
			regs::RAX, regs::RCX, regs::RDX, regs::RBX, regs::RSP, regs::RBP, regs::RSI, regs::RDI,
			regs::R8,  regs::R9,  regs::R10, regs::R11, regs::R12, regs::R13, regs::R14, regs::R15
		};

		constexpr Register RegisterTable_Segment[] = {
			regs::ES, regs::CS, regs::SS, regs::DS, regs::FS, regs::GS
		};

		constexpr Register RegisterTable_Control[] = {
			regs::CR0, regs::CR1, regs::CR2,  regs::CR3,  regs::CR4,  regs::CR5,  regs::CR6,  regs::CR7,
			regs::CR8, regs::CR9, regs::CR10, regs::CR11, regs::CR12, regs::CR13, regs::CR14, regs::CR15
		};

		constexpr Register RegisterTable_Debug[] = {
			regs::DR0, regs::DR1, regs::DR2,  regs::DR3,  regs::DR4,  regs::DR5,  regs::DR6,  regs::DR7,
			regs::DR8, regs::DR9, regs::DR10, regs::DR11, regs::DR12, regs::DR13, regs::DR14, regs::DR15
		};

		constexpr Register RegisterTable_x87[] = {
			regs::ST0, regs::ST1, regs::ST2, regs::ST3, regs::ST4, regs::ST5, regs::ST6, regs::ST7
		};

		constexpr Register RegisterTable_MMX[] = {
			regs::MMX0, regs::MMX1, regs::MMX2, regs::MMX3, regs::MMX4, regs::MMX5, regs::MMX6, regs::MMX7
		};

		constexpr Register RegisterTable_XMM[] = {
			regs::XMM0, regs::XMM1, regs::XMM2,  regs::XMM3,  regs::XMM4,  regs::XMM5,  regs::XMM6,  regs::XMM7,
			regs::XMM8, regs::XMM9, regs::XMM10, regs::XMM11, regs::XMM12, regs::XMM13, regs::XMM14, regs::XMM15
		};

		constexpr Register RegisterTable_YMM[] = {
			regs::YMM0, regs::YMM1, regs::YMM2,  regs::YMM3,  regs::YMM4,  regs::YMM5,  regs::YMM6,  regs::YMM7,
			regs::YMM8, regs::YMM9, regs::YMM10, regs::YMM11, regs::YMM12, regs::YMM13, regs::YMM14, regs::YMM15
		};





		template <typename T, size_t N>
		constexpr size_t ArrayLength(const T (&)[N]) { return N; }

		static_assert(ArrayLength(RegisterTable_Legacy_8) == 8, "invalid register table");
		static_assert(ArrayLength(RegisterTable_8)  == 16, "invalid register table");
		static_assert(ArrayLength(RegisterTable_16) == 16, "invalid register table");
		static_assert(ArrayLength(RegisterTable_32) == 16, "invalid register table");
		static_assert(ArrayLength(RegisterTable_64) == 16, "invalid register table");
		static_assert(ArrayLength(RegisterTable_Segment) == 6, "invalid register table");
		static_assert(ArrayLength(RegisterTable_Control) == 16, "invalid register table");
		static_assert(ArrayLength(RegisterTable_Debug) == 16, "invalid register table");
		static_assert(ArrayLength(RegisterTable_x87) == 8, "invalid register table");
		static_assert(ArrayLength(RegisterTable_MMX) == 8, "invalid register table");
		static_assert(ArrayLength(RegisterTable_XMM) == 16, "invalid register table");
		static_assert(ArrayLength(RegisterTable_YMM) == 16, "invalid register table");

		template <size_t N>
		constexpr Register __getOrInvalid(const Register (&arr)[N], size_t idx)
		{
			if(idx > N) return INVALID;
			return arr[idx];
		}

		constexpr Register get8BitLegacy(size_t idx)    { return __getOrInvalid(RegisterTable_Legacy_8, idx); }
		constexpr Register get8Bit(size_t idx)          { return __getOrInvalid(RegisterTable_8, idx); }
		constexpr Register get16Bit(size_t idx)         { return __getOrInvalid(RegisterTable_16, idx); }
		constexpr Register get32Bit(size_t idx)         { return __getOrInvalid(RegisterTable_32, idx); }
		constexpr Register get64Bit(size_t idx)         { return __getOrInvalid(RegisterTable_64, idx); }
		constexpr Register getSegment(size_t idx)       { return __getOrInvalid(RegisterTable_Segment, idx); }
		constexpr Register getControl(size_t idx)       { return __getOrInvalid(RegisterTable_Control, idx); }
		constexpr Register getDebug(size_t idx)         { return __getOrInvalid(RegisterTable_Debug, idx); }
		constexpr Register getX87(size_t idx)           { return __getOrInvalid(RegisterTable_x87, idx); }
		constexpr Register getMMX(size_t idx)           { return __getOrInvalid(RegisterTable_MMX, idx); }
		constexpr Register getXMM(size_t idx)           { return __getOrInvalid(RegisterTable_XMM, idx); }
		constexpr Register getYMM(size_t idx)           { return __getOrInvalid(RegisterTable_YMM, idx); }
	}
}
