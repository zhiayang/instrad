// primary.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "entry.h"
#include "x87.h"

namespace instrad::x64::tables
{
	// the extension tables are indexed by the ModRM.REG value.
	constexpr TableEntry ModRMExt_80[] = {
		[0] = entry_2(0x80, ops::ADD, OpKind::RegMem8, OpKind::Imm8),
		[1] = entry_2(0x80, ops::OR,  OpKind::RegMem8, OpKind::Imm8),
		[2] = entry_2(0x80, ops::ADC, OpKind::RegMem8, OpKind::Imm8),
		[3] = entry_2(0x80, ops::SBB, OpKind::RegMem8, OpKind::Imm8),
		[4] = entry_2(0x80, ops::AND, OpKind::RegMem8, OpKind::Imm8),
		[5] = entry_2(0x80, ops::SUB, OpKind::RegMem8, OpKind::Imm8),
		[6] = entry_2(0x80, ops::XOR, OpKind::RegMem8, OpKind::Imm8),
		[7] = entry_2(0x80, ops::CMP, OpKind::RegMem8, OpKind::Imm8),
	};

	constexpr TableEntry ModRMExt_81[] = {
		[0] = entry_2(0x81, ops::ADD, OpKind::RegMem32, OpKind::Imm32),
		[1] = entry_2(0x81, ops::OR,  OpKind::RegMem32, OpKind::Imm32),
		[2] = entry_2(0x81, ops::ADC, OpKind::RegMem32, OpKind::Imm32),
		[3] = entry_2(0x81, ops::SBB, OpKind::RegMem32, OpKind::Imm32),
		[4] = entry_2(0x81, ops::AND, OpKind::RegMem32, OpKind::Imm32),
		[5] = entry_2(0x81, ops::SUB, OpKind::RegMem32, OpKind::Imm32),
		[6] = entry_2(0x81, ops::XOR, OpKind::RegMem32, OpKind::Imm32),
		[7] = entry_2(0x81, ops::CMP, OpKind::RegMem32, OpKind::Imm32),
	};

	// these are apparently invalid in 64-bit mode.
	constexpr TableEntry ModRMExt_82[] = {
		[0] = entry_2(0x82, ops::ADD, OpKind::RegMem8, OpKind::Imm8),
		[1] = entry_2(0x82, ops::OR,  OpKind::RegMem8, OpKind::Imm8),
		[2] = entry_2(0x82, ops::ADC, OpKind::RegMem8, OpKind::Imm8),
		[3] = entry_2(0x82, ops::SBB, OpKind::RegMem8, OpKind::Imm8),
		[4] = entry_2(0x82, ops::AND, OpKind::RegMem8, OpKind::Imm8),
		[5] = entry_2(0x82, ops::SUB, OpKind::RegMem8, OpKind::Imm8),
		[6] = entry_2(0x82, ops::XOR, OpKind::RegMem8, OpKind::Imm8),
		[7] = entry_2(0x82, ops::CMP, OpKind::RegMem8, OpKind::Imm8),
	};

	constexpr TableEntry ModRMExt_83[] = {
		[0] = entry_2(0x83, ops::ADD, OpKind::RegMem32, OpKind::SignExtImm8),
		[1] = entry_2(0x83, ops::OR,  OpKind::RegMem32, OpKind::SignExtImm8),
		[2] = entry_2(0x83, ops::ADC, OpKind::RegMem32, OpKind::SignExtImm8),
		[3] = entry_2(0x83, ops::SBB, OpKind::RegMem32, OpKind::SignExtImm8),
		[4] = entry_2(0x83, ops::AND, OpKind::RegMem32, OpKind::SignExtImm8),
		[5] = entry_2(0x83, ops::SUB, OpKind::RegMem32, OpKind::SignExtImm8),
		[6] = entry_2(0x83, ops::XOR, OpKind::RegMem32, OpKind::SignExtImm8),
		[7] = entry_2(0x83, ops::CMP, OpKind::RegMem32, OpKind::SignExtImm8),
	};

	constexpr TableEntry ModRMExt_8F[] = {
		[0] = entry_1(0x8F, ops::POP, OpKind::RegMem64),

		[7] = entry_blank
	};

	constexpr TableEntry ModRMExt_C0[] = {
		[0] = entry_2(0xC0, ops::ROL, OpKind::RegMem8, OpKind::Imm8),
		[1] = entry_2(0xC0, ops::ROR, OpKind::RegMem8, OpKind::Imm8),
		[2] = entry_2(0xC0, ops::RCL, OpKind::RegMem8, OpKind::Imm8),
		[3] = entry_2(0xC0, ops::RCR, OpKind::RegMem8, OpKind::Imm8),
		[4] = entry_2(0xC0, ops::SHL, OpKind::RegMem8, OpKind::Imm8),
		[5] = entry_2(0xC0, ops::SHR, OpKind::RegMem8, OpKind::Imm8),
		[6] = entry_2(0xC0, ops::SAL, OpKind::RegMem8, OpKind::Imm8),
		[7] = entry_2(0xC0, ops::SAR, OpKind::RegMem8, OpKind::Imm8),
	};

