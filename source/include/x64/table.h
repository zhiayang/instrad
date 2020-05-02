// table.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "defs.h"
#include "ops.h"

namespace instrad::x64
{
	enum class OpKind
	{
		Reg8,
		Reg16,
		Reg32,
		Reg64,

		RegMem8,
		RegMem16,
		RegMem32,
		RegMem64,

		Imm8,
		Imm16,
		Imm32,
		Imm64,          // the 0xB8 mov is the only instruction that uses a real 8-byte immediate

		SignExtImm8,
		SignExtImm32,

		Memory,         // only used by LEA
		SegmentReg,     // only used by MOV
		ControlReg,
		DebugReg,

		ImplicitCS,
		ImplicitDS,
		ImplicitES,
		ImplicitFS,
		ImplicitGS,
		ImplicitSS,

		ImplicitCL,
		ImplicitCX,
		ImplicitECX,

		ImplicitDX,

		ImplicitAL,
		ImplicitAX,
		ImplicitEAX,
		ImplicitRAX,

		Rel8Offset,
		Rel16Offset,
		Rel32Offset,

		MemoryOfs8,
		MemoryOfs16,
		MemoryOfs32,
		MemoryOfs64,

		Mem8,
		Mem16,
		Mem32,
		Mem64,
		Mem128,
		Mem256,

		MmxRMReg,   // some dumb instructions encode the (only) register
		XmmRMReg,   // operand in modRM.rm, instead of modRM.reg. use this so we know which to use.

		MmxReg,
		MmxRegMem32,
		MmxRegMem64,

		XmmReg,
		XmmRegMem32,
		XmmRegMem64,
		XmmRegMem128,

		YmmReg,
		YmmRegMem256,

		// dumbness
		Reg32Mem16,


		Ptr16_16,       // for far calls and jumps
		Ptr16_32,

		None,
	};


	struct TableEntry
	{
	private:
		using __Foozle = OpKind[3];

	public:
		constexpr bool present() const { return this->m_op != ops::INVALID; }

		constexpr const TableEntry* extension() const { return this->m_exttable; }
		constexpr bool extensionUsesRMBits() const { return this->m_extensionUsesRMBits; }
		constexpr bool extensionUsesModBits() const { return this->m_extensionUsesModBits; }
		constexpr const __Foozle& operands() const { return this->m_operands; }
		constexpr bool isDirectRegisterIdx() const { return this->m_lowNibbleRegIdx; }

		constexpr int numOperands() const { return this->m_numOperands; }
		constexpr uint8_t opcode() const { return this->m_opcode; }
		constexpr const Op& op() const { return this->m_op; }
		constexpr bool needsModRM() const { return this->m_needsModRM; }

	private:
		Op m_op = ops::INVALID;
		uint8_t m_opcode = 0;
		int m_numOperands = 0;
		bool m_lowNibbleRegIdx = false;
		OpKind m_operands[3] = { OpKind::None, OpKind::None, OpKind::None };

		bool m_needsModRM = false;
		bool m_extensionUsesRMBits = false;
		bool m_extensionUsesModBits = false;
		const TableEntry* m_exttable = nullptr;

		friend constexpr TableEntry entry_none(uint8_t);
		friend constexpr TableEntry entry_0(uint8_t, const Op&);
		friend constexpr TableEntry entry_0_modrm(uint8_t, const Op&);
		friend constexpr TableEntry entry_1(uint8_t, const Op&, OpKind);
		friend constexpr TableEntry entry_2(uint8_t, const Op&, OpKind, OpKind);
		friend constexpr TableEntry entry_3(uint8_t, const Op&, OpKind, OpKind, OpKind);
		friend constexpr TableEntry entry_lnri_1(uint8_t, const Op&, OpKind);
		friend constexpr TableEntry entry_lnri_2(uint8_t, const Op&, OpKind, OpKind);
		friend constexpr TableEntry entry_ext(uint8_t, const TableEntry*);
		friend constexpr TableEntry entry_ext_rm(uint8_t, const TableEntry*);
		friend constexpr TableEntry entry_ext_mod(uint8_t, const TableEntry*);

		friend constexpr TableEntry entry_1_no_modrm(uint8_t, const Op&, OpKind);
		friend constexpr TableEntry entry_2_no_modrm(uint8_t, const Op&, OpKind, OpKind);
	};

	constexpr TableEntry entry_none(uint8_t opcode)
	{
		auto ret = TableEntry();
		ret.m_opcode = opcode;
		return ret;
	}