	constexpr TableEntry ModRMExt_C1[] = {
		[0] = entry_2(0xC1, ops::ROL, OpKind::RegMem32, OpKind::Imm8),
		[1] = entry_2(0xC1, ops::ROR, OpKind::RegMem32, OpKind::Imm8),
		[2] = entry_2(0xC1, ops::RCL, OpKind::RegMem32, OpKind::Imm8),
		[3] = entry_2(0xC1, ops::RCR, OpKind::RegMem32, OpKind::Imm8),
		[4] = entry_2(0xC1, ops::SHL, OpKind::RegMem32, OpKind::Imm8),
		[5] = entry_2(0xC1, ops::SHR, OpKind::RegMem32, OpKind::Imm8),
		[6] = entry_2(0xC1, ops::SAL, OpKind::RegMem32, OpKind::Imm8),
		[7] = entry_2(0xC1, ops::SAR, OpKind::RegMem32, OpKind::Imm8),
	};

	constexpr TableEntry ModRMExt_C6[] = {
		[0] = entry_2(0xC6, ops::MOV, OpKind::RegMem8, OpKind::Imm8),

		[7] = entry_blank,
	};

	constexpr TableEntry ModRMExt_C7[] = {
		[0] = entry_2(0xC7, ops::MOV, OpKind::RegMem32, OpKind::Imm32),

		[7] = entry_blank,
	};

	constexpr TableEntry ModRMExt_D0[] = {
		[0] = entry_1(0xD0, ops::ROL, OpKind::RegMem8),
		[1] = entry_1(0xD0, ops::ROR, OpKind::RegMem8),
		[2] = entry_1(0xD0, ops::RCL, OpKind::RegMem8),
		[3] = entry_1(0xD0, ops::RCR, OpKind::RegMem8),
		[4] = entry_1(0xD0, ops::SHL, OpKind::RegMem8),
		[5] = entry_1(0xD0, ops::SHR, OpKind::RegMem8),
		[6] = entry_1(0xD0, ops::SAL, OpKind::RegMem8),
		[7] = entry_1(0xD0, ops::SAR, OpKind::RegMem8),
	};

	constexpr TableEntry ModRMExt_D1[] = {
		[0] = entry_1(0xD1, ops::ROL, OpKind::RegMem32),
		[1] = entry_1(0xD1, ops::ROR, OpKind::RegMem32),
		[2] = entry_1(0xD1, ops::RCL, OpKind::RegMem32),
		[3] = entry_1(0xD1, ops::RCR, OpKind::RegMem32),
		[4] = entry_1(0xD1, ops::SHL, OpKind::RegMem32),
		[5] = entry_1(0xD1, ops::SHR, OpKind::RegMem32),
		[6] = entry_1(0xD1, ops::SAL, OpKind::RegMem32),
		[7] = entry_1(0xD1, ops::SAR, OpKind::RegMem32),
	};

	constexpr TableEntry ModRMExt_D2[] = {
		[0] = entry_2(0xD2, ops::ROL, OpKind::RegMem8, OpKind::ImplicitCL),
		[1] = entry_2(0xD2, ops::ROR, OpKind::RegMem8, OpKind::ImplicitCL),
		[2] = entry_2(0xD2, ops::RCL, OpKind::RegMem8, OpKind::ImplicitCL),
		[3] = entry_2(0xD2, ops::RCR, OpKind::RegMem8, OpKind::ImplicitCL),
		[4] = entry_2(0xD2, ops::SHL, OpKind::RegMem8, OpKind::ImplicitCL),
		[5] = entry_2(0xD2, ops::SHR, OpKind::RegMem8, OpKind::ImplicitCL),
		[6] = entry_2(0xD2, ops::SAL, OpKind::RegMem8, OpKind::ImplicitCL),
		[7] = entry_2(0xD2, ops::SAR, OpKind::RegMem8, OpKind::ImplicitCL),
	};

	constexpr TableEntry ModRMExt_D3[] = {
		[0] = entry_2(0xD3, ops::ROL, OpKind::RegMem32, OpKind::ImplicitCL),
		[1] = entry_2(0xD3, ops::ROR, OpKind::RegMem32, OpKind::ImplicitCL),
		[2] = entry_2(0xD3, ops::RCL, OpKind::RegMem32, OpKind::ImplicitCL),
		[3] = entry_2(0xD3, ops::RCR, OpKind::RegMem32, OpKind::ImplicitCL),
		[4] = entry_2(0xD3, ops::SHL, OpKind::RegMem32, OpKind::ImplicitCL),
		[5] = entry_2(0xD3, ops::SHR, OpKind::RegMem32, OpKind::ImplicitCL),
		[6] = entry_2(0xD3, ops::SAL, OpKind::RegMem32, OpKind::ImplicitCL),
		[7] = entry_2(0xD3, ops::SAR, OpKind::RegMem32, OpKind::ImplicitCL),
	};