	// entry_0 takes no modrm by default.
	constexpr TableEntry entry_0(uint8_t opcode, const Op& op)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 0;
		ret.m_needsModRM = false;
		return ret;
	}

	// used for stuff in extension tables; takes no operands but still needs modrm.
	constexpr TableEntry entry_0_modrm(uint8_t opcode, const Op& op)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 0;
		ret.m_needsModRM = true;
		return ret;
	}

	constexpr TableEntry entry_1(uint8_t opcode, const Op& op, OpKind dst)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 1;
		ret.m_operands[0] = dst;
		ret.m_needsModRM = true;
		return ret;
	}

	constexpr TableEntry entry_2(uint8_t opcode, const Op& op, OpKind dst, OpKind src)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 2;
		ret.m_operands[0] = dst;
		ret.m_operands[1] = src;
		ret.m_needsModRM = true;
		return ret;
	}

	constexpr TableEntry entry_3(uint8_t opcode, const Op& op, OpKind dst, OpKind src, OpKind extra)
	{
		// only used by imul
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 3;
		ret.m_needsModRM = true;
		ret.m_operands[0] = dst;
		ret.m_operands[1] = src;
		ret.m_operands[2] = extra;
		return ret;
	}

	constexpr TableEntry entry_1_no_modrm(uint8_t opcode, const Op& op, OpKind dst)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 1;
		ret.m_operands[0] = dst;
		ret.m_needsModRM = false;
		return ret;
	}

	constexpr TableEntry entry_2_no_modrm(uint8_t opcode, const Op& op, OpKind dst, OpKind src)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 2;
		ret.m_operands[0] = dst;
		ret.m_operands[1] = src;
		ret.m_needsModRM = false;
		return ret;
	}

	// so far, there aren't any one-operand LNRI opcodes that need a modRM byte.
	constexpr TableEntry entry_lnri_1(uint8_t opcode, const Op& op, OpKind dst)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 1;
		ret.m_operands[0] = dst;
		ret.m_needsModRM = false;
		ret.m_lowNibbleRegIdx = true;
		return ret;
	}

	// so far, there aren't any two-operand LNRI opcodes that need a modRM byte either.
	constexpr TableEntry entry_lnri_2(uint8_t opcode, const Op& op, OpKind dst, OpKind src)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 2;
		ret.m_operands[0] = dst;
		ret.m_operands[1] = src;
		ret.m_needsModRM = false;
		ret.m_lowNibbleRegIdx = true;
		return ret;
	}

	// this one is the "normal" modRM.reg extension
	constexpr TableEntry entry_ext(uint8_t opcode, const TableEntry* ext)
	{
		auto ret = TableEntry();
		ret.m_opcode = opcode;
		ret.m_exttable = ext;
		return ret;
	}

	// this one is for modRM.rm extensions... ugh.
	constexpr TableEntry entry_ext_rm(uint8_t opcode, const TableEntry* ext)
	{
		auto ret = TableEntry();
		ret.m_opcode = opcode;
		ret.m_exttable = ext;
		ret.m_extensionUsesRMBits = true;
		return ret;
	}

	// this one only uses the mod bits... ughhhhhh
	constexpr TableEntry entry_ext_mod(uint8_t opcode, const TableEntry* ext)
	{
		auto ret = TableEntry();
		ret.m_opcode = opcode;
		ret.m_exttable = ext;
		ret.m_extensionUsesModBits = true;
		return ret;
	}



	constexpr TableEntry entry_blank = entry_none(0);

	template <typename T, size_t N>
	constexpr size_t ArrayLength(const T (&arr)[N]) { return N; }

	// all the entry_N functions imply that the instructions accept a modRM byte, except entry_0
	// (but there is an entry_0_modrm to specify that a 0-operand opcode needs modRM).
	// there is entry_1_no_modrm and entry_2_no_modrm to specify 1 and 2-operand instructions that
	// don't take a modRM byte.

	// the extension tables are indexed by the ModRM.REG value.
	constexpr TableEntry ModRMExt_80[] = {
		entry_2(0x80, ops::ADD, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x80, ops::OR,  OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x80, ops::ADC, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x80, ops::SBB, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x80, ops::AND, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x80, ops::SUB, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x80, ops::XOR, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x80, ops::CMP, OpKind::RegMem8, OpKind::Imm8),
	};

	constexpr TableEntry ModRMExt_81[] = {
		entry_2(0x81, ops::ADD, OpKind::RegMem32, OpKind::Imm32),
		entry_2(0x81, ops::OR,  OpKind::RegMem32, OpKind::Imm32),
		entry_2(0x81, ops::ADC, OpKind::RegMem32, OpKind::Imm32),
		entry_2(0x81, ops::SBB, OpKind::RegMem32, OpKind::Imm32),
		entry_2(0x81, ops::AND, OpKind::RegMem32, OpKind::Imm32),
		entry_2(0x81, ops::SUB, OpKind::RegMem32, OpKind::Imm32),
		entry_2(0x81, ops::XOR, OpKind::RegMem32, OpKind::Imm32),
		entry_2(0x81, ops::CMP, OpKind::RegMem32, OpKind::Imm32),
	};

	// these are apparently invalid in 64-bit mode.
	constexpr TableEntry ModRMExt_82[] = {
		entry_2(0x82, ops::ADD, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x82, ops::OR,  OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x82, ops::ADC, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x82, ops::SBB, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x82, ops::AND, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x82, ops::SUB, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x82, ops::XOR, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0x82, ops::CMP, OpKind::RegMem8, OpKind::Imm8),
	};

	constexpr TableEntry ModRMExt_83[] = {
		entry_2(0x83, ops::ADD, OpKind::RegMem32, OpKind::SignExtImm8),
		entry_2(0x83, ops::OR,  OpKind::RegMem32, OpKind::SignExtImm8),
		entry_2(0x83, ops::ADC, OpKind::RegMem32, OpKind::SignExtImm8),
		entry_2(0x83, ops::SBB, OpKind::RegMem32, OpKind::SignExtImm8),
		entry_2(0x83, ops::AND, OpKind::RegMem32, OpKind::SignExtImm8),
		entry_2(0x83, ops::SUB, OpKind::RegMem32, OpKind::SignExtImm8),
		entry_2(0x83, ops::XOR, OpKind::RegMem32, OpKind::SignExtImm8),
		entry_2(0x83, ops::CMP, OpKind::RegMem32, OpKind::SignExtImm8),
	};

	constexpr TableEntry ModRMExt_8F[] = {
		entry_1(0x8F, ops::POP, OpKind::RegMem64),

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank
	};

	constexpr TableEntry ModRMExt_C0[] = {
		entry_2(0xC0, ops::ROL, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0xC0, ops::ROR, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0xC0, ops::RCL, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0xC0, ops::RCR, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0xC0, ops::SHL, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0xC0, ops::SHR, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0xC0, ops::SAL, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0xC0, ops::SAR, OpKind::RegMem8, OpKind::Imm8),
	};

	constexpr TableEntry ModRMExt_C1[] = {
		entry_2(0xC1, ops::ROL, OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xC1, ops::ROR, OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xC1, ops::RCL, OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xC1, ops::RCR, OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xC1, ops::SHL, OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xC1, ops::SHR, OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xC1, ops::SAL, OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xC1, ops::SAR, OpKind::RegMem32, OpKind::Imm8),
	};

	constexpr TableEntry ModRMExt_C6[] = {
		entry_2(0xC6, ops::MOV, OpKind::RegMem8, OpKind::Imm8),

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};

	constexpr TableEntry ModRMExt_C7[] = {
		entry_2(0xC7, ops::MOV, OpKind::RegMem32, OpKind::Imm32),

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};

	constexpr TableEntry ModRMExt_D0[] = {
		entry_1(0xD0, ops::ROL, OpKind::RegMem8),
		entry_1(0xD0, ops::ROR, OpKind::RegMem8),
		entry_1(0xD0, ops::RCL, OpKind::RegMem8),
		entry_1(0xD0, ops::RCR, OpKind::RegMem8),
		entry_1(0xD0, ops::SHL, OpKind::RegMem8),
		entry_1(0xD0, ops::SHR, OpKind::RegMem8),
		entry_1(0xD0, ops::SAL, OpKind::RegMem8),
		entry_1(0xD0, ops::SAR, OpKind::RegMem8),
	};

	constexpr TableEntry ModRMExt_D1[] = {
		entry_1(0xD1, ops::ROL, OpKind::RegMem32),
		entry_1(0xD1, ops::ROR, OpKind::RegMem32),
		entry_1(0xD1, ops::RCL, OpKind::RegMem32),
		entry_1(0xD1, ops::RCR, OpKind::RegMem32),
		entry_1(0xD1, ops::SHL, OpKind::RegMem32),
		entry_1(0xD1, ops::SHR, OpKind::RegMem32),
		entry_1(0xD1, ops::SAL, OpKind::RegMem32),
		entry_1(0xD1, ops::SAR, OpKind::RegMem32),
	};

	constexpr TableEntry ModRMExt_D2[] = {
		entry_2(0xD2, ops::ROL, OpKind::RegMem8, OpKind::ImplicitCL),
		entry_2(0xD2, ops::ROR, OpKind::RegMem8, OpKind::ImplicitCL),
		entry_2(0xD2, ops::RCL, OpKind::RegMem8, OpKind::ImplicitCL),
		entry_2(0xD2, ops::RCR, OpKind::RegMem8, OpKind::ImplicitCL),
		entry_2(0xD2, ops::SHL, OpKind::RegMem8, OpKind::ImplicitCL),
		entry_2(0xD2, ops::SHR, OpKind::RegMem8, OpKind::ImplicitCL),
		entry_2(0xD2, ops::SAL, OpKind::RegMem8, OpKind::ImplicitCL),
		entry_2(0xD2, ops::SAR, OpKind::RegMem8, OpKind::ImplicitCL),
	};

	constexpr TableEntry ModRMExt_D3[] = {
		entry_2(0xD3, ops::ROL, OpKind::RegMem32, OpKind::ImplicitCL),
		entry_2(0xD3, ops::ROR, OpKind::RegMem32, OpKind::ImplicitCL),
		entry_2(0xD3, ops::RCL, OpKind::RegMem32, OpKind::ImplicitCL),
		entry_2(0xD3, ops::RCR, OpKind::RegMem32, OpKind::ImplicitCL),
		entry_2(0xD3, ops::SHL, OpKind::RegMem32, OpKind::ImplicitCL),
		entry_2(0xD3, ops::SHR, OpKind::RegMem32, OpKind::ImplicitCL),
		entry_2(0xD3, ops::SAL, OpKind::RegMem32, OpKind::ImplicitCL),
		entry_2(0xD3, ops::SAR, OpKind::RegMem32, OpKind::ImplicitCL),
	};


	constexpr TableEntry ModRMExt_F6[] = {
		entry_2(0xF6, ops::TEST, OpKind::RegMem8, OpKind::Imm8),
		entry_2(0xF6, ops::TEST, OpKind::RegMem8, OpKind::Imm8),
		entry_1(0xF6, ops::NOT,  OpKind::RegMem8),
		entry_1(0xF6, ops::NEG,  OpKind::RegMem8),
		entry_1(0xF6, ops::MUL,  OpKind::RegMem8),
		entry_1(0xF6, ops::IMUL, OpKind::RegMem8),
		entry_1(0xF6, ops::DIV,  OpKind::RegMem8),
		entry_1(0xF6, ops::IDIV, OpKind::RegMem8),
	};

	constexpr TableEntry ModRMExt_F7[] = {
		entry_2(0xF7, ops::TEST, OpKind::RegMem32, OpKind::Imm32),
		entry_2(0xF7, ops::TEST, OpKind::RegMem32, OpKind::Imm32),
		entry_1(0xF7, ops::NOT,  OpKind::RegMem32),
		entry_1(0xF7, ops::NEG,  OpKind::RegMem32),
		entry_1(0xF7, ops::MUL,  OpKind::RegMem32),
		entry_1(0xF7, ops::IMUL, OpKind::RegMem32),
		entry_1(0xF7, ops::DIV,  OpKind::RegMem32),
		entry_1(0xF7, ops::IDIV, OpKind::RegMem32),
	};

	constexpr TableEntry ModRMExt_FE[] = {
		entry_1(0xFE, ops::INC, OpKind::RegMem8),
		entry_1(0xFE, ops::DEC, OpKind::RegMem8),

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};

	constexpr TableEntry ModRMExt_FF[] = {
		entry_1(0xFF, ops::INC,  OpKind::RegMem32),
		entry_1(0xFF, ops::DEC,  OpKind::RegMem32),
		entry_1(0xFF, ops::CALL, OpKind::RegMem64),
		entry_1(0xFF, ops::CALL, OpKind::Ptr16_32),
		entry_1(0xFF, ops::JMP,  OpKind::RegMem64),
		entry_1(0xFF, ops::JMP,  OpKind::Ptr16_32),
		entry_1(0xFF, ops::PUSH, OpKind::RegMem64),

		entry_blank,
	};








	constexpr TableEntry PrimaryOpcodeMap[] = {
		entry_2(0x00, ops::ADD,  OpKind::RegMem8, OpKind::Reg8),
		entry_2(0x01, ops::ADD,  OpKind::RegMem32, OpKind::Reg32),
		entry_2(0x02, ops::ADD,  OpKind::Reg8, OpKind::RegMem8),
		entry_2(0x03, ops::ADD,  OpKind::Reg32, OpKind::RegMem32),
		entry_2_no_modrm(0x04, ops::ADD,  OpKind::ImplicitAL, OpKind::Imm8),
		entry_2_no_modrm(0x05, ops::ADD,  OpKind::ImplicitEAX, OpKind::Imm32),
		entry_1_no_modrm(0x06, ops::PUSH, OpKind::ImplicitES),              // push es; invalid in 64-bit
		entry_1_no_modrm(0x07, ops::POP,  OpKind::ImplicitES),              // pop es; invalid in 64-bit

		entry_2(0x08, ops::OR, OpKind::RegMem8, OpKind::Reg8),
		entry_2(0x09, ops::OR, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0x0A, ops::OR, OpKind::Reg8, OpKind::RegMem8),
		entry_2(0x0B, ops::OR, OpKind::Reg32, OpKind::RegMem32),
		entry_2_no_modrm(0x0C, ops::OR,   OpKind::ImplicitAL, OpKind::Imm8),
		entry_2_no_modrm(0x0D, ops::OR,   OpKind::ImplicitEAX, OpKind::Imm32),
		entry_1_no_modrm(0x0E, ops::PUSH, OpKind::ImplicitCS),              // push cs; invalid in 64-bit
		entry_none(0x0F),                                                   // escape sequence.

		entry_2(0x10, ops::ADC, OpKind::RegMem8, OpKind::Reg8),
		entry_2(0x11, ops::ADC, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0x12, ops::ADC, OpKind::Reg8, OpKind::RegMem8),
		entry_2(0x13, ops::ADC, OpKind::Reg32, OpKind::RegMem32),
		entry_2_no_modrm(0x14, ops::ADC,  OpKind::ImplicitAL, OpKind::Imm8),
		entry_2_no_modrm(0x15, ops::ADC,  OpKind::ImplicitEAX, OpKind::Imm32),
		entry_1_no_modrm(0x16, ops::PUSH, OpKind::ImplicitSS),              // push ss; invalid in 64-bit
		entry_1_no_modrm(0x17, ops::POP,  OpKind::ImplicitSS),              // pop ss; invalid in 64-bit

		entry_2(0x18, ops::SBB, OpKind::RegMem8, OpKind::Reg8),
		entry_2(0x19, ops::SBB, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0x1A, ops::SBB, OpKind::Reg8, OpKind::RegMem8),
		entry_2(0x1B, ops::SBB, OpKind::Reg32, OpKind::RegMem32),
		entry_2_no_modrm(0x1C, ops::SBB,  OpKind::ImplicitAL, OpKind::Imm8),
		entry_2_no_modrm(0x1D, ops::SBB,  OpKind::ImplicitEAX, OpKind::Imm32),
		entry_1_no_modrm(0x1E, ops::PUSH, OpKind::ImplicitDS),              // push ds; invalid in 64-bit
		entry_1_no_modrm(0x1F, ops::POP,  OpKind::ImplicitDS),              // pop ds; invalid in 64-bit

		entry_2(0x20, ops::AND, OpKind::RegMem8, OpKind::Reg8),
		entry_2(0x21, ops::AND, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0x22, ops::AND, OpKind::Reg8, OpKind::RegMem8),
		entry_2(0x23, ops::AND, OpKind::Reg32, OpKind::RegMem32),
		entry_2_no_modrm(0x24, ops::AND, OpKind::ImplicitAL, OpKind::Imm8),
		entry_2_no_modrm(0x25, ops::AND, OpKind::ImplicitEAX, OpKind::Imm32),
		entry_none(0x26),                                                   // ES segment override
		entry_0(0x27, ops::DAA),                                            // invalid in 64-bit mode

		entry_2(0x28, ops::SUB, OpKind::RegMem8, OpKind::Reg8),
		entry_2(0x29, ops::SUB, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0x2A, ops::SUB, OpKind::Reg8, OpKind::RegMem8),
		entry_2(0x2B, ops::SUB, OpKind::Reg32, OpKind::RegMem32),
		entry_2_no_modrm(0x2C, ops::SUB, OpKind::ImplicitAL, OpKind::Imm8),
		entry_2_no_modrm(0x2D, ops::SUB, OpKind::ImplicitEAX, OpKind::Imm32),
		entry_none(0x2E),                                                   // CS segment override
		entry_0(0x2F, ops::DAS),                                            // invalid in 64-bit mode

		entry_2(0x30, ops::XOR, OpKind::RegMem8, OpKind::Reg8),
		entry_2(0x31, ops::XOR, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0x32, ops::XOR, OpKind::Reg8, OpKind::RegMem8),
		entry_2(0x33, ops::XOR, OpKind::Reg32, OpKind::RegMem32),
		entry_2_no_modrm(0x34, ops::XOR, OpKind::ImplicitAL, OpKind::Imm8),
		entry_2_no_modrm(0x35, ops::XOR, OpKind::ImplicitEAX, OpKind::Imm32),
		entry_none(0x36),                                                   // SS segment override
		entry_0(0x37, ops::AAA),                                            // invalid in 64-bit mode

		entry_2(0x38, ops::CMP, OpKind::RegMem8, OpKind::Reg8),
		entry_2(0x39, ops::CMP, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0x3A, ops::CMP, OpKind::Reg8, OpKind::RegMem8),
		entry_2(0x3B, ops::CMP, OpKind::Reg32, OpKind::RegMem32),
		entry_2_no_modrm(0x3C, ops::CMP, OpKind::ImplicitAL, OpKind::Imm8),
		entry_2_no_modrm(0x3D, ops::CMP, OpKind::ImplicitEAX, OpKind::Imm32),
		entry_none(0x3E),                                                   // DS segment override
		entry_0(0x3F, ops::AAS),                                            // invalid in 64-bit mode

		// in 64-bit mode, this is the REX prefix. but we want to support x86 also, so these are the inc/dec opcodes
		entry_lnri_1(0x40, ops::INC, OpKind::Reg32),
		entry_lnri_1(0x41, ops::INC, OpKind::Reg32),
		entry_lnri_1(0x42, ops::INC, OpKind::Reg32),
		entry_lnri_1(0x43, ops::INC, OpKind::Reg32),
		entry_lnri_1(0x44, ops::INC, OpKind::Reg32),
		entry_lnri_1(0x45, ops::INC, OpKind::Reg32),
		entry_lnri_1(0x46, ops::INC, OpKind::Reg32),
		entry_lnri_1(0x47, ops::INC, OpKind::Reg32),

		entry_lnri_1(0x48, ops::DEC, OpKind::Reg32),
		entry_lnri_1(0x49, ops::DEC, OpKind::Reg32),
		entry_lnri_1(0x4A, ops::DEC, OpKind::Reg32),
		entry_lnri_1(0x4B, ops::DEC, OpKind::Reg32),
		entry_lnri_1(0x4C, ops::DEC, OpKind::Reg32),
		entry_lnri_1(0x4D, ops::DEC, OpKind::Reg32),
		entry_lnri_1(0x4E, ops::DEC, OpKind::Reg32),
		entry_lnri_1(0x4F, ops::DEC, OpKind::Reg32),

		entry_lnri_1(0x50, ops::PUSH, OpKind::Reg64),
		entry_lnri_1(0x51, ops::PUSH, OpKind::Reg64),
		entry_lnri_1(0x52, ops::PUSH, OpKind::Reg64),
		entry_lnri_1(0x53, ops::PUSH, OpKind::Reg64),
		entry_lnri_1(0x54, ops::PUSH, OpKind::Reg64),
		entry_lnri_1(0x55, ops::PUSH, OpKind::Reg64),
		entry_lnri_1(0x56, ops::PUSH, OpKind::Reg64),
		entry_lnri_1(0x57, ops::PUSH, OpKind::Reg64),

		entry_lnri_1(0x58, ops::POP, OpKind::Reg64),
		entry_lnri_1(0x59, ops::POP, OpKind::Reg64),
		entry_lnri_1(0x5A, ops::POP, OpKind::Reg64),
		entry_lnri_1(0x5B, ops::POP, OpKind::Reg64),
		entry_lnri_1(0x5C, ops::POP, OpKind::Reg64),
		entry_lnri_1(0x5D, ops::POP, OpKind::Reg64),
		entry_lnri_1(0x5E, ops::POP, OpKind::Reg64),
		entry_lnri_1(0x5F, ops::POP, OpKind::Reg64),

		entry_0(0x60, ops::PUSHAD),
		entry_0(0x61, ops::POPAD),
		entry_2(0x62, ops::BOUND, OpKind::Reg32, OpKind::RegMem32),

		// in legacy mode this is interpreted as ARPL, but who cares? long mode ftw.
		entry_2(0x63, ops::MOVSXD, OpKind::Reg64, OpKind::RegMem64),
		entry_none(0x64),                                                   // FS segment override
		entry_none(0x65),                                                   // GS segment override
		entry_none(0x66),                                                   // operand size override
		entry_none(0x67),                                                   // address size override

		entry_1_no_modrm(0x68, ops::PUSH, OpKind::SignExtImm32),
		entry_3(0x69, ops::IMUL, OpKind::Reg32, OpKind::RegMem32, OpKind::Imm32),
		entry_1_no_modrm(0x6A, ops::PUSH, OpKind::Imm8),
		entry_3(0x6B, ops::IMUL, OpKind::Reg32, OpKind::RegMem32, OpKind::Imm8),
		entry_0(0x6C, ops::INSB),
		entry_0(0x6D, ops::INSD),
		entry_0(0x6E, ops::OUTSB),
		entry_0(0x6F, ops::OUTSD),

		entry_1_no_modrm(0x70, ops::JO,  OpKind::Rel8Offset),
		entry_1_no_modrm(0x71, ops::JNO, OpKind::Rel8Offset),
		entry_1_no_modrm(0x72, ops::JB,  OpKind::Rel8Offset),
		entry_1_no_modrm(0x73, ops::JNB, OpKind::Rel8Offset),
		entry_1_no_modrm(0x74, ops::JZ,  OpKind::Rel8Offset),
		entry_1_no_modrm(0x75, ops::JNZ, OpKind::Rel8Offset),
		entry_1_no_modrm(0x76, ops::JNA, OpKind::Rel8Offset),
		entry_1_no_modrm(0x77, ops::JA,  OpKind::Rel8Offset),

		entry_1_no_modrm(0x78, ops::JS,  OpKind::Rel8Offset),
		entry_1_no_modrm(0x79, ops::JNS, OpKind::Rel8Offset),
		entry_1_no_modrm(0x7A, ops::JP,  OpKind::Rel8Offset),
		entry_1_no_modrm(0x7B, ops::JNP, OpKind::Rel8Offset),
		entry_1_no_modrm(0x7C, ops::JL,  OpKind::Rel8Offset),
		entry_1_no_modrm(0x7D, ops::JGE, OpKind::Rel8Offset),
		entry_1_no_modrm(0x7E, ops::JLE, OpKind::Rel8Offset),
		entry_1_no_modrm(0x7F, ops::JG,  OpKind::Rel8Offset),

		// 0x80 to 0x83 are part of the "group 1" ModRM.reg extensions.
		entry_ext(0x80, &ModRMExt_80[0]),
		entry_ext(0x81, &ModRMExt_81[0]),
		entry_ext(0x82, &ModRMExt_82[0]),
		entry_ext(0x83, &ModRMExt_83[0]),
		entry_2(0x84, ops::TEST, OpKind::RegMem8,  OpKind::Reg8),
		entry_2(0x85, ops::TEST, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0x86, ops::XCHG, OpKind::RegMem8,  OpKind::Reg8),
		entry_2(0x87, ops::XCHG, OpKind::RegMem32, OpKind::Reg32),

		entry_2(0x88, ops::MOV, OpKind::RegMem8,    OpKind::Reg8),
		entry_2(0x89, ops::MOV, OpKind::RegMem32,   OpKind::Reg32),
		entry_2(0x8A, ops::MOV, OpKind::Reg8,       OpKind::RegMem8),
		entry_2(0x8B, ops::MOV, OpKind::Reg32,      OpKind::RegMem32),
		entry_2(0x8C, ops::MOV, OpKind::RegMem16,   OpKind::SegmentReg),
		entry_2(0x8D, ops::LEA, OpKind::Reg32,      OpKind::Memory),
		entry_2(0x8E, ops::MOV, OpKind::SegmentReg, OpKind::RegMem16),

		// XOP prefix (but also POP with ModRM.reg=0), for whatever fucking reason. handle this separately again.
		entry_ext(0x8F, &ModRMExt_8F[0]),

		// all of these XCHGs are implicitly exchanging with RAX.
		entry_lnri_2(0x90, ops::XCHG, OpKind::ImplicitRAX, OpKind::Reg64),
		entry_lnri_2(0x91, ops::XCHG, OpKind::ImplicitRAX, OpKind::Reg64),
		entry_lnri_2(0x92, ops::XCHG, OpKind::ImplicitRAX, OpKind::Reg64),
		entry_lnri_2(0x93, ops::XCHG, OpKind::ImplicitRAX, OpKind::Reg64),
		entry_lnri_2(0x94, ops::XCHG, OpKind::ImplicitRAX, OpKind::Reg64),
		entry_lnri_2(0x95, ops::XCHG, OpKind::ImplicitRAX, OpKind::Reg64),
		entry_lnri_2(0x96, ops::XCHG, OpKind::ImplicitRAX, OpKind::Reg64),
		entry_lnri_2(0x97, ops::XCHG, OpKind::ImplicitRAX, OpKind::Reg64),

		entry_0(0x98, ops::CWDE),
		entry_0(0x99, ops::CDQ),
		entry_1_no_modrm(0x9A, ops::CALLF, OpKind::Ptr16_32),               // far call, invalid in 64-bit
		entry_0(0x9B, ops::FWAIT),
		entry_0(0x9C, ops::PUSHF),
		entry_0(0x9D, ops::POPF),
		entry_0(0x9E, ops::SAHF),
		entry_0(0x9F, ops::LAHF),

		entry_2_no_modrm(0xA0, ops::MOV, OpKind::ImplicitAL,  OpKind::MemoryOfs8),
		entry_2_no_modrm(0xA1, ops::MOV, OpKind::ImplicitEAX, OpKind::MemoryOfs32),
		entry_2_no_modrm(0xA2, ops::MOV, OpKind::MemoryOfs8,  OpKind::ImplicitAL),
		entry_2_no_modrm(0xA3, ops::MOV, OpKind::MemoryOfs32, OpKind::ImplicitEAX),
		entry_0(0xA4, ops::MOVSB),
		entry_0(0xA5, ops::MOVSD),
		entry_0(0xA6, ops::CMPSB),
		entry_0(0xA7, ops::CMPSD),

		entry_2_no_modrm(0xA8, ops::TEST, OpKind::ImplicitAL,  OpKind::Imm8),
		entry_2_no_modrm(0xA9, ops::TEST, OpKind::ImplicitEAX, OpKind::Imm32),
		entry_0(0xAA, ops::STOSB),
		entry_0(0xAB, ops::STOSD),
		entry_0(0xAC, ops::LODSB),
		entry_0(0xAD, ops::LODSD),
		entry_0(0xAE, ops::SCASB),
		entry_0(0xAF, ops::SCASD),

		entry_lnri_2(0xB0, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		entry_lnri_2(0xB1, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		entry_lnri_2(0xB2, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		entry_lnri_2(0xB3, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		entry_lnri_2(0xB4, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		entry_lnri_2(0xB5, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		entry_lnri_2(0xB6, ops::MOV, OpKind::Reg8, OpKind::Imm8),
		entry_lnri_2(0xB7, ops::MOV, OpKind::Reg8, OpKind::Imm8),

		// encode the immediate size as 64-bits; the handling code will need to know how
		// to downgrade to either 32 or 16 bits depending on the prefix.
		entry_lnri_2(0xB8, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		entry_lnri_2(0xB9, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		entry_lnri_2(0xBA, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		entry_lnri_2(0xBB, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		entry_lnri_2(0xBC, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		entry_lnri_2(0xBD, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		entry_lnri_2(0xBE, ops::MOV, OpKind::Reg32, OpKind::Imm64),
		entry_lnri_2(0xBF, ops::MOV, OpKind::Reg32, OpKind::Imm64),

		entry_ext(0xC0, &ModRMExt_C0[0]),                                   // the whole "group 2 extensions" of shift/rotate opcodes
		entry_ext(0xC1, &ModRMExt_C1[0]),
		entry_1_no_modrm(0xC2, ops::RET, OpKind::Imm16),
		entry_0(0xC3, ops::RET),
		entry_none(0xC4),                                                   // vector extension escape sequence
		entry_none(0xC5),                                                   // vector extension escape sequence
		entry_ext(0xC6, &ModRMExt_C6[0]),
		entry_ext(0xC7, &ModRMExt_C7[0]),

		entry_2_no_modrm(0xC8, ops::ENTER, OpKind::Imm16, OpKind::Imm8),
		entry_0(0xC9, ops::LEAVE),
		entry_1_no_modrm(0xCA, ops::RETF, OpKind::Imm16),
		entry_0(0xCB, ops::RETF),
		entry_0(0xCC, ops::INT3),
		entry_1_no_modrm(0xCD, ops::INT, OpKind::Imm8),
		entry_0(0xCE, ops::INTO),                                           // invalid in 64-bit
		entry_0(0xCF, ops::IRET),

		entry_ext(0xD0, &ModRMExt_D0[0]),                                   // group 2 extensions, shift/rotate
		entry_ext(0xD1, &ModRMExt_D1[0]),                                   // group 2 extensions, shift/rotate
		entry_ext(0xD2, &ModRMExt_D2[0]),                                   // group 2 extensions, shift/rotate
		entry_ext(0xD3, &ModRMExt_D3[0]),                                   // group 2 extensions, shift/rotate
		entry_1_no_modrm(0xD4, ops::AAM, OpKind::Imm8),                     // invalid in 64-bit
		entry_1_no_modrm(0xD5, ops::AAD, OpKind::Imm8),                     // invalid in 64-bit
		entry_none(0xD6),                                                   // straight up invalid
		entry_0(0xD7, ops::XLAT),

		// x87 fpu instructions
		entry_none(0xD8),
		entry_none(0xD9),
		entry_none(0xDA),
		entry_none(0xDB),
		entry_none(0xDC),
		entry_none(0xDD),
		entry_none(0xDE),
		entry_none(0xDF),

		entry_1_no_modrm(0xE0, ops::LOOPNZ, OpKind::Rel8Offset),
		entry_1_no_modrm(0xE1, ops::LOOPZ,  OpKind::Rel8Offset),
		entry_1_no_modrm(0xE2, ops::LOOP,   OpKind::Rel8Offset),
		entry_1_no_modrm(0xE3, ops::JECXZ,  OpKind::Rel8Offset),
		entry_2_no_modrm(0xE4, ops::IN,     OpKind::ImplicitAL, OpKind::Imm8),
		entry_2_no_modrm(0xE5, ops::IN,     OpKind::ImplicitEAX, OpKind::Imm8),
		entry_2_no_modrm(0xE6, ops::OUT,    OpKind::Imm8, OpKind::ImplicitAL),
		entry_2_no_modrm(0xE7, ops::OUT,    OpKind::Imm8, OpKind::ImplicitEAX),

		entry_1_no_modrm(0xE8, ops::CALL, OpKind::Rel32Offset),
		entry_1_no_modrm(0xE9, ops::JMP,  OpKind::Rel32Offset),
		entry_none(0xEA),                                                   // apparently invalid
		entry_1_no_modrm(0xEB, ops::JMP, OpKind::Rel8Offset),
		entry_2_no_modrm(0xEC, ops::IN,  OpKind::ImplicitAL, OpKind::ImplicitDX),
		entry_2_no_modrm(0xED, ops::IN,  OpKind::ImplicitEAX, OpKind::ImplicitDX),
		entry_2_no_modrm(0xEE, ops::OUT, OpKind::ImplicitDX, OpKind::ImplicitAL),
		entry_2_no_modrm(0xEF, ops::OUT, OpKind::ImplicitDX, OpKind::ImplicitEAX),

		entry_none(0xF0),                                                   // lock prefix
		entry_0(0xF1, ops::ICEBP),
		entry_none(0xF2),                                                   // repne prefix
		entry_none(0xF3),                                                   // rep prefix
		entry_0(0xF4, ops::HLT),
		entry_0(0xF5, ops::CMC),
		entry_ext(0xF6, &ModRMExt_F6[0]),                                   // F6 and F7 are group 3 extensions
		entry_ext(0xF7, &ModRMExt_F7[0]),

		entry_0(0xF8, ops::CLC),
		entry_0(0xF9, ops::STC),
		entry_0(0xFA, ops::CLI),
		entry_0(0xFB, ops::STI),
		entry_0(0xFC, ops::CLD),
		entry_0(0xFD, ops::STD),
		entry_ext(0xFE, &ModRMExt_FE[0]),                                   // group 4 extensions
		entry_ext(0xFF, &ModRMExt_FF[0]),                                   // group 5 extensions
	};

	static_assert(ArrayLength(PrimaryOpcodeMap) == 256, "table invalid");
	static_assert(ArrayLength(ModRMExt_80) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_81) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_82) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_83) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_8F) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_C0) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_C1) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_C6) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_C7) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_D0) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_D1) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_D2) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_D3) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_F6) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_F7) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_FE) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_FF) == 8, "table invalid");



	// group 6
	constexpr TableEntry ModRMExt_0F_00[] = {
		entry_1(0x00, ops::SLDT, OpKind::RegMem32),                         // it's not really r/m32, but... it works.
		entry_1(0x00, ops::STR,  OpKind::RegMem32),                         // it's not really r/m32, but... it works.
		entry_1(0x00, ops::LLDT, OpKind::RegMem16),
		entry_1(0x00, ops::LTR,  OpKind::RegMem16),
		entry_1(0x00, ops::VERR, OpKind::RegMem16),
		entry_1(0x00, ops::VERW, OpKind::RegMem16),

		entry_blank,
		entry_blank,
	};

	// group 7. these fuckers additionally need ModRM.RM bits to differentiate, so make a nested table.
	// each entry here is offset by 1; the extensions are only used when modRM.mod == 3; if it is not, then
	// the first entry is the real instruction. if it == 3, then use the following instructions, but add 1 to
	// modRM.rm to get which instruction to use.
	constexpr TableEntry ModRMExt_0F_01_Mod3_Reg1_RM[] = {
		entry_1(0x01, ops::SIDT, OpKind::Memory),                           // it's actually mem16:32/64, but... it works.

		entry_0_modrm(0x01, ops::MONITOR),                                  // modRM == 0xC8
		entry_0_modrm(0x01, ops::MWAIT),                                    // modRM == 0xC9

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};

	constexpr TableEntry ModRMExt_0F_01_Mod3_Reg2_RM[] = {
		entry_1(0x01, ops::LGDT, OpKind::Memory),                           // it's actually mem16:32/64, but... it works.

		entry_0_modrm(0x01, ops::XGETBV),                                   // modRM == 0xD0
		entry_0_modrm(0x01, ops::XSETBV),                                   // modRM == 0xD1

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};

	constexpr TableEntry ModRMExt_0F_01_Mod3_Reg3_RM[] = {
		entry_1(0x01, ops::LIDT, OpKind::Memory),                           // it's actually mem16:32/64, but... it works.

		entry_0_modrm(0x01, ops::VMRUN),                                    // modRM == 0xD8
		entry_0_modrm(0x01, ops::VMCALL),                                   // modRM == 0xD9
		entry_1(0x01, ops::VMLOAD, OpKind::ImplicitRAX),                    // modRM == 0xDA
		entry_1(0x01, ops::VMSAVE, OpKind::ImplicitRAX),                    // modRM == 0xDB
		entry_0_modrm(0x01, ops::STGI),                                     // modRM == 0xDC
		entry_0_modrm(0x01, ops::CLGI),                                     // modRM == 0xDD
		entry_1(0x01, ops::SKINIT, OpKind::ImplicitEAX),                    // modRM == 0xDE
		entry_2(0x01, ops::INVLPGA, OpKind::ImplicitRAX, OpKind::ImplicitECX), // modRM == 0xDF
	};

	constexpr TableEntry ModRMExt_0F_01_Mod3_Reg7_RM[] = {
		entry_1(0x01, ops::INVLPG, OpKind::Memory),

		entry_0_modrm(0x01, ops::SWAPGS),                                   // modRM == 0xF8
		entry_0_modrm(0x01, ops::RDTSCP),                                   // modRM == 0xF9
		entry_0_modrm(0x01, ops::MONITORX),                                 // modRM == 0xFA
		entry_0_modrm(0x01, ops::MWAITX),                                   // modRM == 0xFB

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};

	constexpr TableEntry ModRMExt_0F_01[] = {
		entry_1(0x01, ops::SGDT, OpKind::Memory),                           // it's actually mem16:32/64, but... it works.
		entry_ext_rm(0x01, &ModRMExt_0F_01_Mod3_Reg1_RM[0]),
		entry_ext_rm(0x01, &ModRMExt_0F_01_Mod3_Reg2_RM[0]),
		entry_ext_rm(0x01, &ModRMExt_0F_01_Mod3_Reg3_RM[0]),
		entry_1(0x01, ops::SMSW, OpKind::RegMem32),                         // not really...
		entry_none(0x01),
		entry_1(0x01, ops::LMSW, OpKind::RegMem16),
		entry_ext_rm(0x1, &ModRMExt_0F_01_Mod3_Reg7_RM[0]),
	};

	// group 16
	constexpr TableEntry ModRMExt_0F_18[] = {
		entry_1(0x18, ops::PREFETCH, OpKind::Memory),
		entry_1(0x18, ops::PREFETCH, OpKind::Memory),
		entry_1(0x18, ops::PREFETCH, OpKind::Memory),
		entry_1(0x18, ops::PREFETCH, OpKind::Memory),
		entry_0_modrm(0x18, ops::NOP),
		entry_0_modrm(0x18, ops::NOP),
		entry_0_modrm(0x18, ops::NOP),
		entry_0_modrm(0x18, ops::NOP),
	};

	// group 15. same shit here; we have two different instructions depending on whether
	// modRM.mod == 3 or modRM.mod != 3. we follow the same structure as the nested tables above.
	// since there's no differentiation based on modRM.rm, the first entry is modRM.mod != 3
	// and the second entry is modRM.mod == 3.
	constexpr TableEntry ModRMExt_0F_AE_Mod3_Reg4_RM[] = {
		entry_1(0xAE, ops::XSAVE, OpKind::Memory),                          // modRM.mod != 3
		entry_none(0xAE),                                                   // modRM.mod == 3
	};

	constexpr TableEntry ModRMExt_0F_AE_Mod3_Reg5_RM[] = {
		entry_1(0xAE, ops::XRSTOR, OpKind::Memory),                         // modRM.mod != 3
		entry_0_modrm(0xAE, ops::LFENCE),                                   // modRM.mod == 3
	};

	constexpr TableEntry ModRMExt_0F_AE_Mod3_Reg6_RM[] = {
		entry_1(0xAE, ops::XSAVEOPT, OpKind::Memory),                       // modRM.mod != 3
		entry_0_modrm(0xAE, ops::MFENCE),                                   // modRM.mod == 3
	};

	constexpr TableEntry ModRMExt_0F_AE_Mod3_Reg7_RM[] = {
		entry_1(0xAE, ops::CLFLUSH, OpKind::Memory),                        // modRM.mod != 3
		entry_0_modrm(0xAE, ops::SFENCE),                                   // modRM.mod == 3
	};

	// only used when opcode == 0x0F 0xAE, and has no prefix
	constexpr TableEntry ModRMExt_0F_AE_Prefix_None[] = {
		entry_1(0xAE, ops::FXSAVE,  OpKind::Memory),
		entry_1(0xAE, ops::FXRSTOR, OpKind::Memory),
		entry_1(0xAE, ops::LDMXCSR, OpKind::Memory),
		entry_1(0xAE, ops::STMXCSR, OpKind::Memory),
		entry_ext_mod(0xAE, &ModRMExt_0F_AE_Mod3_Reg4_RM[0]),
		entry_ext_mod(0xAE, &ModRMExt_0F_AE_Mod3_Reg5_RM[0]),
		entry_ext_mod(0xAE, &ModRMExt_0F_AE_Mod3_Reg6_RM[0]),
		entry_ext_mod(0xAE, &ModRMExt_0F_AE_Mod3_Reg7_RM[0]),
	};

	// only used when opcode == 0x0F 0xAE, and has prefix 0xF3
	constexpr TableEntry ModRMExt_0F_AE_Prefix_F3[] = {
		entry_1(0xAE, ops::RDFSBASE, OpKind::Reg32),
		entry_1(0xAE, ops::RDGSBASE, OpKind::Reg32),
		entry_1(0xAE, ops::WRFSBASE, OpKind::Reg32),
		entry_1(0xAE, ops::WRGSBASE, OpKind::Reg32),

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};


	// this is a bit annoying. because there are really *5* kinds of entries in the 0xF map;
	// there's the "normal" one, which is just an opcode, then there's 4 fucked up ones:
	// (a) no prefix; (b) 0x66 prefix; (c) 0xF2 prefix; (d) 0xF3 prefix. to handle this, we
	// make 5 separate tables. those that are invalid in that table will get filled with entry_none.
	constexpr TableEntry SecondaryOpcodeMap_F0_Normal[] = {
		entry_ext(0x00, &ModRMExt_0F_00[0]),
		entry_ext(0x01, &ModRMExt_0F_01[0]),
		entry_2(0x02, ops::LAR, OpKind::Reg32, OpKind::RegMem16),
		entry_2(0x03, ops::LSL, OpKind::Reg32, OpKind::RegMem16),
		entry_none(0x04),
		entry_0(0x05, ops::SYSCALL),
		entry_0(0x06, ops::CLTS),
		entry_0(0x07, ops::SYSRET),

		entry_0(0x08, ops::INVD),
		entry_0(0x09, ops::WBINVD),
		entry_none(0x0A),
		entry_0(0x0B, ops::UD2),
		entry_none(0x0C),
		entry_1(0x0D, ops::PREFETCH, OpKind::Memory),
		entry_0(0x0E, ops::FEMMS),
		entry_none(0x0F),                                                   // 3dnow escape

		// opcodes 0x10 to 0x17 use prefixes
		entry_none(0x10),
		entry_none(0x11),
		entry_none(0x12),
		entry_none(0x13),
		entry_none(0x14),
		entry_none(0x15),
		entry_none(0x16),
		entry_none(0x17),

		entry_ext(0x18, &ModRMExt_0F_18[0]),
		entry_1(0x19, ops::NOP, OpKind::RegMem32),
		entry_1(0x1A, ops::NOP, OpKind::RegMem32),
		entry_1(0x1B, ops::NOP, OpKind::RegMem32),
		entry_1(0x1C, ops::NOP, OpKind::RegMem32),
		entry_1(0x1D, ops::NOP, OpKind::RegMem32),
		entry_1(0x1E, ops::NOP, OpKind::RegMem32),
		entry_1(0x1F, ops::NOP, OpKind::RegMem32),

		// the non-special register is encoded in the rm bits, so use RegMem64 even though it's not really mem.
		entry_2(0x20, ops::MOV, OpKind::RegMem64, OpKind::ControlReg),
		entry_2(0x21, ops::MOV, OpKind::RegMem64, OpKind::DebugReg),
		entry_2(0x22, ops::MOV, OpKind::ControlReg, OpKind::RegMem64),
		entry_2(0x23, ops::MOV, OpKind::DebugReg, OpKind::RegMem64),
		entry_none(0x24),
		entry_none(0x25),
		entry_none(0x26),
		entry_none(0x27),

		// opcodes 0x28 to 0x2F use prefixes
		entry_none(0x28),
		entry_none(0x29),
		entry_none(0x2A),
		entry_none(0x2B),
		entry_none(0x2C),
		entry_none(0x2D),
		entry_none(0x2E),
		entry_none(0x2F),

		entry_0(0x30, ops::WRMSR),
		entry_0(0x31, ops::RDTSC),
		entry_0(0x32, ops::RDMSR),
		entry_0(0x33, ops::RDPMC),
		entry_0(0x34, ops::SYSENTER),
		entry_0(0x35, ops::SYSEXIT),
		entry_none(0x36),
		entry_none(0x37),

		entry_none(0x38),                                                   // escape to 0x0F_38
		entry_none(0x39),
		entry_none(0x3A),                                                   // escape to 0x0F_3A
		entry_none(0x3B),
		entry_none(0x3C),
		entry_none(0x3D),
		entry_none(0x3E),
		entry_none(0x3F),

		entry_2(0x40, ops::CMOVO,  OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x41, ops::CMOVNO, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x42, ops::CMOVB,  OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x43, ops::CMOVNB, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x44, ops::CMOVZ,  OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x45, ops::CMOVNZ, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x46, ops::CMOVNA, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x47, ops::CMOVA,  OpKind::Reg32, OpKind::RegMem32),

		entry_2(0x48, ops::CMOVS,  OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x49, ops::CMOVNS, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x4A, ops::CMOVP,  OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x4B, ops::CMOVNP, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x4C, ops::CMOVL,  OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x4D, ops::CMOVGE, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x4E, ops::CMOVLE, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0x4F, ops::CMOVG,  OpKind::Reg32, OpKind::RegMem32),

		// opcodes 0x50 to 0x7F use prefixes
		entry_none(0x50), entry_none(0x51), entry_none(0x52), entry_none(0x53),
		entry_none(0x54), entry_none(0x55), entry_none(0x56), entry_none(0x57),
		entry_none(0x58), entry_none(0x59), entry_none(0x5A), entry_none(0x5B),
		entry_none(0x5C), entry_none(0x5D), entry_none(0x5E), entry_none(0x5F),
		entry_none(0x60), entry_none(0x61), entry_none(0x62), entry_none(0x63),
		entry_none(0x64), entry_none(0x65), entry_none(0x66), entry_none(0x67),
		entry_none(0x68), entry_none(0x69), entry_none(0x6A), entry_none(0x6B),
		entry_none(0x6C), entry_none(0x6D), entry_none(0x6E), entry_none(0x6F),
		entry_none(0x70), entry_none(0x71), entry_none(0x72), entry_none(0x73),
		entry_none(0x74), entry_none(0x75), entry_none(0x76), entry_none(0x77),
		entry_none(0x78), entry_none(0x79), entry_none(0x7A), entry_none(0x7B),
		entry_none(0x7C), entry_none(0x7D), entry_none(0x7E), entry_none(0x7F),

		// these are the same as the one-byte opcode versions of Jcc, but they take Rel16/32Offset.
		entry_1_no_modrm(0x80, ops::JO,  OpKind::Rel32Offset),
		entry_1_no_modrm(0x81, ops::JNO, OpKind::Rel32Offset),
		entry_1_no_modrm(0x82, ops::JB,  OpKind::Rel32Offset),
		entry_1_no_modrm(0x83, ops::JNB, OpKind::Rel32Offset),
		entry_1_no_modrm(0x84, ops::JZ,  OpKind::Rel32Offset),
		entry_1_no_modrm(0x85, ops::JNZ, OpKind::Rel32Offset),
		entry_1_no_modrm(0x86, ops::JNA, OpKind::Rel32Offset),
		entry_1_no_modrm(0x87, ops::JA,  OpKind::Rel32Offset),

		entry_1_no_modrm(0x88, ops::JS,  OpKind::Rel32Offset),
		entry_1_no_modrm(0x89, ops::JNS, OpKind::Rel32Offset),
		entry_1_no_modrm(0x8A, ops::JP,  OpKind::Rel32Offset),
		entry_1_no_modrm(0x8B, ops::JNP, OpKind::Rel32Offset),
		entry_1_no_modrm(0x8C, ops::JL,  OpKind::Rel32Offset),
		entry_1_no_modrm(0x8D, ops::JGE, OpKind::Rel32Offset),
		entry_1_no_modrm(0x8E, ops::JLE, OpKind::Rel32Offset),
		entry_1_no_modrm(0x8F, ops::JG,  OpKind::Rel32Offset),

		// same order as Jcc, just Setcc.
		entry_1(0x90, ops::SETO,  OpKind::RegMem8),
		entry_1(0x91, ops::SETNO, OpKind::RegMem8),
		entry_1(0x92, ops::SETB,  OpKind::RegMem8),
		entry_1(0x93, ops::SETNB, OpKind::RegMem8),
		entry_1(0x94, ops::SETZ,  OpKind::RegMem8),
		entry_1(0x95, ops::SETNZ, OpKind::RegMem8),
		entry_1(0x96, ops::SETNA, OpKind::RegMem8),
		entry_1(0x97, ops::SETA,  OpKind::RegMem8),

		entry_1(0x98, ops::SETS,  OpKind::RegMem8),
		entry_1(0x99, ops::SETNS, OpKind::RegMem8),
		entry_1(0x9A, ops::SETP,  OpKind::RegMem8),
		entry_1(0x9B, ops::SETNP, OpKind::RegMem8),
		entry_1(0x9C, ops::SETL,  OpKind::RegMem8),
		entry_1(0x9D, ops::SETGE, OpKind::RegMem8),
		entry_1(0x9E, ops::SETLE, OpKind::RegMem8),
		entry_1(0x9F, ops::SETG,  OpKind::RegMem8),

		entry_1_no_modrm(0xA0, ops::PUSH, OpKind::ImplicitFS),
		entry_1_no_modrm(0xA1, ops::POP,  OpKind::ImplicitFS),
		entry_0(0xA2, ops::CPUID),
		entry_2(0xA3, ops::BT,   OpKind::RegMem32, OpKind::Reg32),
		entry_3(0xA4, ops::SHLD, OpKind::RegMem32, OpKind::Reg32, OpKind::Imm8),
		entry_3(0xA5, ops::SHLD, OpKind::RegMem32, OpKind::Reg32, OpKind::ImplicitCL),
		entry_none(0xA6),
		entry_none(0xA7),

		entry_1_no_modrm(0xA8, ops::PUSH, OpKind::ImplicitGS),
		entry_1_no_modrm(0xA9, ops::POP,  OpKind::ImplicitGS),
		entry_0(0xAA, ops::RSM),
		entry_2(0xAB, ops::BTS,  OpKind::RegMem32, OpKind::Reg32),
		entry_3(0xAC, ops::SHLD, OpKind::RegMem32, OpKind::Reg32, OpKind::Imm8),
		entry_3(0xAD, ops::SHLD, OpKind::RegMem32, OpKind::Reg32, OpKind::ImplicitCL),
		entry_none(0xAE),
		entry_2(0xAF, ops::IMUL, OpKind::Reg32, OpKind::RegMem32),

		entry_2(0xB0, ops::CMPXCHG, OpKind::RegMem8, OpKind::Reg8),
		entry_2(0xB1, ops::CMPXCHG, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0xB2, ops::LSS,     OpKind::Reg32, OpKind::Ptr16_32),
		entry_2(0xB3, ops::BTR,     OpKind::RegMem32, OpKind::Reg32),
		entry_2(0xB4, ops::LFS,     OpKind::Reg32, OpKind::Ptr16_32),
		entry_2(0xB5, ops::LGS,     OpKind::Reg32, OpKind::Ptr16_32),
		entry_2(0xB6, ops::MOVZX,   OpKind::Reg32, OpKind::RegMem8),
		entry_2(0xB7, ops::MOVZX,   OpKind::Reg32, OpKind::RegMem16),

		// opcodes 0xB8 to 0xC7 use prefixes
		entry_none(0xB8), entry_none(0xB9), entry_none(0xBA), entry_none(0xBB),
		entry_none(0xBC), entry_none(0xBD), entry_none(0xBE), entry_none(0xBF),
		entry_none(0xC0), entry_none(0xC1), entry_none(0xC2), entry_none(0xC3),
		entry_none(0xC4), entry_none(0xC5), entry_none(0xC6), entry_none(0xC7),

		entry_lnri_1(0xC8, ops::BSWAP, OpKind::RegMem32),
		entry_lnri_1(0xC9, ops::BSWAP, OpKind::RegMem32),
		entry_lnri_1(0xCA, ops::BSWAP, OpKind::RegMem32),
		entry_lnri_1(0xCB, ops::BSWAP, OpKind::RegMem32),
		entry_lnri_1(0xCC, ops::BSWAP, OpKind::RegMem32),
		entry_lnri_1(0xCD, ops::BSWAP, OpKind::RegMem32),
		entry_lnri_1(0xCE, ops::BSWAP, OpKind::RegMem32),
		entry_lnri_1(0xCF, ops::BSWAP, OpKind::RegMem32),

		// opcodes 0xD0 to 0xFF use prefixes.
		entry_none(0xD0), entry_none(0xD1), entry_none(0xD2), entry_none(0xD3),
		entry_none(0xD4), entry_none(0xD5), entry_none(0xD6), entry_none(0xD7),
		entry_none(0xD8), entry_none(0xD9), entry_none(0xDA), entry_none(0xDB),
		entry_none(0xDC), entry_none(0xDD), entry_none(0xDE), entry_none(0xDF),
		entry_none(0xE0), entry_none(0xE1), entry_none(0xE2), entry_none(0xE3),
		entry_none(0xE4), entry_none(0xE5), entry_none(0xE6), entry_none(0xE7),
		entry_none(0xE8), entry_none(0xE9), entry_none(0xEA), entry_none(0xEB),
		entry_none(0xEC), entry_none(0xED), entry_none(0xEE), entry_none(0xEF),
		entry_none(0xF0), entry_none(0xF1), entry_none(0xF2), entry_none(0xF3),
		entry_none(0xF4), entry_none(0xF5), entry_none(0xF6), entry_none(0xF7),
		entry_none(0xF8), entry_none(0xF9), entry_none(0xFA), entry_none(0xFB),
		entry_none(0xFC), entry_none(0xFD), entry_none(0xFE), entry_none(0xFF),
	};

	// TODO: tables need re-doing for MMX; apparently, some of them fucking encode the
	// only register operand (eg. 'PSLLW mm0, 0xFF') in modRM.rm, instead of modRM.reg.
	// JESUS.

	// the stupid thing about these dumbass SSE opcodes is that we need to differentiate
	// the opcodes based on their operand type. for example. 0x12: MOVLPS xmm, mem64
	// is also 0x12 MOVHLPS xmm, xmm; they do quite different things, but have the same
	// opcode. how we handle this is, again, to use extensions. the saving grace is
	// that the non-memory version (MOVHLPS) will have modrm.mod == 3, so we can use
	// our existing mechanisms to disambiguate them.
	// we set the first entry as the memory-operand-type (modrm.mod != 3), and
	// the second entry as the register-operand-type (modrm.mod == 3).
	constexpr TableEntry ModRMExt_0F_PrefixNone_12_Mod3[] = {
		entry_2(0x12, ops::MOVLPS,  OpKind::XmmReg, OpKind::Mem64),         // modRM.mod != 3
		entry_2(0x12, ops::MOVHLPS, OpKind::XmmReg, OpKind::XmmReg),        // modRM.mod == 3
	};

	constexpr TableEntry ModRMExt_0F_PrefixNone_16_Mod3[] = {
		entry_2(0x16, ops::MOVHPS,  OpKind::XmmReg, OpKind::Mem64),         // modRM.mod != 3
		entry_2(0x16, ops::MOVLHPS, OpKind::XmmReg, OpKind::XmmReg),        // modRM.mod == 3
	};

	// "normal" extensions
	constexpr TableEntry ModRMExt_0F_PrefixAny_C7[] = {
		entry_blank,
		// cmpxchg8b or cmpxchg16b; need to differentiate with REX.W
		entry_none(0xC7),
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_1(0xC7, ops::RDRAND, OpKind::Reg32),
		entry_1(0xC7, ops::RDSEED, OpKind::Reg32),
	};

	constexpr TableEntry ModRMExt_0F_PrefixNone_71[] = {
		entry_blank,
		entry_blank,
		entry_2(0x71, ops::PSRLW, OpKind::MmxReg, OpKind::Imm8),
		entry_blank,
		entry_2(0x71, ops::PSRAW, OpKind::MmxReg, OpKind::Imm8),
		entry_blank,
		entry_2(0x71, ops::PSLLW, OpKind::MmxReg, OpKind::Imm8),
		entry_blank,
	};

	constexpr TableEntry ModRMExt_0F_PrefixNone_72[] = {
		entry_blank,
		entry_blank,
		entry_2(0x72, ops::PSRLD, OpKind::MmxReg, OpKind::Imm8),
		entry_blank,
		entry_2(0x72, ops::PSRAD, OpKind::MmxReg, OpKind::Imm8),
		entry_blank,
		entry_2(0x72, ops::PSLLD, OpKind::MmxReg, OpKind::Imm8),
		entry_blank,
	};

	constexpr TableEntry ModRMExt_0F_PrefixNone_73[] = {
		entry_blank,
		entry_blank,
		entry_2(0x73, ops::PSRLQ, OpKind::MmxReg, OpKind::Imm8),
		entry_blank,
		entry_blank,
		entry_blank,
		entry_2(0x73, ops::PSLLQ, OpKind::MmxReg, OpKind::Imm8),
		entry_blank,
	};

	constexpr TableEntry ModRMExt_0F_PrefixNone_B9[] = {
		entry_0(0xB9, ops::UD1),
		entry_0(0xB9, ops::UD1),
		entry_0(0xB9, ops::UD1),
		entry_0(0xB9, ops::UD1),
		entry_0(0xB9, ops::UD1),
		entry_0(0xB9, ops::UD1),
		entry_0(0xB9, ops::UD1),
		entry_0(0xB9, ops::UD1),
	};

	constexpr TableEntry ModRMExt_0F_PrefixNone_BA[] = {
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_2(0xBA, ops::BT,  OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xBA, ops::BTS, OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xBA, ops::BTR, OpKind::RegMem32, OpKind::Imm8),
		entry_2(0xBA, ops::BTC, OpKind::RegMem32, OpKind::Imm8),
	};

	static_assert(ArrayLength(ModRMExt_0F_PrefixNone_12_Mod3) == 2, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_PrefixNone_16_Mod3) == 2, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_PrefixAny_C7) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_PrefixNone_BA) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_PrefixNone_B9) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_PrefixNone_71) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_PrefixNone_72) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_PrefixNone_73) == 8, "table invalid");


	constexpr TableEntry SecondaryOpcodeMap_F0_Prefix_None[] = {
		// opcodes 0x00 to 0x0F are not prefixed
		entry_none(0x00), entry_none(0x01), entry_none(0x02), entry_none(0x03),
		entry_none(0x04), entry_none(0x05), entry_none(0x06), entry_none(0x07),
		entry_none(0x08), entry_none(0x09), entry_none(0x0A), entry_none(0x0B),
		entry_none(0x0C), entry_none(0x0D), entry_none(0x0E), entry_none(0x0F),

		entry_2(0x10, ops::MOVUPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x11, ops::MOVUPS, OpKind::XmmRegMem128, OpKind::XmmReg),
		entry_ext_mod(0x12, &ModRMExt_0F_PrefixNone_12_Mod3[0]),
		entry_2(0x13, ops::MOVLPS, OpKind::Mem64, OpKind::XmmReg),
		entry_2(0x14, ops::UNPCKLPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x15, ops::UNPCKHPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_ext_mod(0x16, &ModRMExt_0F_PrefixNone_16_Mod3[0]),
		entry_2(0x17, ops::MOVHPS, OpKind::Mem64, OpKind::XmmReg),

		// opcodes 0x18 to 0x27 are not prefixed
		entry_none(0x18), entry_none(0x19), entry_none(0x1A), entry_none(0x1B),
		entry_none(0x1C), entry_none(0x1D), entry_none(0x1E), entry_none(0x1F),
		entry_none(0x20), entry_none(0x21), entry_none(0x22), entry_none(0x23),
		entry_none(0x24), entry_none(0x25), entry_none(0x26), entry_none(0x27),

		entry_2(0x28, ops::MOVAPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x29, ops::MOVAPS, OpKind::XmmRegMem128, OpKind::XmmReg),
		entry_2(0x2A, ops::CVTPI2PS, OpKind::XmmReg, OpKind::MmxRegMem64),
		entry_2(0x2B, ops::MOVNTPS, OpKind::Mem128, OpKind::XmmReg),
		entry_2(0x2C, ops::CVTTPS2PI, OpKind::MmxReg, OpKind::XmmRegMem64),
		entry_2(0x2D, ops::CVTPS2PI, OpKind::MmxReg, OpKind::XmmRegMem64),
		entry_2(0x2E, ops::UCOMISS, OpKind::XmmReg, OpKind::XmmRegMem32),
		entry_2(0x2F, ops::COMISS, OpKind::XmmReg, OpKind::XmmRegMem32),

		// opcodes 0x30 to 0x4F are not prefixed
		entry_none(0x30), entry_none(0x31), entry_none(0x32), entry_none(0x33),
		entry_none(0x34), entry_none(0x35), entry_none(0x36), entry_none(0x37),
		entry_none(0x38), entry_none(0x39), entry_none(0x3A), entry_none(0x3B),
		entry_none(0x3C), entry_none(0x3D), entry_none(0x3E), entry_none(0x3F),
		entry_none(0x40), entry_none(0x41), entry_none(0x42), entry_none(0x43),
		entry_none(0x44), entry_none(0x45), entry_none(0x46), entry_none(0x47),
		entry_none(0x48), entry_none(0x49), entry_none(0x4A), entry_none(0x4B),
		entry_none(0x4C), entry_none(0x4D), entry_none(0x4E), entry_none(0x4F),

		entry_2(0x50, ops::MOVMSKPS, OpKind::Reg32, OpKind::XmmReg),
		entry_2(0x51, ops::SQRTPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x52, ops::RSQRTPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x53, ops::RCPPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x54, ops::ANDPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x55, ops::ANDNPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x56, ops::ORPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x57, ops::XORPS, OpKind::XmmReg, OpKind::XmmRegMem128),

		entry_2(0x58, ops::ADDPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x59, ops::MULPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x5A, ops::CVTPS2PD, OpKind::XmmReg, OpKind::XmmRegMem64),
		entry_2(0x5B, ops::CVTDQ2PS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x5C, ops::SUBPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x5D, ops::MINPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x5E, ops::DIVPS, OpKind::XmmReg, OpKind::XmmRegMem128),
		entry_2(0x5F, ops::MAXPS, OpKind::XmmReg, OpKind::XmmRegMem128),

		entry_2(0x60, ops::PUNPCKLBW, OpKind::MmxReg, OpKind::MmxRegMem32),
		entry_2(0x61, ops::PUNPCKLWD, OpKind::MmxReg, OpKind::MmxRegMem32),
		entry_2(0x62, ops::PUNPCKLDQ, OpKind::MmxReg, OpKind::MmxRegMem32),
		entry_2(0x63, ops::PACKSSWB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0x64, ops::PCMPGTB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0x65, ops::PCMPGTW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0x66, ops::PCMPGTD, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0x67, ops::PACKUSWB, OpKind::MmxReg, OpKind::MmxRegMem64),

		entry_2(0x68, ops::PUNPCKHBW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0x69, ops::PUNPCKHWD, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0x6A, ops::PUNPCKHDQ, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0x6B, ops::PACKSSDW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_none(0x6C),
		entry_none(0x6D),
		entry_2(0x6E, ops::MOVD, OpKind::MmxReg, OpKind::RegMem32),         // this will auto-promote to RegMem64 based on REX.W
		entry_2(0x6F, ops::MOVQ, OpKind::MmxReg, OpKind::MmxRegMem64),

		entry_3(0x70, ops::PSHUFW, OpKind::MmxReg, OpKind::MmxRegMem64, OpKind::Imm8),
		entry_ext(0x71, &ModRMExt_0F_PrefixNone_71[0]),
		entry_ext(0x72, &ModRMExt_0F_PrefixNone_72[0]),
		entry_ext(0x73, &ModRMExt_0F_PrefixNone_73[0]),
		entry_2(0x74, ops::PCMPEQB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0x75, ops::PCMPEQW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0x76, ops::PCMPEQD, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_0(0x77, ops::EMMS),

		entry_none(0x78),
		entry_none(0x79),
		entry_none(0x7A),
		entry_none(0x7B),
		entry_none(0x7C),
		entry_none(0x7D),
		entry_2(0x7E, ops::MOVD, OpKind::RegMem32, OpKind::MmxReg),         // this will auto-promote to RegMem64 based on REX.W
		entry_2(0x7F, ops::MOVQ, OpKind::MmxRegMem64, OpKind::MmxReg),

		// opcodes 0x80 to 0xAF are not prefixed
		entry_none(0x80), entry_none(0x81), entry_none(0x82), entry_none(0x83),
		entry_none(0x84), entry_none(0x85), entry_none(0x86), entry_none(0x87),
		entry_none(0x88), entry_none(0x89), entry_none(0x8A), entry_none(0x8B),
		entry_none(0x8C), entry_none(0x8D), entry_none(0x8E), entry_none(0x8F),
		entry_none(0x90), entry_none(0x91), entry_none(0x92), entry_none(0x93),
		entry_none(0x94), entry_none(0x95), entry_none(0x96), entry_none(0x97),
		entry_none(0x98), entry_none(0x99), entry_none(0x9A), entry_none(0x9B),
		entry_none(0x9C), entry_none(0x9D), entry_none(0x9E), entry_none(0x9F),
		entry_none(0xA0), entry_none(0xA1), entry_none(0xA2), entry_none(0xA3),
		entry_none(0xA4), entry_none(0xA5), entry_none(0xA6), entry_none(0xA7),
		entry_none(0xA8), entry_none(0xA9), entry_none(0xAA), entry_none(0xAB),
		entry_none(0xAC),
		entry_none(0xAD),
		entry_ext(0xAE, &ModRMExt_0F_AE_Prefix_None[0]),                    // group 15
		entry_none(0xAF),
		entry_none(0xB0), entry_none(0xB1), entry_none(0xB2), entry_none(0xB3),
		entry_none(0xB4), entry_none(0xB5), entry_none(0xB6), entry_none(0xB7),

		entry_none(0xB8),
		entry_ext(0xB9, &ModRMExt_0F_PrefixNone_B9[0]),
		entry_ext(0xBA, &ModRMExt_0F_PrefixNone_BA[0]),
		entry_2(0xBB, ops::BTC, OpKind::RegMem32, OpKind::Reg32),
		entry_2(0xBC, ops::BSF, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0xBD, ops::BSR, OpKind::Reg32, OpKind::RegMem32),
		entry_2(0xBE, ops::MOVSX, OpKind::Reg32, OpKind::RegMem8),
		entry_2(0xBF, ops::MOVSX, OpKind::Reg32, OpKind::RegMem16),

		entry_2(0xC0, ops::XADD, OpKind::RegMem8, OpKind::Reg8),
		entry_2(0xC1, ops::XADD, OpKind::RegMem32, OpKind::Reg32),
		entry_3(0xC2, ops::CMPPS, OpKind::XmmReg, OpKind::XmmRegMem128, OpKind::Imm8),
		entry_2(0xC3, ops::MOVNTI, OpKind::Mem32, OpKind::Reg32),
		entry_3(0xC4, ops::PINSRW, OpKind::MmxReg, OpKind::Reg32Mem16, OpKind::Imm8),
		entry_3(0xC5, ops::PEXTRW, OpKind::Reg32, OpKind::MmxReg, OpKind::Imm8),
		entry_3(0xC6, ops::SHUFPS, OpKind::XmmReg, OpKind::XmmRegMem128, OpKind::Imm8),
		entry_ext(0xC7, &ModRMExt_0F_PrefixAny_C7[0]),

		// opcodes 0xC8 to 0xCF are not prefixed
		entry_none(0xC8), entry_none(0xC9), entry_none(0xCA), entry_none(0xCB),
		entry_none(0xCC), entry_none(0xCD), entry_none(0xCE), entry_none(0xCF),

		entry_none(0xD0),
		entry_2(0xD1, ops::PSRLW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xD2, ops::PSRLD, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xD3, ops::PSRLQ, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xD4, ops::PADDQ, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xD5, ops::PMULLW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_none(0xD6),
		entry_2(0xD7, ops::PMOVMSKB, OpKind::Reg32, OpKind::MmxReg),

		entry_2(0xD8, ops::PSUBUSB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xD9, ops::PSUBUSW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xDA, ops::PMINUB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xDB, ops::PAND, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xDC, ops::PADDUSB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xDD, ops::PADDUSW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xDE, ops::PMAXUB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xDF, ops::PANDN, OpKind::MmxReg, OpKind::MmxRegMem64),

		entry_2(0xE0, ops::PAVGB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xE1, ops::PSRAW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xE2, ops::PSRAD, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xE3, ops::PAVGW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xE4, ops::PMULHUW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xE5, ops::PMULHW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_none(0xE6),
		entry_2(0xE7, ops::MOVNTQ, OpKind::Mem64, OpKind::MmxReg),

		entry_2(0xE8, ops::PSUBSB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xE9, ops::PSUBSW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xEA, ops::PMINSW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xEB, ops::POR, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xEC, ops::PADDSB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xED, ops::PADDSW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xEE, ops::PMAXSW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xEF, ops::PXOR, OpKind::MmxReg, OpKind::MmxRegMem64),

		entry_none(0xF0),
		entry_2(0xF1, ops::PSLLW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xF2, ops::PSLLD, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xF3, ops::PSLLQ, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xF4, ops::PMULUDQ, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xF5, ops::PMADDWD, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xF6, ops::PSADBW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xF7, ops::MASKMOVQ, OpKind::MmxReg, OpKind::MmxReg),

		entry_2(0xF8, ops::PSUBB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xF9, ops::PSUBW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xFA, ops::PSUBD, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xFB, ops::PSUBQ, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xFC, ops::PADDB, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xFD, ops::PADDW, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_2(0xFE, ops::PADDD, OpKind::MmxReg, OpKind::MmxRegMem64),
		entry_0(0xFF, ops::UD0),
	};



	constexpr TableEntry SecondaryOpcodeMap_F0_Prefix_66[] = {
		// opcodes 0x00 to 0x0F are not prefixed
		entry_none(0x00), entry_none(0x01), entry_none(0x02), entry_none(0x03),
		entry_none(0x04), entry_none(0x05), entry_none(0x06), entry_none(0x07),
		entry_none(0x08), entry_none(0x09), entry_none(0x0A), entry_none(0x0B),
		entry_none(0x0C), entry_none(0x0D), entry_none(0x0E), entry_none(0x0F),

		entry_none(0x10),
		entry_none(0x11),
		entry_none(0x12),
		entry_none(0x13),
		entry_none(0x14),
		entry_none(0x15),
		entry_none(0x16),
		entry_none(0x17),

		// opcodes 0x18 to 0x27 are not prefixed
		entry_none(0x18), entry_none(0x19), entry_none(0x1A), entry_none(0x1B),
		entry_none(0x1C), entry_none(0x1D), entry_none(0x1E), entry_none(0x1F),
		entry_none(0x20), entry_none(0x21), entry_none(0x22), entry_none(0x23),
		entry_none(0x24), entry_none(0x25), entry_none(0x26), entry_none(0x27),

		entry_none(0x28),
		entry_none(0x29),
		entry_none(0x2A),
		entry_none(0x2B),
		entry_none(0x2C),
		entry_none(0x2D),
		entry_none(0x2E),
		entry_none(0x2F),

		// opcodes 0x30 to 0x4F are not prefixed
		entry_none(0x30), entry_none(0x31), entry_none(0x32), entry_none(0x33),
		entry_none(0x34), entry_none(0x35), entry_none(0x36), entry_none(0x37),
		entry_none(0x38), entry_none(0x39), entry_none(0x3A), entry_none(0x3B),
		entry_none(0x3C), entry_none(0x3D), entry_none(0x3E), entry_none(0x3F),
		entry_none(0x40), entry_none(0x41), entry_none(0x42), entry_none(0x43),
		entry_none(0x44), entry_none(0x45), entry_none(0x46), entry_none(0x47),
		entry_none(0x48), entry_none(0x49), entry_none(0x4A), entry_none(0x4B),
		entry_none(0x4C), entry_none(0x4D), entry_none(0x4E), entry_none(0x4F),

		entry_none(0x50),
		entry_none(0x51),
		entry_none(0x52),
		entry_none(0x53),
		entry_none(0x54),
		entry_none(0x55),
		entry_none(0x56),
		entry_none(0x57),

		entry_none(0x58),
		entry_none(0x59),
		entry_none(0x5A),
		entry_none(0x5B),
		entry_none(0x5C),
		entry_none(0x5D),
		entry_none(0x5E),
		entry_none(0x5F),

		entry_none(0x60),
		entry_none(0x61),
		entry_none(0x62),
		entry_none(0x63),
		entry_none(0x64),
		entry_none(0x65),
		entry_none(0x66),
		entry_none(0x67),

		entry_none(0x68),
		entry_none(0x69),
		entry_none(0x6A),
		entry_none(0x6B),
		entry_none(0x6C),
		entry_none(0x6D),
		entry_none(0x6E),
		entry_none(0x6F),

		entry_none(0x70),
		entry_none(0x71),
		entry_none(0x72),
		entry_none(0x73),
		entry_none(0x74),
		entry_none(0x75),
		entry_none(0x76),
		entry_none(0x77),

		entry_none(0x78),
		entry_none(0x79),
		entry_none(0x7A),
		entry_none(0x7B),
		entry_none(0x7C),
		entry_none(0x7D),
		entry_none(0x7E),
		entry_none(0x7F),

		// opcodes 0x80 to 0xAF are not prefixed
		entry_none(0x80), entry_none(0x81), entry_none(0x82), entry_none(0x83),
		entry_none(0x84), entry_none(0x85), entry_none(0x86), entry_none(0x87),
		entry_none(0x88), entry_none(0x89), entry_none(0x8A), entry_none(0x8B),
		entry_none(0x8C), entry_none(0x8D), entry_none(0x8E), entry_none(0x8F),
		entry_none(0x90), entry_none(0x91), entry_none(0x92), entry_none(0x93),
		entry_none(0x94), entry_none(0x95), entry_none(0x96), entry_none(0x97),
		entry_none(0x98), entry_none(0x99), entry_none(0x9A), entry_none(0x9B),
		entry_none(0x9C), entry_none(0x9D), entry_none(0x9E), entry_none(0x9F),
		entry_none(0xA0), entry_none(0xA1), entry_none(0xA2), entry_none(0xA3),
		entry_none(0xA4), entry_none(0xA5), entry_none(0xA6), entry_none(0xA7),
		entry_none(0xA8), entry_none(0xA9), entry_none(0xAA), entry_none(0xAB),
		entry_none(0xAC), entry_none(0xAD), entry_none(0xAE), entry_none(0xAF),

		// opcodes 0xB0 to 0xB7 are not prefixed
		entry_none(0xB0), entry_none(0xB1), entry_none(0xB2), entry_none(0xB3),
		entry_none(0xB4), entry_none(0xB5), entry_none(0xB6), entry_none(0xB7),

		entry_none(0xB8),
		entry_none(0xB9),
		entry_none(0xBA),
		entry_none(0xBB),
		entry_none(0xBC),
		entry_none(0xBD),
		entry_none(0xBE),
		entry_none(0xBF),

		entry_none(0xC0),
		entry_none(0xC1),
		entry_none(0xC2),
		entry_none(0xC3),
		entry_none(0xC4),
		entry_none(0xC5),
		entry_none(0xC6),
		entry_none(0xC7),

		// opcodes 0xC8 to 0xCF are not prefixed
		entry_none(0xC8), entry_none(0xC9), entry_none(0xCA), entry_none(0xCB),
		entry_none(0xCC), entry_none(0xCD), entry_none(0xCE), entry_none(0xCF),

		entry_none(0xD0),
		entry_none(0xD1),
		entry_none(0xD2),
		entry_none(0xD3),
		entry_none(0xD4),
		entry_none(0xD5),
		entry_none(0xD6),
		entry_none(0xD7),

		entry_none(0xD8),
		entry_none(0xD9),
		entry_none(0xDA),
		entry_none(0xDB),
		entry_none(0xDC),
		entry_none(0xDD),
		entry_none(0xDE),
		entry_none(0xDF),

		entry_none(0xE0),
		entry_none(0xE1),
		entry_none(0xE2),
		entry_none(0xE3),
		entry_none(0xE4),
		entry_none(0xE5),
		entry_none(0xE6),
		entry_none(0xE7),

		entry_none(0xE8),
		entry_none(0xE9),
		entry_none(0xEA),
		entry_none(0xEB),
		entry_none(0xEC),
		entry_none(0xED),
		entry_none(0xEE),
		entry_none(0xEF),

		entry_none(0xF0),
		entry_none(0xF1),
		entry_none(0xF2),
		entry_none(0xF3),
		entry_none(0xF4),
		entry_none(0xF5),
		entry_none(0xF6),
		entry_none(0xF7),

		entry_none(0xF8),
		entry_none(0xF9),
		entry_none(0xFA),
		entry_none(0xFB),
		entry_none(0xFC),
		entry_none(0xFD),
		entry_none(0xFE),
		entry_none(0xFF),
	};


	constexpr TableEntry SecondaryOpcodeMap_F0_Prefix_F2[] = {
		// opcodes 0x00 to 0x0F are not prefixed
		entry_none(0x00), entry_none(0x01), entry_none(0x02), entry_none(0x03),
		entry_none(0x04), entry_none(0x05), entry_none(0x06), entry_none(0x07),
		entry_none(0x08), entry_none(0x09), entry_none(0x0A), entry_none(0x0B),
		entry_none(0x0C), entry_none(0x0D), entry_none(0x0E), entry_none(0x0F),

		entry_none(0x10),
		entry_none(0x11),
		entry_none(0x12),
		entry_none(0x13),
		entry_none(0x14),
		entry_none(0x15),
		entry_none(0x16),
		entry_none(0x17),

		// opcodes 0x18 to 0x27 are not prefixed
		entry_none(0x18), entry_none(0x19), entry_none(0x1A), entry_none(0x1B),
		entry_none(0x1C), entry_none(0x1D), entry_none(0x1E), entry_none(0x1F),
		entry_none(0x20), entry_none(0x21), entry_none(0x22), entry_none(0x23),
		entry_none(0x24), entry_none(0x25), entry_none(0x26), entry_none(0x27),

		entry_none(0x28),
		entry_none(0x29),
		entry_none(0x2A),
		entry_none(0x2B),
		entry_none(0x2C),
		entry_none(0x2D),
		entry_none(0x2E),
		entry_none(0x2F),

		// opcodes 0x30 to 0x4F are not prefixed
		entry_none(0x30), entry_none(0x31), entry_none(0x32), entry_none(0x33),
		entry_none(0x34), entry_none(0x35), entry_none(0x36), entry_none(0x37),
		entry_none(0x38), entry_none(0x39), entry_none(0x3A), entry_none(0x3B),
		entry_none(0x3C), entry_none(0x3D), entry_none(0x3E), entry_none(0x3F),
		entry_none(0x40), entry_none(0x41), entry_none(0x42), entry_none(0x43),
		entry_none(0x44), entry_none(0x45), entry_none(0x46), entry_none(0x47),
		entry_none(0x48), entry_none(0x49), entry_none(0x4A), entry_none(0x4B),
		entry_none(0x4C), entry_none(0x4D), entry_none(0x4E), entry_none(0x4F),

		entry_none(0x50),
		entry_none(0x51),
		entry_none(0x52),
		entry_none(0x53),
		entry_none(0x54),
		entry_none(0x55),
		entry_none(0x56),
		entry_none(0x57),

		entry_none(0x58),
		entry_none(0x59),
		entry_none(0x5A),
		entry_none(0x5B),
		entry_none(0x5C),
		entry_none(0x5D),
		entry_none(0x5E),
		entry_none(0x5F),

		entry_none(0x60),
		entry_none(0x61),
		entry_none(0x62),
		entry_none(0x63),
		entry_none(0x64),
		entry_none(0x65),
		entry_none(0x66),
		entry_none(0x67),

		entry_none(0x68),
		entry_none(0x69),
		entry_none(0x6A),
		entry_none(0x6B),
		entry_none(0x6C),
		entry_none(0x6D),
		entry_none(0x6E),
		entry_none(0x6F),

		entry_none(0x70),
		entry_none(0x71),
		entry_none(0x72),
		entry_none(0x73),
		entry_none(0x74),
		entry_none(0x75),
		entry_none(0x76),
		entry_none(0x77),

		entry_none(0x78),
		entry_none(0x79),
		entry_none(0x7A),
		entry_none(0x7B),
		entry_none(0x7C),
		entry_none(0x7D),
		entry_none(0x7E),
		entry_none(0x7F),

		// opcodes 0x80 to 0xAF are not prefixed
		entry_none(0x80), entry_none(0x81), entry_none(0x82), entry_none(0x83),
		entry_none(0x84), entry_none(0x85), entry_none(0x86), entry_none(0x87),
		entry_none(0x88), entry_none(0x89), entry_none(0x8A), entry_none(0x8B),
		entry_none(0x8C), entry_none(0x8D), entry_none(0x8E), entry_none(0x8F),
		entry_none(0x90), entry_none(0x91), entry_none(0x92), entry_none(0x93),
		entry_none(0x94), entry_none(0x95), entry_none(0x96), entry_none(0x97),
		entry_none(0x98), entry_none(0x99), entry_none(0x9A), entry_none(0x9B),
		entry_none(0x9C), entry_none(0x9D), entry_none(0x9E), entry_none(0x9F),
		entry_none(0xA0), entry_none(0xA1), entry_none(0xA2), entry_none(0xA3),
		entry_none(0xA4), entry_none(0xA5), entry_none(0xA6), entry_none(0xA7),
		entry_none(0xA8), entry_none(0xA9), entry_none(0xAA), entry_none(0xAB),
		entry_none(0xAC), entry_none(0xAD), entry_none(0xAE), entry_none(0xAF),

		// opcodes 0xB0 to 0xB7 are not prefixed
		entry_none(0xB0), entry_none(0xB1), entry_none(0xB2), entry_none(0xB3),
		entry_none(0xB4), entry_none(0xB5), entry_none(0xB6), entry_none(0xB7),

		entry_none(0xB8),
		entry_none(0xB9),
		entry_none(0xBA),
		entry_none(0xBB),
		entry_none(0xBC),
		entry_none(0xBD),
		entry_none(0xBE),
		entry_none(0xBF),

		entry_none(0xC0),
		entry_none(0xC1),
		entry_none(0xC2),
		entry_none(0xC3),
		entry_none(0xC4),
		entry_none(0xC5),
		entry_none(0xC6),
		entry_none(0xC7),

		// opcodes 0xC8 to 0xCF are not prefixed
		entry_none(0xC8), entry_none(0xC9), entry_none(0xCA), entry_none(0xCB),
		entry_none(0xCC), entry_none(0xCD), entry_none(0xCE), entry_none(0xCF),

		entry_none(0xD0),
		entry_none(0xD1),
		entry_none(0xD2),
		entry_none(0xD3),
		entry_none(0xD4),
		entry_none(0xD5),
		entry_none(0xD6),
		entry_none(0xD7),

		entry_none(0xD8),
		entry_none(0xD9),
		entry_none(0xDA),
		entry_none(0xDB),
		entry_none(0xDC),
		entry_none(0xDD),
		entry_none(0xDE),
		entry_none(0xDF),

		entry_none(0xE0),
		entry_none(0xE1),
		entry_none(0xE2),
		entry_none(0xE3),
		entry_none(0xE4),
		entry_none(0xE5),
		entry_none(0xE6),
		entry_none(0xE7),

		entry_none(0xE8),
		entry_none(0xE9),
		entry_none(0xEA),
		entry_none(0xEB),
		entry_none(0xEC),
		entry_none(0xED),
		entry_none(0xEE),
		entry_none(0xEF),

		entry_none(0xF0),
		entry_none(0xF1),
		entry_none(0xF2),
		entry_none(0xF3),
		entry_none(0xF4),
		entry_none(0xF5),
		entry_none(0xF6),
		entry_none(0xF7),

		entry_none(0xF8),
		entry_none(0xF9),
		entry_none(0xFA),
		entry_none(0xFB),
		entry_none(0xFC),
		entry_none(0xFD),
		entry_none(0xFE),
		entry_none(0xFF),
	};


	constexpr TableEntry SecondaryOpcodeMap_F0_Prefix_F3[] = {
		// opcodes 0x00 to 0x0F are not prefixed
		entry_none(0x00), entry_none(0x01), entry_none(0x02), entry_none(0x03),
		entry_none(0x04), entry_none(0x05), entry_none(0x06), entry_none(0x07),
		entry_none(0x08), entry_none(0x09), entry_none(0x0A), entry_none(0x0B),
		entry_none(0x0C), entry_none(0x0D), entry_none(0x0E), entry_none(0x0F),

		entry_none(0x10),
		entry_none(0x11),
		entry_none(0x12),
		entry_none(0x13),
		entry_none(0x14),
		entry_none(0x15),
		entry_none(0x16),
		entry_none(0x17),

		// opcodes 0x18 to 0x27 are not prefixed
		entry_none(0x18), entry_none(0x19), entry_none(0x1A), entry_none(0x1B),
		entry_none(0x1C), entry_none(0x1D), entry_none(0x1E), entry_none(0x1F),
		entry_none(0x20), entry_none(0x21), entry_none(0x22), entry_none(0x23),
		entry_none(0x24), entry_none(0x25), entry_none(0x26), entry_none(0x27),

		entry_none(0x28),
		entry_none(0x29),
		entry_none(0x2A),
		entry_none(0x2B),
		entry_none(0x2C),
		entry_none(0x2D),
		entry_none(0x2E),
		entry_none(0x2F),

		// opcodes 0x30 to 0x4F are not prefixed
		entry_none(0x30), entry_none(0x31), entry_none(0x32), entry_none(0x33),
		entry_none(0x34), entry_none(0x35), entry_none(0x36), entry_none(0x37),
		entry_none(0x38), entry_none(0x39), entry_none(0x3A), entry_none(0x3B),
		entry_none(0x3C), entry_none(0x3D), entry_none(0x3E), entry_none(0x3F),
		entry_none(0x40), entry_none(0x41), entry_none(0x42), entry_none(0x43),
		entry_none(0x44), entry_none(0x45), entry_none(0x46), entry_none(0x47),
		entry_none(0x48), entry_none(0x49), entry_none(0x4A), entry_none(0x4B),
		entry_none(0x4C), entry_none(0x4D), entry_none(0x4E), entry_none(0x4F),

		entry_none(0x50),
		entry_none(0x51),
		entry_none(0x52),
		entry_none(0x53),
		entry_none(0x54),
		entry_none(0x55),
		entry_none(0x56),
		entry_none(0x57),

		entry_none(0x58),
		entry_none(0x59),
		entry_none(0x5A),
		entry_none(0x5B),
		entry_none(0x5C),
		entry_none(0x5D),
		entry_none(0x5E),
		entry_none(0x5F),

		entry_none(0x60),
		entry_none(0x61),
		entry_none(0x62),
		entry_none(0x63),
		entry_none(0x64),
		entry_none(0x65),
		entry_none(0x66),
		entry_none(0x67),

		entry_none(0x68),
		entry_none(0x69),
		entry_none(0x6A),
		entry_none(0x6B),
		entry_none(0x6C),
		entry_none(0x6D),
		entry_none(0x6E),
		entry_none(0x6F),

		entry_none(0x70),
		entry_none(0x71),
		entry_none(0x72),
		entry_none(0x73),
		entry_none(0x74),
		entry_none(0x75),
		entry_none(0x76),
		entry_none(0x77),

		entry_none(0x78),
		entry_none(0x79),
		entry_none(0x7A),
		entry_none(0x7B),
		entry_none(0x7C),
		entry_none(0x7D),
		entry_none(0x7E),
		entry_none(0x7F),

		// opcodes 0x80 to 0xAF are not prefixed
		entry_none(0x80), entry_none(0x81), entry_none(0x82), entry_none(0x83),
		entry_none(0x84), entry_none(0x85), entry_none(0x86), entry_none(0x87),
		entry_none(0x88), entry_none(0x89), entry_none(0x8A), entry_none(0x8B),
		entry_none(0x8C), entry_none(0x8D), entry_none(0x8E), entry_none(0x8F),
		entry_none(0x90), entry_none(0x91), entry_none(0x92), entry_none(0x93),
		entry_none(0x94), entry_none(0x95), entry_none(0x96), entry_none(0x97),
		entry_none(0x98), entry_none(0x99), entry_none(0x9A), entry_none(0x9B),
		entry_none(0x9C), entry_none(0x9D), entry_none(0x9E), entry_none(0x9F),
		entry_none(0xA0), entry_none(0xA1), entry_none(0xA2), entry_none(0xA3),
		entry_none(0xA4), entry_none(0xA5), entry_none(0xA6), entry_none(0xA7),

		entry_none(0xA8),
		entry_none(0xA9),
		entry_none(0xAA),
		entry_none(0xAB),
		entry_none(0xAC),
		entry_none(0xAD),
		entry_ext(0xAE, &ModRMExt_0F_AE_Prefix_F3[0]),
		entry_none(0xAF),

		// opcodes 0xB0 to 0xB7 are not prefixed
		entry_none(0xB0), entry_none(0xB1), entry_none(0xB2), entry_none(0xB3),
		entry_none(0xB4), entry_none(0xB5), entry_none(0xB6), entry_none(0xB7),

		entry_none(0xB8),
		entry_none(0xB9),
		entry_none(0xBA),
		entry_none(0xBB),
		entry_none(0xBC),
		entry_none(0xBD),
		entry_none(0xBE),
		entry_none(0xBF),

		entry_none(0xC0),
		entry_none(0xC1),
		entry_none(0xC2),
		entry_none(0xC3),
		entry_none(0xC4),
		entry_none(0xC5),
		entry_none(0xC6),
		entry_none(0xC7),

		// opcodes 0xC8 to 0xCF are not prefixed
		entry_none(0xC8), entry_none(0xC9), entry_none(0xCA), entry_none(0xCB),
		entry_none(0xCC), entry_none(0xCD), entry_none(0xCE), entry_none(0xCF),

		entry_none(0xD0),
		entry_none(0xD1),
		entry_none(0xD2),
		entry_none(0xD3),
		entry_none(0xD4),
		entry_none(0xD5),
		entry_none(0xD6),
		entry_none(0xD7),

		entry_none(0xD8),
		entry_none(0xD9),
		entry_none(0xDA),
		entry_none(0xDB),
		entry_none(0xDC),
		entry_none(0xDD),
		entry_none(0xDE),
		entry_none(0xDF),

		entry_none(0xE0),
		entry_none(0xE1),
		entry_none(0xE2),
		entry_none(0xE3),
		entry_none(0xE4),
		entry_none(0xE5),
		entry_none(0xE6),
		entry_none(0xE7),

		entry_none(0xE8),
		entry_none(0xE9),
		entry_none(0xEA),
		entry_none(0xEB),
		entry_none(0xEC),
		entry_none(0xED),
		entry_none(0xEE),
		entry_none(0xEF),

		entry_none(0xF0),
		entry_none(0xF1),
		entry_none(0xF2),
		entry_none(0xF3),
		entry_none(0xF4),
		entry_none(0xF5),
		entry_none(0xF6),
		entry_none(0xF7),

		entry_none(0xF8),
		entry_none(0xF9),
		entry_none(0xFA),
		entry_none(0xFB),
		entry_none(0xFC),
		entry_none(0xFD),
		entry_none(0xFE),
		entry_none(0xFF),
	};

	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Normal) == 256, "table invalid");
	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Prefix_66) == 256, "table invalid");
	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Prefix_F2) == 256, "table invalid");
	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Prefix_F3) == 256, "table invalid");
	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Prefix_None) == 256, "table invalid");

	static_assert(ArrayLength(ModRMExt_0F_00) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_01) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_18) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_AE_Prefix_None) == 8, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_AE_Prefix_F3) == 8, "table invalid");

	// these need 9 entries; the first entry is used when modRM.mod != 3, and the next 8 are
	// used (and indexed) by modRM.rm when modrm.mod == 3.
	static_assert(ArrayLength(ModRMExt_0F_01_Mod3_Reg1_RM) == 9, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_01_Mod3_Reg2_RM) == 9, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_01_Mod3_Reg3_RM) == 9, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_01_Mod3_Reg7_RM) == 9, "table invalid");

	// these only need two entries; the first one for when modRM.mod != 3, and
	// the second one for when modRM.mod == 3.
	static_assert(ArrayLength(ModRMExt_0F_AE_Mod3_Reg4_RM) == 2, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_AE_Mod3_Reg5_RM) == 2, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_AE_Mod3_Reg6_RM) == 2, "table invalid");
	static_assert(ArrayLength(ModRMExt_0F_AE_Mod3_Reg7_RM) == 2, "table invalid");
}