	constexpr TableEntry ModRMExt_F6[] = {
		[0] = entry_2(0xF6, ops::TEST, OpKind::RegMem8, OpKind::Imm8),
		[1] = entry_2(0xF6, ops::TEST, OpKind::RegMem8, OpKind::Imm8),
		[2] = entry_1(0xF6, ops::NOT,  OpKind::RegMem8),
		[3] = entry_1(0xF6, ops::NEG,  OpKind::RegMem8),
		[4] = entry_1(0xF6, ops::MUL,  OpKind::RegMem8),
		[5] = entry_1(0xF6, ops::IMUL, OpKind::RegMem8),
		[6] = entry_1(0xF6, ops::DIV,  OpKind::RegMem8),
		[7] = entry_1(0xF6, ops::IDIV, OpKind::RegMem8),
	};

	constexpr TableEntry ModRMExt_F7[] = {
		[0] = entry_2(0xF7, ops::TEST, OpKind::RegMem32, OpKind::Imm32),
		[1] = entry_2(0xF7, ops::TEST, OpKind::RegMem32, OpKind::Imm32),
		[2] = entry_1(0xF7, ops::NOT,  OpKind::RegMem32),
		[3] = entry_1(0xF7, ops::NEG,  OpKind::RegMem32),
		[4] = entry_1(0xF7, ops::MUL,  OpKind::RegMem32),
		[5] = entry_1(0xF7, ops::IMUL, OpKind::RegMem32),
		[6] = entry_1(0xF7, ops::DIV,  OpKind::RegMem32),
		[7] = entry_1(0xF7, ops::IDIV, OpKind::RegMem32),
	};

	constexpr TableEntry ModRMExt_FE[] = {
		[0] = entry_1(0xFE, ops::INC, OpKind::RegMem8),
		[1] = entry_1(0xFE, ops::DEC, OpKind::RegMem8),

		[7] = entry_blank,
	};

	constexpr TableEntry ModRMExt_FF[] = {
		[0] = entry_1(0xFF, ops::INC,  OpKind::RegMem32),
		[1] = entry_1(0xFF, ops::DEC,  OpKind::RegMem32),
		[2] = entry_1(0xFF, ops::CALL, OpKind::RegMem64),
		[3] = entry_1(0xFF, ops::CALL, OpKind::Ptr16_32),
		[4] = entry_1(0xFF, ops::JMP,  OpKind::RegMem64),
		[5] = entry_1(0xFF, ops::JMP,  OpKind::Ptr16_32),
		[6] = entry_1(0xFF, ops::PUSH, OpKind::RegMem64),

		[7] = entry_blank,
	};



	constexpr TableEntry PrimaryOpcodeMap[] = {
		[0x00] = entry_2(0x00, ops::ADD,  OpKind::RegMem8, OpKind::Reg8),
		[0x01] = entry_2(0x01, ops::ADD,  OpKind::RegMem32, OpKind::Reg32),
		[0x02] = entry_2(0x02, ops::ADD,  OpKind::Reg8, OpKind::RegMem8),
		[0x03] = entry_2(0x03, ops::ADD,  OpKind::Reg32, OpKind::RegMem32),
		[0x04] = entry_2_no_modrm(0x04, ops::ADD,  OpKind::ImplicitAL, OpKind::Imm8),
		[0x05] = entry_2_no_modrm(0x05, ops::ADD,  OpKind::ImplicitEAX, OpKind::Imm32),
		[0x06] = entry_1_no_modrm(0x06, ops::PUSH, OpKind::ImplicitES),     // push es; invalid in 64-bit
		[0x07] = entry_1_no_modrm(0x07, ops::POP,  OpKind::ImplicitES),     // pop es; invalid in 64-bit

		[0x08] = entry_2(0x08, ops::OR, OpKind::RegMem8, OpKind::Reg8),
		[0x09] = entry_2(0x09, ops::OR, OpKind::RegMem32, OpKind::Reg32),
		[0x0A] = entry_2(0x0A, ops::OR, OpKind::Reg8, OpKind::RegMem8),
		[0x0B] = entry_2(0x0B, ops::OR, OpKind::Reg32, OpKind::RegMem32),
		[0x0C] = entry_2_no_modrm(0x0C, ops::OR,   OpKind::ImplicitAL, OpKind::Imm8),
		[0x0D] = entry_2_no_modrm(0x0D, ops::OR,   OpKind::ImplicitEAX, OpKind::Imm32),
		[0x0E] = entry_1_no_modrm(0x0E, ops::PUSH, OpKind::ImplicitCS),     // push cs; invalid in 64-bit

		[0x10] = entry_2(0x10, ops::ADC, OpKind::RegMem8, OpKind::Reg8),
		[0x11] = entry_2(0x11, ops::ADC, OpKind::RegMem32, OpKind::Reg32),
		[0x12] = entry_2(0x12, ops::ADC, OpKind::Reg8, OpKind::RegMem8),
		[0x13] = entry_2(0x13, ops::ADC, OpKind::Reg32, OpKind::RegMem32),
		[0x14] = entry_2_no_modrm(0x14, ops::ADC,  OpKind::ImplicitAL, OpKind::Imm8),
		[0x15] = entry_2_no_modrm(0x15, ops::ADC,  OpKind::ImplicitEAX, OpKind::Imm32),
		[0x16] = entry_1_no_modrm(0x16, ops::PUSH, OpKind::ImplicitSS),     // push ss; invalid in 64-bit
		[0x17] = entry_1_no_modrm(0x17, ops::POP,  OpKind::ImplicitSS),     // pop ss; invalid in 64-bit

		[0x18] = entry_2(0x18, ops::SBB, OpKind::RegMem8, OpKind::Reg8),
		[0x19] = entry_2(0x19, ops::SBB, OpKind::RegMem32, OpKind::Reg32),
		[0x1A] = entry_2(0x1A, ops::SBB, OpKind::Reg8, OpKind::RegMem8),
		[0x1B] = entry_2(0x1B, ops::SBB, OpKind::Reg32, OpKind::RegMem32),
		[0x1C] = entry_2_no_modrm(0x1C, ops::SBB,  OpKind::ImplicitAL, OpKind::Imm8),
		[0x1D] = entry_2_no_modrm(0x1D, ops::SBB,  OpKind::ImplicitEAX, OpKind::Imm32),
		[0x1E] = entry_1_no_modrm(0x1E, ops::PUSH, OpKind::ImplicitDS),     // push ds; invalid in 64-bit
		[0x1F] = entry_1_no_modrm(0x1F, ops::POP,  OpKind::ImplicitDS),     // pop ds; invalid in 64-bit

		[0x20] = entry_2(0x20, ops::AND, OpKind::RegMem8, OpKind::Reg8),
		[0x21] = entry_2(0x21, ops::AND, OpKind::RegMem32, OpKind::Reg32),
		[0x22] = entry_2(0x22, ops::AND, OpKind::Reg8, OpKind::RegMem8),
		[0x23] = entry_2(0x23, ops::AND, OpKind::Reg32, OpKind::RegMem32),
		[0x24] = entry_2_no_modrm(0x24, ops::AND, OpKind::ImplicitAL, OpKind::Imm8),
		[0x25] = entry_2_no_modrm(0x25, ops::AND, OpKind::ImplicitEAX, OpKind::Imm32),
		[0x26] = entry_blank,                                               // ES segment override
		[0x27] = entry_0(0x27, ops::DAA),                                   // invalid in 64-bit mode

		[0x28] = entry_2(0x28, ops::SUB, OpKind::RegMem8, OpKind::Reg8),
		[0x29] = entry_2(0x29, ops::SUB, OpKind::RegMem32, OpKind::Reg32),
		[0x2A] = entry_2(0x2A, ops::SUB, OpKind::Reg8, OpKind::RegMem8),
		[0x2B] = entry_2(0x2B, ops::SUB, OpKind::Reg32, OpKind::RegMem32),
		[0x2C] = entry_2_no_modrm(0x2C, ops::SUB, OpKind::ImplicitAL, OpKind::Imm8),
		[0x2D] = entry_2_no_modrm(0x2D, ops::SUB, OpKind::ImplicitEAX, OpKind::Imm32),
		[0x2E] = entry_blank,                                               // CS segment override
		[0x2F] = entry_0(0x2F, ops::DAS),                                   // invalid in 64-bit mode

		[0x30] = entry_2(0x30, ops::XOR, OpKind::RegMem8, OpKind::Reg8),
		[0x31] = entry_2(0x31, ops::XOR, OpKind::RegMem32, OpKind::Reg32),
		[0x32] = entry_2(0x32, ops::XOR, OpKind::Reg8, OpKind::RegMem8),
		[0x33] = entry_2(0x33, ops::XOR, OpKind::Reg32, OpKind::RegMem32),
		[0x34] = entry_2_no_modrm(0x34, ops::XOR, OpKind::ImplicitAL, OpKind::Imm8),
		[0x35] = entry_2_no_modrm(0x35, ops::XOR, OpKind::ImplicitEAX, OpKind::Imm32),
		[0x36] = entry_blank,                                               // SS segment override
		[0x37] = entry_0(0x37, ops::AAA),                                   // invalid in 64-bit mode

		[0x38] = entry_2(0x38, ops::CMP, OpKind::RegMem8, OpKind::Reg8),
		[0x39] = entry_2(0x39, ops::CMP, OpKind::RegMem32, OpKind::Reg32),
		[0x3A] = entry_2(0x3A, ops::CMP, OpKind::Reg8, OpKind::RegMem8),
		[0x3B] = entry_2(0x3B, ops::CMP, OpKind::Reg32, OpKind::RegMem32),
		[0x3C] = entry_2_no_modrm(0x3C, ops::CMP, OpKind::ImplicitAL, OpKind::Imm8),
		[0x3D] = entry_2_no_modrm(0x3D, ops::CMP, OpKind::ImplicitEAX, OpKind::Imm32),
		[0x3E] = entry_blank,                                               // DS segment override
		[0x3F] = entry_0(0x3F, ops::AAS),                                   // invalid in 64-bit mode

		// in 64-bit mode, this is the REX prefix. but we want to support x86 also, so these are the inc/dec opcodes
		[0x40] = entry_lnri_1(0x40, ops::INC, OpKind::Reg32),
		[0x41] = entry_lnri_1(0x41, ops::INC, OpKind::Reg32),
		[0x42] = entry_lnri_1(0x42, ops::INC, OpKind::Reg32),
		[0x43] = entry_lnri_1(0x43, ops::INC, OpKind::Reg32),
		[0x44] = entry_lnri_1(0x44, ops::INC, OpKind::Reg32),
		[0x45] = entry_lnri_1(0x45, ops::INC, OpKind::Reg32),
		[0x46] = entry_lnri_1(0x46, ops::INC, OpKind::Reg32),
		[0x47] = entry_lnri_1(0x47, ops::INC, OpKind::Reg32),

		[0x48] = entry_lnri_1(0x48, ops::DEC, OpKind::Reg32),
		[0x49] = entry_lnri_1(0x49, ops::DEC, OpKind::Reg32),
		[0x4A] = entry_lnri_1(0x4A, ops::DEC, OpKind::Reg32),
		[0x4B] = entry_lnri_1(0x4B, ops::DEC, OpKind::Reg32),
		[0x4C] = entry_lnri_1(0x4C, ops::DEC, OpKind::Reg32),
		[0x4D] = entry_lnri_1(0x4D, ops::DEC, OpKind::Reg32),
		[0x4E] = entry_lnri_1(0x4E, ops::DEC, OpKind::Reg32),
		[0x4F] = entry_lnri_1(0x4F, ops::DEC, OpKind::Reg32),

		[0x50] = entry_lnri_1(0x50, ops::PUSH, OpKind::Reg64),
		[0x51] = entry_lnri_1(0x51, ops::PUSH, OpKind::Reg64),
		[0x52] = entry_lnri_1(0x52, ops::PUSH, OpKind::Reg64),
		[0x53] = entry_lnri_1(0x53, ops::PUSH, OpKind::Reg64),
		[0x54] = entry_lnri_1(0x54, ops::PUSH, OpKind::Reg64),
		[0x55] = entry_lnri_1(0x55, ops::PUSH, OpKind::Reg64),
		[0x56] = entry_lnri_1(0x56, ops::PUSH, OpKind::Reg64),
		[0x57] = entry_lnri_1(0x57, ops::PUSH, OpKind::Reg64),

		[0x58] = entry_lnri_1(0x58, ops::POP, OpKind::Reg64),
		[0x59] = entry_lnri_1(0x59, ops::POP, OpKind::Reg64),
		[0x5A] = entry_lnri_1(0x5A, ops::POP, OpKind::Reg64),
		[0x5B] = entry_lnri_1(0x5B, ops::POP, OpKind::Reg64),
		[0x5C] = entry_lnri_1(0x5C, ops::POP, OpKind::Reg64),
		[0x5D] = entry_lnri_1(0x5D, ops::POP, OpKind::Reg64),
		[0x5E] = entry_lnri_1(0x5E, ops::POP, OpKind::Reg64),
		[0x5F] = entry_lnri_1(0x5F, ops::POP, OpKind::Reg64),

		[0x60] = entry_0(0x60, ops::PUSHAD),
		[0x61] = entry_0(0x61, ops::POPAD),
		[0x62] = entry_2(0x62, ops::BOUND, OpKind::Reg32, OpKind::RegMem32),
		[0x63] = entry_2(0x63, ops::MOVSXD, OpKind::Reg64, OpKind::RegMem64),
		[0x64] = entry_blank,                                               // FS segment override
		[0x65] = entry_blank,                                               // GS segment override
		[0x66] = entry_blank,                                               // operand size override
		[0x67] = entry_blank,                                               // address size override

		[0x68] = entry_1_no_modrm(0x68, ops::PUSH, OpKind::SignExtImm32),
		[0x69] = entry_3(0x69, ops::IMUL, OpKind::Reg32, OpKind::RegMem32, OpKind::Imm32),
		[0x6A] = entry_1_no_modrm(0x6A, ops::PUSH, OpKind::Imm8),
		[0x6B] = entry_3(0x6B, ops::IMUL, OpKind::Reg32, OpKind::RegMem32, OpKind::Imm8),
		[0x6C] = entry_2(0x6C, ops::INSB, OpKind::ImplicitMem8_ES_EDI, OpKind::ImplicitDX),
		[0x6D] = entry_2(0x6D, ops::INS, OpKind::ImplicitMem32_ES_EDI, OpKind::ImplicitDX),
		[0x6E] = entry_2(0x6E, ops::OUTSB, OpKind::ImplicitDX, OpKind::ImplicitMem8_ES_EDI),
		[0x6F] = entry_2(0x6F, ops::OUTS, OpKind::ImplicitDX, OpKind::ImplicitMem32_ES_EDI),

		[0x70] = entry_1_no_modrm(0x70, ops::JO,  OpKind::Rel8Offset),
		[0x71] = entry_1_no_modrm(0x71, ops::JNO, OpKind::Rel8Offset),
		[0x72] = entry_1_no_modrm(0x72, ops::JB,  OpKind::Rel8Offset),
		[0x73] = entry_1_no_modrm(0x73, ops::JNB, OpKind::Rel8Offset),
		[0x74] = entry_1_no_modrm(0x74, ops::JZ,  OpKind::Rel8Offset),
		[0x75] = entry_1_no_modrm(0x75, ops::JNZ, OpKind::Rel8Offset),
		[0x76] = entry_1_no_modrm(0x76, ops::JNA, OpKind::Rel8Offset),
		[0x77] = entry_1_no_modrm(0x77, ops::JA,  OpKind::Rel8Offset),

		[0x78] = entry_1_no_modrm(0x78, ops::JS,  OpKind::Rel8Offset),
		[0x79] = entry_1_no_modrm(0x79, ops::JNS, OpKind::Rel8Offset),
		[0x7A] = entry_1_no_modrm(0x7A, ops::JP,  OpKind::Rel8Offset),
		[0x7B] = entry_1_no_modrm(0x7B, ops::JNP, OpKind::Rel8Offset),
		[0x7C] = entry_1_no_modrm(0x7C, ops::JL,  OpKind::Rel8Offset),
		[0x7D] = entry_1_no_modrm(0x7D, ops::JGE, OpKind::Rel8Offset),
		[0x7E] = entry_1_no_modrm(0x7E, ops::JLE, OpKind::Rel8Offset),
		[0x7F] = entry_1_no_modrm(0x7F, ops::JG,  OpKind::Rel8Offset),

		[0x80] = entry_ext(0x80, &ModRMExt_80[0]),
		[0x81] = entry_ext(0x81, &ModRMExt_81[0]),
		[0x82] = entry_ext(0x82, &ModRMExt_82[0]),
		[0x83] = entry_ext(0x83, &ModRMExt_83[0]),
		[0x84] = entry_2(0x84, ops::TEST, OpKind::RegMem8,  OpKind::Reg8),
		[0x85] = entry_2(0x85, ops::TEST, OpKind::RegMem32, OpKind::Reg32),
		[0x86] = entry_2(0x86, ops::XCHG, OpKind::RegMem8,  OpKind::Reg8),
		[0x87] = entry_2(0x87, ops::XCHG, OpKind::RegMem32, OpKind::Reg32),

		[0x88] = entry_2(0x88, ops::MOV, OpKind::RegMem8,    OpKind::Reg8),
		[0x89] = entry_2(0x89, ops::MOV, OpKind::RegMem32,   OpKind::Reg32),
		[0x8A] = entry_2(0x8A, ops::MOV, OpKind::Reg8,       OpKind::RegMem8),
		[0x8B] = entry_2(0x8B, ops::MOV, OpKind::Reg32,      OpKind::RegMem32),
		[0x8C] = entry_2(0x8C, ops::MOV, OpKind::RegMem16,   OpKind::SegmentReg),
		[0x8D] = entry_2(0x8D, ops::LEA, OpKind::Reg32,      OpKind::Memory),
		[0x8E] = entry_2(0x8E, ops::MOV, OpKind::SegmentReg, OpKind::RegMem16),

		// XOP prefix (but also POP with ModRM.reg=0), for whatever fucking reason. handle this separately again.
		[0x8F] = entry_ext(0x8F, &ModRMExt_8F[0]),

		// all of these XCHGs are implicitly exchanging with RAX.
		[0x90] = entry_lnri_2(0x90, ops::XCHG, OpKind::ImplicitEAX, OpKind::Reg32),
		[0x91] = entry_lnri_2(0x91, ops::XCHG, OpKind::ImplicitEAX, OpKind::Reg32),
		[0x92] = entry_lnri_2(0x92, ops::XCHG, OpKind::ImplicitEAX, OpKind::Reg32),
		[0x93] = entry_lnri_2(0x93, ops::XCHG, OpKind::ImplicitEAX, OpKind::Reg32),
		[0x94] = entry_lnri_2(0x94, ops::XCHG, OpKind::ImplicitEAX, OpKind::Reg32),
		[0x95] = entry_lnri_2(0x95, ops::XCHG, OpKind::ImplicitEAX, OpKind::Reg32),
		[0x96] = entry_lnri_2(0x96, ops::XCHG, OpKind::ImplicitEAX, OpKind::Reg32),
		[0x97] = entry_lnri_2(0x97, ops::XCHG, OpKind::ImplicitEAX, OpKind::Reg32),

		[0x98] = entry_0(0x98, ops::CWDE),
		[0x99] = entry_0(0x99, ops::CDQ),
		[0x9A] = entry_1_no_modrm(0x9A, ops::CALLF, OpKind::Ptr16_32),      // far call, invalid in 64-bit
		[0x9B] = entry_0(0x9B, ops::FWAIT),
		[0x9C] = entry_0(0x9C, ops::PUSHF),
		[0x9D] = entry_0(0x9D, ops::POPF),
		[0x9E] = entry_0(0x9E, ops::SAHF),
		[0x9F] = entry_0(0x9F, ops::LAHF),

		[0xA0] = entry_2_no_modrm(0xA0, ops::MOV, OpKind::ImplicitAL,  OpKind::MemoryOfs8),
		[0xA1] = entry_2_no_modrm(0xA1, ops::MOV, OpKind::ImplicitEAX, OpKind::MemoryOfs32),
		[0xA2] = entry_2_no_modrm(0xA2, ops::MOV, OpKind::MemoryOfs8,  OpKind::ImplicitAL),
		[0xA3] = entry_2_no_modrm(0xA3, ops::MOV, OpKind::MemoryOfs32, OpKind::ImplicitEAX),
		[0xA4] = entry_2_no_modrm(0xA4, ops::MOVSB, OpKind::ImplicitMem8_ES_EDI, OpKind::ImplicitMem8_ESI),
		[0xA5] = entry_2_no_modrm(0xA5, ops::MOVS,  OpKind::ImplicitMem32_ES_EDI, OpKind::ImplicitMem32_ESI),
		[0xA6] = entry_2_no_modrm(0xA6, ops::CMPSB, OpKind::ImplicitMem8_ES_EDI, OpKind::ImplicitMem8_ESI),
		[0xA7] = entry_2_no_modrm(0xA7, ops::CMPS,  OpKind::ImplicitMem32_ES_EDI, OpKind::ImplicitMem32_ESI),

		[0xA8] = entry_2_no_modrm(0xA8, ops::TEST, OpKind::ImplicitAL,  OpKind::Imm8),
		[0xA9] = entry_2_no_modrm(0xA9, ops::TEST, OpKind::ImplicitEAX, OpKind::Imm32),
		[0xAA] = entry_2_no_modrm(0xAA, ops::STOSB, OpKind::ImplicitMem8_ES_EDI, OpKind::ImplicitAL),
		[0xAB] = entry_2_no_modrm(0xAB, ops::STOS,  OpKind::ImplicitMem32_ES_EDI, OpKind::ImplicitEAX),
		[0xAC] = entry_2_no_modrm(0xAC, ops::LODSB, OpKind::ImplicitAL, OpKind::ImplicitMem8_ESI),
		[0xAD] = entry_2_no_modrm(0xAD, ops::LODS,  OpKind::ImplicitEAX, OpKind::ImplicitMem32_ESI),
		[0xAE] = entry_2_no_modrm(0xAE, ops::SCASB, OpKind::ImplicitAL, OpKind::ImplicitMem8_ESI),
		[0xAF] = entry_2_no_modrm(0xAF, ops::SCAS,  OpKind::ImplicitEAX, OpKind::ImplicitMem32_ESI),

		[0xB0] = entry_lnri_2(0xB0, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		[0xB1] = entry_lnri_2(0xB1, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		[0xB2] = entry_lnri_2(0xB2, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		[0xB3] = entry_lnri_2(0xB3, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		[0xB4] = entry_lnri_2(0xB4, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		[0xB5] = entry_lnri_2(0xB5, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		[0xB6] = entry_lnri_2(0xB6, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		[0xB7] = entry_lnri_2(0xB7, ops::MOV, OpKind::Reg8, OpKind::Imm8),

		// encode the immediate size as 64-bits; the handling code will need to know how
		// to downgrade to either 32 or 16 bits depending on the prefix.
		[0xB8] = entry_lnri_2(0xB8, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		[0xB9] = entry_lnri_2(0xB9, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		[0xBA] = entry_lnri_2(0xBA, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		[0xBB] = entry_lnri_2(0xBB, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		[0xBC] = entry_lnri_2(0xBC, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		[0xBD] = entry_lnri_2(0xBD, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		[0xBE] = entry_lnri_2(0xBE, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		[0xBF] = entry_lnri_2(0xBF, ops::MOV, OpKind::Reg32, OpKind::Imm64),

		[0xC0] = entry_ext(0xC0, &ModRMExt_C0[0]),                          // the whole "group 2 extensions" of shift/rotate opcodes
		[0xC1] = entry_ext(0xC1, &ModRMExt_C1[0]),
		[0xC2] = entry_1_no_modrm(0xC2, ops::RET, OpKind::Imm16),
		[0xC3] = entry_0(0xC3, ops::RET),
		[0xC4] = entry_blank,                                               // vector extension escape sequence
		[0xC5] = entry_blank,                                               // vector extension escape sequence
		[0xC6] = entry_ext(0xC6, &ModRMExt_C6[0]),
		[0xC7] = entry_ext(0xC7, &ModRMExt_C7[0]),

		[0xC8] = entry_2_no_modrm(0xC8, ops::ENTER, OpKind::Imm16, OpKind::Imm8),
		[0xC9] = entry_0(0xC9, ops::LEAVE),
		[0xCA] = entry_1_no_modrm(0xCA, ops::RETF, OpKind::Imm16),
		[0xCB] = entry_0(0xCB, ops::RETF),
		[0xCC] = entry_0(0xCC, ops::INT3),
		[0xCD] = entry_1_no_modrm(0xCD, ops::INT, OpKind::Imm8),
		[0xCE] = entry_0(0xCE, ops::INTO),                                  // invalid in 64-bit
		[0xCF] = entry_0(0xCF, ops::IRET),

		[0xD0] = entry_ext(0xD0, &ModRMExt_D0[0]),
		[0xD1] = entry_ext(0xD1, &ModRMExt_D1[0]),
		[0xD2] = entry_ext(0xD2, &ModRMExt_D2[0]),
		[0xD3] = entry_ext(0xD3, &ModRMExt_D3[0]),
		[0xD4] = entry_1_no_modrm(0xD4, ops::AAM, OpKind::Imm8),            // invalid in 64-bit
		[0xD5] = entry_1_no_modrm(0xD5, ops::AAD, OpKind::Imm8),            // invalid in 64-bit
		[0xD6] = entry_blank,
		[0xD7] = entry_0(0xD7, ops::XLAT),


		// x87 fpu instructions, 0xD8 to 0xDF
		[0xD8] = entry_ext(0xD8, &ModRMExt_x87_D8[0]),
		[0xD9] = entry_ext(0xD9, &ModRMExt_x87_D9[0]),
		[0xDA] = entry_ext(0xDA, &ModRMExt_x87_DA[0]),
		[0xDB] = entry_ext(0xDB, &ModRMExt_x87_DB[0]),
		[0xDC] = entry_ext(0xDC, &ModRMExt_x87_DC[0]),
		[0xDD] = entry_ext(0xDD, &ModRMExt_x87_DD[0]),
		[0xDE] = entry_ext(0xDE, &ModRMExt_x87_DE[0]),
		[0xDF] = entry_ext(0xDF, &ModRMExt_x87_DF[0]),


		[0xE0] = entry_1_no_modrm(0xE0, ops::LOOPNZ, OpKind::Rel8Offset),
		[0xE1] = entry_1_no_modrm(0xE1, ops::LOOPZ,  OpKind::Rel8Offset),
		[0xE2] = entry_1_no_modrm(0xE2, ops::LOOP,   OpKind::Rel8Offset),
		[0xE3] = entry_1_no_modrm(0xE3, ops::JECXZ,  OpKind::Rel8Offset),
		[0xE4] = entry_2_no_modrm(0xE4, ops::IN,     OpKind::ImplicitAL, OpKind::Imm8),
		[0xE5] = entry_2_no_modrm(0xE5, ops::IN,     OpKind::ImplicitEAX, OpKind::Imm8),
		[0xE6] = entry_2_no_modrm(0xE6, ops::OUT,    OpKind::Imm8, OpKind::ImplicitAL),
		[0xE7] = entry_2_no_modrm(0xE7, ops::OUT,    OpKind::Imm8, OpKind::ImplicitEAX),

		[0xE8] = entry_1_no_modrm(0xE8, ops::CALL, OpKind::Rel32Offset),
		[0xE9] = entry_1_no_modrm(0xE9, ops::JMP,  OpKind::Rel32Offset),
		[0xEA] = entry_blank,
		[0xEB] = entry_1_no_modrm(0xEB, ops::JMP, OpKind::Rel8Offset),
		[0xEC] = entry_2_no_modrm(0xEC, ops::IN,  OpKind::ImplicitAL, OpKind::ImplicitDX),
		[0xED] = entry_2_no_modrm(0xED, ops::IN,  OpKind::ImplicitEAX, OpKind::ImplicitDX),
		[0xEE] = entry_2_no_modrm(0xEE, ops::OUT, OpKind::ImplicitDX, OpKind::ImplicitAL),
		[0xEF] = entry_2_no_modrm(0xEF, ops::OUT, OpKind::ImplicitDX, OpKind::ImplicitEAX),

		[0xF0] = entry_blank,                                               // lock prefix
		[0xF1] = entry_0(0xF1, ops::ICEBP),
		[0xF2] = entry_blank,                                               // repne prefix
		[0xF3] = entry_blank,                                               // rep prefix
		[0xF4] = entry_0(0xF4, ops::HLT),
		[0xF5] = entry_0(0xF5, ops::CMC),
		[0xF6] = entry_ext(0xF6, &ModRMExt_F6[0]),
		[0xF7] = entry_ext(0xF7, &ModRMExt_F7[0]),

		[0xF8] = entry_0(0xF8, ops::CLC),
		[0xF9] = entry_0(0xF9, ops::STC),
		[0xFA] = entry_0(0xFA, ops::CLI),
		[0xFB] = entry_0(0xFB, ops::STI),
		[0xFC] = entry_0(0xFC, ops::CLD),
		[0xFD] = entry_0(0xFD, ops::STD),
		[0xFE] = entry_ext(0xFE, &ModRMExt_FE[0]),
		[0xFF] = entry_ext(0xFF, &ModRMExt_FF[0]),
	};
}
