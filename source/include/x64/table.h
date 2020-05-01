// table.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "defs.h"
#include "ops.h"

namespace instrad::x64
{
	enum class OperandKind
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

		Ptr16_16,       // for far calls and jumps
		Ptr16_32,

		None,
	};


	struct TableEntry
	{
	private:
		using __Foozle = OperandKind[3];

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
		OperandKind m_operands[3] = { OperandKind::None, OperandKind::None, OperandKind::None };

		bool m_needsModRM = false;
		bool m_extensionUsesRMBits = false;
		bool m_extensionUsesModBits = false;
		const TableEntry* m_exttable = nullptr;

		friend constexpr TableEntry entry_none(uint8_t);
		friend constexpr TableEntry entry_0(uint8_t, const Op&);
		friend constexpr TableEntry entry_0_modrm(uint8_t, const Op&);
		friend constexpr TableEntry entry_1(uint8_t, const Op&, bool, OperandKind);
		friend constexpr TableEntry entry_2(uint8_t, const Op&, bool, OperandKind, OperandKind);
		friend constexpr TableEntry entry_3(uint8_t, const Op&, bool, OperandKind, OperandKind, OperandKind);
		friend constexpr TableEntry entry_lnri_1(uint8_t, const Op&, OperandKind);
		friend constexpr TableEntry entry_lnri_2(uint8_t, const Op&, bool, OperandKind, OperandKind);
		friend constexpr TableEntry entry_ext(uint8_t, const TableEntry*);
		friend constexpr TableEntry entry_ext_rm(uint8_t, const TableEntry*);
		friend constexpr TableEntry entry_ext_mod(uint8_t, const TableEntry*);
	};

	constexpr TableEntry entry_none(uint8_t opcode)
	{
		auto ret = TableEntry();
		ret.m_opcode = opcode;
		return ret;
	}

	constexpr TableEntry entry_0(uint8_t opcode, const Op& op)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 0;
		return ret;
	}

	// used for stuff in extension tables; takes no operands but still needs modrm.
	constexpr TableEntry entry_0_modrm(uint8_t opcode, const Op& op)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_needsModRM = true;
		ret.m_numOperands = 0;
		return ret;
	}

	constexpr TableEntry entry_1(uint8_t opcode, const Op& op, bool needsModRM, OperandKind dst)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_needsModRM = needsModRM;
		ret.m_numOperands = 1;
		ret.m_operands[0] = dst;
		return ret;
	}

	constexpr TableEntry entry_2(uint8_t opcode, const Op& op, bool needsModRM, OperandKind dst, OperandKind src)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_needsModRM = needsModRM;
		ret.m_numOperands = 2;
		ret.m_operands[0] = dst;
		ret.m_operands[1] = src;
		return ret;
	}

	constexpr TableEntry entry_3(uint8_t opcode, const Op& op, bool needsModRM, OperandKind dst, OperandKind src, OperandKind extra)
	{
		// only used by imul
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_needsModRM = needsModRM;
		ret.m_numOperands = 3;
		ret.m_operands[0] = dst;
		ret.m_operands[1] = src;
		ret.m_operands[2] = extra;
		return ret;
	}

	// if you only take 1 argument, and it's by LNRI, you probably don't need a modRM... right?!
	constexpr TableEntry entry_lnri_1(uint8_t opcode, const Op& op, OperandKind dst)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_numOperands = 1;
		ret.m_operands[0] = dst;
		ret.m_lowNibbleRegIdx = true;
		return ret;
	}

	constexpr TableEntry entry_lnri_2(uint8_t opcode, const Op& op, bool needsModRM, OperandKind dst, OperandKind src)
	{
		auto ret = TableEntry();
		ret.m_op = op;
		ret.m_opcode = opcode;
		ret.m_needsModRM = needsModRM;
		ret.m_numOperands = 2;
		ret.m_operands[0] = dst;
		ret.m_operands[1] = src;
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

	// all the entry_N functions (where N != 0) take a boolean of whether the instruction has a ModRM
	// byte, after the op argument. it's too troublesome and space-wasting to comment /* modrm: */ in each.
	// note that all instructions in ModRM extension tables by definition need to have a ModRM byte!


	// the extension tables are indexed by the ModRM.REG value.
	constexpr TableEntry ModRMExt_80[] = {
		entry_2(0x80, ops::ADD, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x80, ops::OR,  true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x80, ops::ADC, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x80, ops::SBB, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x80, ops::AND, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x80, ops::SUB, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x80, ops::XOR, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x80, ops::CMP, true, OperandKind::RegMem8, OperandKind::Imm8),
	};

	constexpr TableEntry ModRMExt_81[] = {
		entry_2(0x81, ops::ADD, true, OperandKind::RegMem32, OperandKind::Imm32),
		entry_2(0x81, ops::OR,  true, OperandKind::RegMem32, OperandKind::Imm32),
		entry_2(0x81, ops::ADC, true, OperandKind::RegMem32, OperandKind::Imm32),
		entry_2(0x81, ops::SBB, true, OperandKind::RegMem32, OperandKind::Imm32),
		entry_2(0x81, ops::AND, true, OperandKind::RegMem32, OperandKind::Imm32),
		entry_2(0x81, ops::SUB, true, OperandKind::RegMem32, OperandKind::Imm32),
		entry_2(0x81, ops::XOR, true, OperandKind::RegMem32, OperandKind::Imm32),
		entry_2(0x81, ops::CMP, true, OperandKind::RegMem32, OperandKind::Imm32),
	};

	// these are apparently invalid in 64-bit mode.
	constexpr TableEntry ModRMExt_82[] = {
		entry_2(0x82, ops::ADD, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x82, ops::OR,  true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x82, ops::ADC, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x82, ops::SBB, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x82, ops::AND, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x82, ops::SUB, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x82, ops::XOR, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0x82, ops::CMP, true, OperandKind::RegMem8, OperandKind::Imm8),
	};

	constexpr TableEntry ModRMExt_83[] = {
		entry_2(0x83, ops::ADD, true, OperandKind::RegMem32, OperandKind::SignExtImm8),
		entry_2(0x83, ops::OR,  true, OperandKind::RegMem32, OperandKind::SignExtImm8),
		entry_2(0x83, ops::ADC, true, OperandKind::RegMem32, OperandKind::SignExtImm8),
		entry_2(0x83, ops::SBB, true, OperandKind::RegMem32, OperandKind::SignExtImm8),
		entry_2(0x83, ops::AND, true, OperandKind::RegMem32, OperandKind::SignExtImm8),
		entry_2(0x83, ops::SUB, true, OperandKind::RegMem32, OperandKind::SignExtImm8),
		entry_2(0x83, ops::XOR, true, OperandKind::RegMem32, OperandKind::SignExtImm8),
		entry_2(0x83, ops::CMP, true, OperandKind::RegMem32, OperandKind::SignExtImm8),
	};

	constexpr TableEntry ModRMExt_8F[] = {
		entry_1(0x8F, ops::POP, true, OperandKind::RegMem64),

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank
	};

	constexpr TableEntry ModRMExt_C0[] = {
		entry_2(0xC0, ops::ROL, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0xC0, ops::ROR, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0xC0, ops::RCL, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0xC0, ops::RCR, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0xC0, ops::SHL, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0xC0, ops::SHR, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0xC0, ops::SAL, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0xC0, ops::SAR, true, OperandKind::RegMem8, OperandKind::Imm8),
	};

	constexpr TableEntry ModRMExt_C1[] = {
		entry_2(0xC1, ops::ROL, true, OperandKind::RegMem32, OperandKind::Imm8),
		entry_2(0xC1, ops::ROR, true, OperandKind::RegMem32, OperandKind::Imm8),
		entry_2(0xC1, ops::RCL, true, OperandKind::RegMem32, OperandKind::Imm8),
		entry_2(0xC1, ops::RCR, true, OperandKind::RegMem32, OperandKind::Imm8),
		entry_2(0xC1, ops::SHL, true, OperandKind::RegMem32, OperandKind::Imm8),
		entry_2(0xC1, ops::SHR, true, OperandKind::RegMem32, OperandKind::Imm8),
		entry_2(0xC1, ops::SAL, true, OperandKind::RegMem32, OperandKind::Imm8),
		entry_2(0xC1, ops::SAR, true, OperandKind::RegMem32, OperandKind::Imm8),
	};

	constexpr TableEntry ModRMExt_C6[] = {
		entry_2(0xC6, ops::MOV, true, OperandKind::RegMem8, OperandKind::Imm8),

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};

	constexpr TableEntry ModRMExt_C7[] = {
		entry_2(0xC7, ops::MOV, true, OperandKind::RegMem32, OperandKind::Imm32),

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};

	constexpr TableEntry ModRMExt_D0[] = {
		entry_1(0xD0, ops::ROL, true, OperandKind::RegMem8),
		entry_1(0xD0, ops::ROR, true, OperandKind::RegMem8),
		entry_1(0xD0, ops::RCL, true, OperandKind::RegMem8),
		entry_1(0xD0, ops::RCR, true, OperandKind::RegMem8),
		entry_1(0xD0, ops::SHL, true, OperandKind::RegMem8),
		entry_1(0xD0, ops::SHR, true, OperandKind::RegMem8),
		entry_1(0xD0, ops::SAL, true, OperandKind::RegMem8),
		entry_1(0xD0, ops::SAR, true, OperandKind::RegMem8),
	};

	constexpr TableEntry ModRMExt_D1[] = {
		entry_1(0xD1, ops::ROL, true, OperandKind::RegMem32),
		entry_1(0xD1, ops::ROR, true, OperandKind::RegMem32),
		entry_1(0xD1, ops::RCL, true, OperandKind::RegMem32),
		entry_1(0xD1, ops::RCR, true, OperandKind::RegMem32),
		entry_1(0xD1, ops::SHL, true, OperandKind::RegMem32),
		entry_1(0xD1, ops::SHR, true, OperandKind::RegMem32),
		entry_1(0xD1, ops::SAL, true, OperandKind::RegMem32),
		entry_1(0xD1, ops::SAR, true, OperandKind::RegMem32),
	};

	constexpr TableEntry ModRMExt_D2[] = {
		entry_2(0xD2, ops::ROL, true, OperandKind::RegMem8, OperandKind::ImplicitCL),
		entry_2(0xD2, ops::ROR, true, OperandKind::RegMem8, OperandKind::ImplicitCL),
		entry_2(0xD2, ops::RCL, true, OperandKind::RegMem8, OperandKind::ImplicitCL),
		entry_2(0xD2, ops::RCR, true, OperandKind::RegMem8, OperandKind::ImplicitCL),
		entry_2(0xD2, ops::SHL, true, OperandKind::RegMem8, OperandKind::ImplicitCL),
		entry_2(0xD2, ops::SHR, true, OperandKind::RegMem8, OperandKind::ImplicitCL),
		entry_2(0xD2, ops::SAL, true, OperandKind::RegMem8, OperandKind::ImplicitCL),
		entry_2(0xD2, ops::SAR, true, OperandKind::RegMem8, OperandKind::ImplicitCL),
	};

	constexpr TableEntry ModRMExt_D3[] = {
		entry_2(0xD3, ops::ROL, true, OperandKind::RegMem32, OperandKind::ImplicitCL),
		entry_2(0xD3, ops::ROR, true, OperandKind::RegMem32, OperandKind::ImplicitCL),
		entry_2(0xD3, ops::RCL, true, OperandKind::RegMem32, OperandKind::ImplicitCL),
		entry_2(0xD3, ops::RCR, true, OperandKind::RegMem32, OperandKind::ImplicitCL),
		entry_2(0xD3, ops::SHL, true, OperandKind::RegMem32, OperandKind::ImplicitCL),
		entry_2(0xD3, ops::SHR, true, OperandKind::RegMem32, OperandKind::ImplicitCL),
		entry_2(0xD3, ops::SAL, true, OperandKind::RegMem32, OperandKind::ImplicitCL),
		entry_2(0xD3, ops::SAR, true, OperandKind::RegMem32, OperandKind::ImplicitCL),
	};


	constexpr TableEntry ModRMExt_F6[] = {
		entry_2(0xF6, ops::TEST, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_2(0xF6, ops::TEST, true, OperandKind::RegMem8, OperandKind::Imm8),
		entry_1(0xF6, ops::NOT,  true, OperandKind::RegMem8),
		entry_1(0xF6, ops::NEG,  true, OperandKind::RegMem8),
		entry_1(0xF6, ops::MUL,  true, OperandKind::RegMem8),
		entry_1(0xF6, ops::IMUL, true, OperandKind::RegMem8),
		entry_1(0xF6, ops::DIV,  true, OperandKind::RegMem8),
		entry_1(0xF6, ops::IDIV, true, OperandKind::RegMem8),
	};

	constexpr TableEntry ModRMExt_F7[] = {
		entry_2(0xF7, ops::TEST, true, OperandKind::RegMem32, OperandKind::Imm32),
		entry_2(0xF7, ops::TEST, true, OperandKind::RegMem32, OperandKind::Imm32),
		entry_1(0xF7, ops::NOT,  true, OperandKind::RegMem32),
		entry_1(0xF7, ops::NEG,  true, OperandKind::RegMem32),
		entry_1(0xF7, ops::MUL,  true, OperandKind::RegMem32),
		entry_1(0xF7, ops::IMUL, true, OperandKind::RegMem32),
		entry_1(0xF7, ops::DIV,  true, OperandKind::RegMem32),
		entry_1(0xF7, ops::IDIV, true, OperandKind::RegMem32),
	};

	constexpr TableEntry ModRMExt_FE[] = {
		entry_1(0xFE, ops::INC, true, OperandKind::RegMem8),
		entry_1(0xFE, ops::DEC, true, OperandKind::RegMem8),

		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
		entry_blank,
	};

	constexpr TableEntry ModRMExt_FF[] = {
		entry_1(0xFF, ops::INC,  true, OperandKind::RegMem32),
		entry_1(0xFF, ops::DEC,  true, OperandKind::RegMem32),
		entry_1(0xFF, ops::CALL, true, OperandKind::RegMem64),
		entry_1(0xFF, ops::CALL, true, OperandKind::Ptr16_32),
		entry_1(0xFF, ops::JMP,  true, OperandKind::RegMem64),
		entry_1(0xFF, ops::JMP,  true, OperandKind::Ptr16_32),
		entry_1(0xFF, ops::PUSH, true, OperandKind::RegMem64),

		entry_blank,
	};








	constexpr TableEntry PrimaryOpcodeMap[] = {
		entry_2(0x00, ops::ADD,  true,  OperandKind::RegMem8, OperandKind::Reg8),
		entry_2(0x01, ops::ADD,  true,  OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0x02, ops::ADD,  true,  OperandKind::Reg8, OperandKind::RegMem8),
		entry_2(0x03, ops::ADD,  true,  OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x04, ops::ADD,  false, OperandKind::ImplicitAL, OperandKind::Imm8),
		entry_2(0x05, ops::ADD,  false, OperandKind::ImplicitEAX, OperandKind::Imm32),
		entry_1(0x06, ops::PUSH, false, OperandKind::ImplicitES),           // push es; invalid in 64-bit
		entry_1(0x07, ops::POP,  false, OperandKind::ImplicitES),           // pop es; invalid in 64-bit

		entry_2(0x08, ops::OR,   true,  OperandKind::RegMem8, OperandKind::Reg8),
		entry_2(0x09, ops::OR,   true,  OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0x0A, ops::OR,   true,  OperandKind::Reg8, OperandKind::RegMem8),
		entry_2(0x0B, ops::OR,   true,  OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x0C, ops::OR,   false, OperandKind::ImplicitAL, OperandKind::Imm8),
		entry_2(0x0D, ops::OR,   false, OperandKind::ImplicitEAX, OperandKind::Imm32),
		entry_1(0x0E, ops::PUSH, false, OperandKind::ImplicitCS),           // push cs; invalid in 64-bit
		entry_none(0x0F),                                                   // escape sequence.

		entry_2(0x10, ops::ADC,  true,  OperandKind::RegMem8, OperandKind::Reg8),
		entry_2(0x11, ops::ADC,  true,  OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0x12, ops::ADC,  true,  OperandKind::Reg8, OperandKind::RegMem8),
		entry_2(0x13, ops::ADC,  true,  OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x14, ops::ADC,  false, OperandKind::ImplicitAL, OperandKind::Imm8),
		entry_2(0x15, ops::ADC,  false, OperandKind::ImplicitEAX, OperandKind::Imm32),
		entry_1(0x16, ops::PUSH, false, OperandKind::ImplicitSS),           // push ss; invalid in 64-bit
		entry_1(0x17, ops::POP,  false, OperandKind::ImplicitSS),           // pop ss; invalid in 64-bit

		entry_2(0x18, ops::SBB,  true,  OperandKind::RegMem8, OperandKind::Reg8),
		entry_2(0x19, ops::SBB,  true,  OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0x1A, ops::SBB,  true,  OperandKind::Reg8, OperandKind::RegMem8),
		entry_2(0x1B, ops::SBB,  true,  OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x1C, ops::SBB,  false, OperandKind::ImplicitAL, OperandKind::Imm8),
		entry_2(0x1D, ops::SBB,  false, OperandKind::ImplicitEAX, OperandKind::Imm32),
		entry_1(0x1E, ops::PUSH, false, OperandKind::ImplicitDS),           // push ds; invalid in 64-bit
		entry_1(0x1F, ops::POP,  false, OperandKind::ImplicitDS),           // pop ds; invalid in 64-bit

		entry_2(0x20, ops::AND, true,  OperandKind::RegMem8, OperandKind::Reg8),
		entry_2(0x21, ops::AND, true,  OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0x22, ops::AND, true,  OperandKind::Reg8, OperandKind::RegMem8),
		entry_2(0x23, ops::AND, true,  OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x24, ops::AND, false, OperandKind::ImplicitAL, OperandKind::Imm8),
		entry_2(0x25, ops::AND, false, OperandKind::ImplicitEAX, OperandKind::Imm32),
		entry_none(0x26),                                                   // ES segment override
		entry_0(0x27, ops::DAA),                                            // invalid in 64-bit mode

		entry_2(0x28, ops::SUB, true,  OperandKind::RegMem8, OperandKind::Reg8),
		entry_2(0x29, ops::SUB, true,  OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0x2A, ops::SUB, true,  OperandKind::Reg8, OperandKind::RegMem8),
		entry_2(0x2B, ops::SUB, true,  OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x2C, ops::SUB, false, OperandKind::ImplicitAL, OperandKind::Imm8),
		entry_2(0x2D, ops::SUB, false, OperandKind::ImplicitEAX, OperandKind::Imm32),
		entry_none(0x2E),                                                   // CS segment override
		entry_0(0x2F, ops::DAS),                                            // invalid in 64-bit mode

		entry_2(0x30, ops::XOR, true,  OperandKind::RegMem8, OperandKind::Reg8),
		entry_2(0x31, ops::XOR, true,  OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0x32, ops::XOR, true,  OperandKind::Reg8, OperandKind::RegMem8),
		entry_2(0x33, ops::XOR, true,  OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x34, ops::XOR, false, OperandKind::ImplicitAL, OperandKind::Imm8),
		entry_2(0x35, ops::XOR, false, OperandKind::ImplicitEAX, OperandKind::Imm32),
		entry_none(0x36),                                                   // SS segment override
		entry_0(0x37, ops::AAA),                                            // invalid in 64-bit mode

		entry_2(0x38, ops::CMP, true,  OperandKind::RegMem8, OperandKind::Reg8),
		entry_2(0x39, ops::CMP, true,  OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0x3A, ops::CMP, true,  OperandKind::Reg8, OperandKind::RegMem8),
		entry_2(0x3B, ops::CMP, true,  OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x3C, ops::CMP, false, OperandKind::ImplicitAL, OperandKind::Imm8),
		entry_2(0x3D, ops::CMP, false, OperandKind::ImplicitEAX, OperandKind::Imm32),
		entry_none(0x3E),                                                   // DS segment override
		entry_0(0x3F, ops::AAS),                                            // invalid in 64-bit mode

		// in 64-bit mode, this is the REX prefix. but we want to support x86 also, so these are the inc/dec opcodes
		entry_lnri_1(0x40, ops::INC, OperandKind::Reg32),
		entry_lnri_1(0x41, ops::INC, OperandKind::Reg32),
		entry_lnri_1(0x42, ops::INC, OperandKind::Reg32),
		entry_lnri_1(0x43, ops::INC, OperandKind::Reg32),
		entry_lnri_1(0x44, ops::INC, OperandKind::Reg32),
		entry_lnri_1(0x45, ops::INC, OperandKind::Reg32),
		entry_lnri_1(0x46, ops::INC, OperandKind::Reg32),
		entry_lnri_1(0x47, ops::INC, OperandKind::Reg32),

		entry_lnri_1(0x48, ops::DEC, OperandKind::Reg32),
		entry_lnri_1(0x49, ops::DEC, OperandKind::Reg32),
		entry_lnri_1(0x4A, ops::DEC, OperandKind::Reg32),
		entry_lnri_1(0x4B, ops::DEC, OperandKind::Reg32),
		entry_lnri_1(0x4C, ops::DEC, OperandKind::Reg32),
		entry_lnri_1(0x4D, ops::DEC, OperandKind::Reg32),
		entry_lnri_1(0x4E, ops::DEC, OperandKind::Reg32),
		entry_lnri_1(0x4F, ops::DEC, OperandKind::Reg32),

		entry_lnri_1(0x50, ops::PUSH, OperandKind::Reg64),
		entry_lnri_1(0x51, ops::PUSH, OperandKind::Reg64),
		entry_lnri_1(0x52, ops::PUSH, OperandKind::Reg64),
		entry_lnri_1(0x53, ops::PUSH, OperandKind::Reg64),
		entry_lnri_1(0x54, ops::PUSH, OperandKind::Reg64),
		entry_lnri_1(0x55, ops::PUSH, OperandKind::Reg64),
		entry_lnri_1(0x56, ops::PUSH, OperandKind::Reg64),
		entry_lnri_1(0x57, ops::PUSH, OperandKind::Reg64),

		entry_lnri_1(0x58, ops::POP, OperandKind::Reg64),
		entry_lnri_1(0x59, ops::POP, OperandKind::Reg64),
		entry_lnri_1(0x5A, ops::POP, OperandKind::Reg64),
		entry_lnri_1(0x5B, ops::POP, OperandKind::Reg64),
		entry_lnri_1(0x5C, ops::POP, OperandKind::Reg64),
		entry_lnri_1(0x5D, ops::POP, OperandKind::Reg64),
		entry_lnri_1(0x5E, ops::POP, OperandKind::Reg64),
		entry_lnri_1(0x5F, ops::POP, OperandKind::Reg64),

		entry_0(0x60, ops::PUSHAD),
		entry_0(0x61, ops::POPAD),
		entry_2(0x62, ops::BOUND, true, OperandKind::Reg32, OperandKind::RegMem32),

		// in legacy mode this is interpreted as ARPL, but who cares? long mode ftw.
		entry_2(0x63, ops::MOVSXD, true, OperandKind::Reg64, OperandKind::RegMem64),
		entry_none(0x64),                                                   // FS segment override
		entry_none(0x65),                                                   // GS segment override
		entry_none(0x66),                                                   // operand size override
		entry_none(0x67),                                                   // address size override

		entry_1(0x68, ops::PUSH, false, OperandKind::SignExtImm32),
		entry_3(0x69, ops::IMUL, true,  OperandKind::Reg32, OperandKind::RegMem32, OperandKind::Imm32),
		entry_1(0x6A, ops::PUSH, false, OperandKind::Imm8),
		entry_3(0x6B, ops::IMUL, true,  OperandKind::Reg32, OperandKind::RegMem32, OperandKind::Imm8),
		entry_0(0x6C, ops::INSB),
		entry_0(0x6D, ops::INSD),
		entry_0(0x6E, ops::OUTSB),
		entry_0(0x6F, ops::OUTSD),

		entry_1(0x70, ops::JO,  false, OperandKind::Rel8Offset),
		entry_1(0x71, ops::JNO, false, OperandKind::Rel8Offset),
		entry_1(0x72, ops::JB,  false, OperandKind::Rel8Offset),
		entry_1(0x73, ops::JNB, false, OperandKind::Rel8Offset),
		entry_1(0x74, ops::JZ,  false, OperandKind::Rel8Offset),
		entry_1(0x75, ops::JNZ, false, OperandKind::Rel8Offset),
		entry_1(0x76, ops::JNA, false, OperandKind::Rel8Offset),
		entry_1(0x77, ops::JA,  false, OperandKind::Rel8Offset),

		entry_1(0x78, ops::JS,  false, OperandKind::Rel8Offset),
		entry_1(0x79, ops::JNS, false, OperandKind::Rel8Offset),
		entry_1(0x7A, ops::JP,  false, OperandKind::Rel8Offset),
		entry_1(0x7B, ops::JNP, false, OperandKind::Rel8Offset),
		entry_1(0x7C, ops::JL,  false, OperandKind::Rel8Offset),
		entry_1(0x7D, ops::JGE, false, OperandKind::Rel8Offset),
		entry_1(0x7E, ops::JLE, false, OperandKind::Rel8Offset),
		entry_1(0x7F, ops::JG,  false, OperandKind::Rel8Offset),

		// 0x80 to 0x83 are part of the "group 1" ModRM.reg extensions.
		entry_ext(0x80, &ModRMExt_80[0]),
		entry_ext(0x81, &ModRMExt_81[0]),
		entry_ext(0x82, &ModRMExt_82[0]),
		entry_ext(0x83, &ModRMExt_83[0]),
		entry_2(0x84, ops::TEST, true, OperandKind::RegMem8,  OperandKind::Reg8),
		entry_2(0x85, ops::TEST, true, OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0x86, ops::XCHG, true, OperandKind::RegMem8,  OperandKind::Reg8),
		entry_2(0x87, ops::XCHG, true, OperandKind::RegMem32, OperandKind::Reg32),

		entry_2(0x88, ops::MOV, true, OperandKind::RegMem8,    OperandKind::Reg8),
		entry_2(0x89, ops::MOV, true, OperandKind::RegMem32,   OperandKind::Reg32),
		entry_2(0x8A, ops::MOV, true, OperandKind::Reg8,       OperandKind::RegMem8),
		entry_2(0x8B, ops::MOV, true, OperandKind::Reg32,      OperandKind::RegMem32),
		entry_2(0x8C, ops::MOV, true, OperandKind::RegMem16,   OperandKind::SegmentReg),
		entry_2(0x8D, ops::LEA, true, OperandKind::Reg32,      OperandKind::Memory),
		entry_2(0x8E, ops::MOV, true, OperandKind::SegmentReg, OperandKind::RegMem16),

		// XOP prefix (but also POP with ModRM.reg=0), for whatever fucking reason. handle this separately again.
		entry_ext(0x8F, &ModRMExt_8F[0]),

		// all of these XCHGs are implicitly exchanging with RAX.
		entry_lnri_2(0x90, ops::XCHG, false, OperandKind::ImplicitRAX, OperandKind::Reg64),
		entry_lnri_2(0x91, ops::XCHG, false, OperandKind::ImplicitRAX, OperandKind::Reg64),
		entry_lnri_2(0x92, ops::XCHG, false, OperandKind::ImplicitRAX, OperandKind::Reg64),
		entry_lnri_2(0x93, ops::XCHG, false, OperandKind::ImplicitRAX, OperandKind::Reg64),
		entry_lnri_2(0x94, ops::XCHG, false, OperandKind::ImplicitRAX, OperandKind::Reg64),
		entry_lnri_2(0x95, ops::XCHG, false, OperandKind::ImplicitRAX, OperandKind::Reg64),
		entry_lnri_2(0x96, ops::XCHG, false, OperandKind::ImplicitRAX, OperandKind::Reg64),
		entry_lnri_2(0x97, ops::XCHG, false, OperandKind::ImplicitRAX, OperandKind::Reg64),

		entry_0(0x98, ops::CWDE),
		entry_0(0x99, ops::CDQ),
		entry_1(0x9A, ops::CALLF, false, OperandKind::Ptr16_32),            // far call, invalid in 64-bit
		entry_0(0x9B, ops::FWAIT),
		entry_0(0x9C, ops::PUSHF),
		entry_0(0x9D, ops::POPF),
		entry_0(0x9E, ops::SAHF),
		entry_0(0x9F, ops::LAHF),

		entry_2(0xA0, ops::MOV, false, OperandKind::ImplicitAL,  OperandKind::MemoryOfs8),
		entry_2(0xA1, ops::MOV, false, OperandKind::ImplicitEAX, OperandKind::MemoryOfs32),
		entry_2(0xA2, ops::MOV, false, OperandKind::MemoryOfs8,  OperandKind::ImplicitAL),
		entry_2(0xA3, ops::MOV, false, OperandKind::MemoryOfs32, OperandKind::ImplicitEAX),
		entry_0(0xA4, ops::MOVSB),
		entry_0(0xA5, ops::MOVSD),
		entry_0(0xA6, ops::CMPSB),
		entry_0(0xA7, ops::CMPSD),

		entry_2(0xA8, ops::TEST, false, OperandKind::ImplicitAL,  OperandKind::Imm8),
		entry_2(0xA9, ops::TEST, false, OperandKind::ImplicitEAX, OperandKind::Imm32),
		entry_0(0xAA, ops::STOSB),
		entry_0(0xAB, ops::STOSD),
		entry_0(0xAC, ops::LODSB),
		entry_0(0xAD, ops::LODSD),
		entry_0(0xAE, ops::SCASB),
		entry_0(0xAF, ops::SCASD),

		entry_lnri_2(0xB0, ops::MOV, false, OperandKind::Reg8, OperandKind::Imm8),
		entry_lnri_2(0xB1, ops::MOV, false, OperandKind::Reg8, OperandKind::Imm8),
		entry_lnri_2(0xB2, ops::MOV, false, OperandKind::Reg8, OperandKind::Imm8),
		entry_lnri_2(0xB3, ops::MOV, false, OperandKind::Reg8, OperandKind::Imm8),
		entry_lnri_2(0xB4, ops::MOV, false, OperandKind::Reg8, OperandKind::Imm8),
		entry_lnri_2(0xB5, ops::MOV, false, OperandKind::Reg8, OperandKind::Imm8),
		entry_lnri_2(0xB6, ops::MOV, false, OperandKind::Reg8, OperandKind::Imm8),
		entry_lnri_2(0xB7, ops::MOV, false, OperandKind::Reg8, OperandKind::Imm8),

		// encode the immediate size as 64-bits; the handling code will need to know how
		// to downgrade to either 32 or 16 bits depending on the prefix.
		entry_lnri_2(0xB8, ops::MOV, false, OperandKind::Reg32, OperandKind::Imm64),
		entry_lnri_2(0xB9, ops::MOV, false, OperandKind::Reg32, OperandKind::Imm64),
		entry_lnri_2(0xBA, ops::MOV, false, OperandKind::Reg32, OperandKind::Imm64),
		entry_lnri_2(0xBB, ops::MOV, false, OperandKind::Reg32, OperandKind::Imm64),
		entry_lnri_2(0xBC, ops::MOV, false, OperandKind::Reg32, OperandKind::Imm64),
		entry_lnri_2(0xBD, ops::MOV, false, OperandKind::Reg32, OperandKind::Imm64),
		entry_lnri_2(0xBE, ops::MOV, false, OperandKind::Reg32, OperandKind::Imm64),
		entry_lnri_2(0xBF, ops::MOV, false, OperandKind::Reg32, OperandKind::Imm64),

		entry_ext(0xC0, &ModRMExt_C0[0]),                                   // the whole "group 2 extensions" of shift/rotate opcodes
		entry_ext(0xC1, &ModRMExt_C1[0]),
		entry_1(0xC2, ops::RET, false, OperandKind::Imm16),
		entry_0(0xC3, ops::RET),
		entry_none(0xC4),                                                   // vector extension escape sequence
		entry_none(0xC5),                                                   // vector extension escape sequence
		entry_ext(0xC6, &ModRMExt_C6[0]),
		entry_ext(0xC7, &ModRMExt_C7[0]),

		entry_2(0xC8, ops::ENTER, false, OperandKind::Imm16, OperandKind::Imm8),
		entry_0(0xC9, ops::LEAVE),
		entry_1(0xCA, ops::RETF, false, OperandKind::Imm16),
		entry_0(0xCB, ops::RETF),
		entry_0(0xCC, ops::INT3),
		entry_1(0xCD, ops::INT, false, OperandKind::Imm8),
		entry_0(0xCE, ops::INTO),                  // invalid in 64-bit
		entry_0(0xCF, ops::IRET),

		entry_ext(0xD0, &ModRMExt_D0[0]),                                   // group 2 extensions, shift/rotate
		entry_ext(0xD1, &ModRMExt_D1[0]),                                   // group 2 extensions, shift/rotate
		entry_ext(0xD2, &ModRMExt_D2[0]),                                   // group 2 extensions, shift/rotate
		entry_ext(0xD3, &ModRMExt_D3[0]),                                   // group 2 extensions, shift/rotate
		entry_1(0xD4, ops::AAM, false, OperandKind::Imm8),                  // invalid in 64-bit
		entry_1(0xD5, ops::AAD, false, OperandKind::Imm8),                  // invalid in 64-bit
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

		entry_1(0xE0, ops::LOOPNZ, false, OperandKind::Rel8Offset),
		entry_1(0xE1, ops::LOOPZ,  false, OperandKind::Rel8Offset),
		entry_1(0xE2, ops::LOOP,   false, OperandKind::Rel8Offset),
		entry_1(0xE3, ops::JECXZ,  false, OperandKind::Rel8Offset),
		entry_2(0xE4, ops::IN,     false, OperandKind::ImplicitAL, OperandKind::Imm8),
		entry_2(0xE5, ops::IN,     false, OperandKind::ImplicitEAX, OperandKind::Imm8),
		entry_2(0xE6, ops::OUT,    false, OperandKind::Imm8, OperandKind::ImplicitAL),
		entry_2(0xE7, ops::OUT,    false, OperandKind::Imm8, OperandKind::ImplicitEAX),

		entry_1(0xE8, ops::CALL, false, OperandKind::Rel32Offset),
		entry_1(0xE9, ops::JMP,  false, OperandKind::Rel32Offset),
		entry_none(0xEA),                                                   // apparently invalid
		entry_1(0xEB, ops::JMP, false, OperandKind::Rel8Offset),
		entry_2(0xEC, ops::IN,  false, OperandKind::ImplicitAL, OperandKind::ImplicitDX),
		entry_2(0xED, ops::IN,  false, OperandKind::ImplicitEAX, OperandKind::ImplicitDX),
		entry_2(0xEE, ops::OUT, false, OperandKind::ImplicitDX, OperandKind::ImplicitAL),
		entry_2(0xEF, ops::OUT, false, OperandKind::ImplicitDX, OperandKind::ImplicitEAX),

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
		entry_1(0x00, ops::SLDT, true, OperandKind::RegMem32),              // it's not really r/m32, but... it works.
		entry_1(0x00, ops::STR,  true, OperandKind::RegMem32),              // it's not really r/m32, but... it works.
		entry_1(0x00, ops::LLDT, true, OperandKind::RegMem16),
		entry_1(0x00, ops::LTR,  true, OperandKind::RegMem16),
		entry_1(0x00, ops::VERR, true, OperandKind::RegMem16),
		entry_1(0x00, ops::VERW, true, OperandKind::RegMem16),

		entry_blank,
		entry_blank,
	};

	// group 7. these fuckers additionally need ModRM.RM bits to differentiate, so make a nested table.
	// each entry here is offset by 1; the extensions are only used when modRM.mod == 3; if it is not, then
	// the first entry is the real instruction. if it == 3, then use the following instructions, but add 1 to
	// modRM.rm to get which instruction to use.
	constexpr TableEntry ModRMExt_0F_01_Mod3_Reg1_RM[] = {
		entry_1(0x01, ops::SIDT, true, OperandKind::Memory),                // it's actually mem16:32/64, but... it works.

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
		entry_1(0x01, ops::LGDT, true, OperandKind::Memory),                // it's actually mem16:32/64, but... it works.

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
		entry_1(0x01, ops::LIDT, true, OperandKind::Memory),                // it's actually mem16:32/64, but... it works.

		entry_0_modrm(0x01, ops::VMRUN),                                    // modRM == 0xD8
		entry_0_modrm(0x01, ops::VMCALL),                                   // modRM == 0xD9
		entry_1(0x01, ops::VMLOAD, true, OperandKind::ImplicitRAX),         // modRM == 0xDA
		entry_1(0x01, ops::VMSAVE, true, OperandKind::ImplicitRAX),         // modRM == 0xDB
		entry_0_modrm(0x01, ops::STGI),                                     // modRM == 0xDC
		entry_0_modrm(0x01, ops::CLGI),                                     // modRM == 0xDD
		entry_1(0x01, ops::SKINIT, true, OperandKind::ImplicitEAX),         // modRM == 0xDE
		entry_2(0x01, ops::INVLPGA, true, OperandKind::ImplicitRAX, OperandKind::ImplicitECX), // modRM == 0xDF
	};

	constexpr TableEntry ModRMExt_0F_01_Mod3_Reg7_RM[] = {
		entry_1(0x01, ops::INVLPG, true, OperandKind::Memory),

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
		entry_1(0x01, ops::SGDT, true, OperandKind::Memory),                // it's actually mem16:32/64, but... it works.
		entry_ext_rm(0x01, &ModRMExt_0F_01_Mod3_Reg1_RM[0]),
		entry_ext_rm(0x01, &ModRMExt_0F_01_Mod3_Reg2_RM[0]),
		entry_ext_rm(0x01, &ModRMExt_0F_01_Mod3_Reg3_RM[0]),
		entry_1(0x01, ops::SMSW, true, OperandKind::RegMem32),              // not really...
		entry_none(0x01),
		entry_1(0x01, ops::LMSW, true, OperandKind::RegMem16),
		entry_ext_rm(0x1, &ModRMExt_0F_01_Mod3_Reg7_RM[0]),
	};

	// group 16
	constexpr TableEntry ModRMExt_0F_18[] = {
		entry_1(0x18, ops::PREFETCH, true, OperandKind::Memory),
		entry_1(0x18, ops::PREFETCH, true, OperandKind::Memory),
		entry_1(0x18, ops::PREFETCH, true, OperandKind::Memory),
		entry_1(0x18, ops::PREFETCH, true, OperandKind::Memory),
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
		entry_1(0xAE, ops::XSAVE, true, OperandKind::Memory),       // modRM.mod != 3
		entry_none(0xAE),                                           // modRM.mod == 3
	};

	constexpr TableEntry ModRMExt_0F_AE_Mod3_Reg5_RM[] = {
		entry_1(0xAE, ops::XRSTOR, true, OperandKind::Memory),      // modRM.mod != 3
		entry_0_modrm(0xAE, ops::LFENCE),                           // modRM.mod == 3
	};

	constexpr TableEntry ModRMExt_0F_AE_Mod3_Reg6_RM[] = {
		entry_1(0xAE, ops::XSAVEOPT, true, OperandKind::Memory),    // modRM.mod != 3
		entry_0_modrm(0xAE, ops::MFENCE),                           // modRM.mod == 3
	};

	constexpr TableEntry ModRMExt_0F_AE_Mod3_Reg7_RM[] = {
		entry_1(0xAE, ops::CLFLUSH, true, OperandKind::Memory),     // modRM.mod != 3
		entry_0_modrm(0xAE, ops::SFENCE),                           // modRM.mod == 3
	};

	// only used when opcode == 0x0F 0xAE, and has no prefix
	constexpr TableEntry ModRMExt_0F_AE_Prefix_None[] = {
		entry_1(0xAE, ops::FXSAVE,  true, OperandKind::Memory),
		entry_1(0xAE, ops::FXRSTOR, true, OperandKind::Memory),
		entry_1(0xAE, ops::LDMXCSR, true, OperandKind::Memory),
		entry_1(0xAE, ops::STMXCSR, true, OperandKind::Memory),
		entry_ext_mod(0xAE, &ModRMExt_0F_AE_Mod3_Reg4_RM[0]),
		entry_ext_mod(0xAE, &ModRMExt_0F_AE_Mod3_Reg5_RM[0]),
		entry_ext_mod(0xAE, &ModRMExt_0F_AE_Mod3_Reg6_RM[0]),
		entry_ext_mod(0xAE, &ModRMExt_0F_AE_Mod3_Reg7_RM[0]),
	};

	// only used when opcode == 0x0F 0xAE, and has prefix 0xF3
	constexpr TableEntry ModRMExt_0F_AE_Prefix_F3[] = {
		entry_1(0xAE, ops::RDFSBASE, true, OperandKind::Reg32),
		entry_1(0xAE, ops::RDGSBASE, true, OperandKind::Reg32),
		entry_1(0xAE, ops::WRFSBASE, true, OperandKind::Reg32),
		entry_1(0xAE, ops::WRGSBASE, true, OperandKind::Reg32),

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
		entry_2(0x02, ops::LAR, true, OperandKind::Reg32, OperandKind::RegMem16),
		entry_2(0x03, ops::LSL, true, OperandKind::Reg32, OperandKind::RegMem16),
		entry_none(0x04),
		entry_0(0x05, ops::SYSCALL),
		entry_0(0x06, ops::CLTS),
		entry_0(0x07, ops::SYSRET),

		entry_0(0x08, ops::INVD),
		entry_0(0x09, ops::WBINVD),
		entry_none(0x0A),
		entry_0(0x0B, ops::UD2),
		entry_none(0x0C),
		entry_1(0x0D, ops::PREFETCH, true, OperandKind::Memory),
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
		entry_1(0x19, ops::NOP, true, OperandKind::RegMem32),
		entry_1(0x1A, ops::NOP, true, OperandKind::RegMem32),
		entry_1(0x1B, ops::NOP, true, OperandKind::RegMem32),
		entry_1(0x1C, ops::NOP, true, OperandKind::RegMem32),
		entry_1(0x1D, ops::NOP, true, OperandKind::RegMem32),
		entry_1(0x1E, ops::NOP, true, OperandKind::RegMem32),
		entry_1(0x1F, ops::NOP, true, OperandKind::RegMem32),

		entry_2(0x20, ops::MOV, true, OperandKind::Reg64, OperandKind::ControlReg),
		entry_2(0x21, ops::MOV, true, OperandKind::Reg64, OperandKind::DebugReg),
		entry_2(0x22, ops::MOV, true, OperandKind::ControlReg, OperandKind::Reg64),
		entry_2(0x23, ops::MOV, true, OperandKind::DebugReg, OperandKind::Reg64),
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

		entry_2(0x40, ops::CMOVO,  true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x41, ops::CMOVNO, true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x42, ops::CMOVB,  true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x43, ops::CMOVNB, true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x44, ops::CMOVZ,  true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x45, ops::CMOVNZ, true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x46, ops::CMOVNA, true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x47, ops::CMOVA,  true, OperandKind::Reg32, OperandKind::RegMem32),

		entry_2(0x48, ops::CMOVS,  true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x49, ops::CMOVNS, true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x4A, ops::CMOVP,  true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x4B, ops::CMOVNP, true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x4C, ops::CMOVL,  true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x4D, ops::CMOVGE, true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x4E, ops::CMOVLE, true, OperandKind::Reg32, OperandKind::RegMem32),
		entry_2(0x4F, ops::CMOVG,  true, OperandKind::Reg32, OperandKind::RegMem32),

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
		entry_1(0x80, ops::JO,  false, OperandKind::Rel32Offset),
		entry_1(0x81, ops::JNO, false, OperandKind::Rel32Offset),
		entry_1(0x82, ops::JB,  false, OperandKind::Rel32Offset),
		entry_1(0x83, ops::JNB, false, OperandKind::Rel32Offset),
		entry_1(0x84, ops::JZ,  false, OperandKind::Rel32Offset),
		entry_1(0x85, ops::JNZ, false, OperandKind::Rel32Offset),
		entry_1(0x86, ops::JNA, false, OperandKind::Rel32Offset),
		entry_1(0x87, ops::JA,  false, OperandKind::Rel32Offset),

		entry_1(0x88, ops::JS,  false, OperandKind::Rel32Offset),
		entry_1(0x89, ops::JNS, false, OperandKind::Rel32Offset),
		entry_1(0x8A, ops::JP,  false, OperandKind::Rel32Offset),
		entry_1(0x8B, ops::JNP, false, OperandKind::Rel32Offset),
		entry_1(0x8C, ops::JL,  false, OperandKind::Rel32Offset),
		entry_1(0x8D, ops::JGE, false, OperandKind::Rel32Offset),
		entry_1(0x8E, ops::JLE, false, OperandKind::Rel32Offset),
		entry_1(0x8F, ops::JG,  false, OperandKind::Rel32Offset),

		// same order as Jcc, just Setcc.
		entry_1(0x90, ops::SETO,  true, OperandKind::RegMem8),
		entry_1(0x91, ops::SETNO, true, OperandKind::RegMem8),
		entry_1(0x92, ops::SETB,  true, OperandKind::RegMem8),
		entry_1(0x93, ops::SETNB, true, OperandKind::RegMem8),
		entry_1(0x94, ops::SETZ,  true, OperandKind::RegMem8),
		entry_1(0x95, ops::SETNZ, true, OperandKind::RegMem8),
		entry_1(0x96, ops::SETNA, true, OperandKind::RegMem8),
		entry_1(0x97, ops::SETA,  true, OperandKind::RegMem8),

		entry_1(0x98, ops::SETS,  true, OperandKind::RegMem8),
		entry_1(0x99, ops::SETNS, true, OperandKind::RegMem8),
		entry_1(0x9A, ops::SETP,  true, OperandKind::RegMem8),
		entry_1(0x9B, ops::SETNP, true, OperandKind::RegMem8),
		entry_1(0x9C, ops::SETL,  true, OperandKind::RegMem8),
		entry_1(0x9D, ops::SETGE, true, OperandKind::RegMem8),
		entry_1(0x9E, ops::SETLE, true, OperandKind::RegMem8),
		entry_1(0x9F, ops::SETG,  true, OperandKind::RegMem8),

		entry_1(0xA0, ops::PUSH, false, OperandKind::ImplicitFS),
		entry_1(0xA1, ops::POP,  false, OperandKind::ImplicitFS),
		entry_0(0xA2, ops::CPUID),
		entry_2(0xA3, ops::BT,   true, OperandKind::RegMem32, OperandKind::Reg32),
		entry_3(0xA4, ops::SHLD, true, OperandKind::RegMem32, OperandKind::Reg32, OperandKind::Imm8),
		entry_3(0xA5, ops::SHLD, true, OperandKind::RegMem32, OperandKind::Reg32, OperandKind::ImplicitCL),
		entry_none(0xA6),
		entry_none(0xA7),

		entry_1(0xA8, ops::PUSH, false, OperandKind::ImplicitGS),
		entry_1(0xA9, ops::POP,  false, OperandKind::ImplicitGS),
		entry_0(0xAA, ops::RSM),
		entry_2(0xAB, ops::BTS,  true, OperandKind::RegMem32, OperandKind::Reg32),
		entry_3(0xAC, ops::SHLD, true, OperandKind::RegMem32, OperandKind::Reg32, OperandKind::Imm8),
		entry_3(0xAD, ops::SHLD, true, OperandKind::RegMem32, OperandKind::Reg32, OperandKind::ImplicitCL),
		entry_none(0xAE),
		entry_2(0xAF, ops::IMUL, true, OperandKind::Reg32, OperandKind::RegMem32),

		entry_2(0xB0, ops::CMPXCHG, true, OperandKind::RegMem8, OperandKind::Reg8),
		entry_2(0xB1, ops::CMPXCHG, true, OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0xB2, ops::LSS,     true, OperandKind::Reg32, OperandKind::Ptr16_32),
		entry_2(0xB3, ops::BTR,     true, OperandKind::RegMem32, OperandKind::Reg32),
		entry_2(0xB4, ops::LFS,     true, OperandKind::Reg32, OperandKind::Ptr16_32),
		entry_2(0xB5, ops::LGS,     true, OperandKind::Reg32, OperandKind::Ptr16_32),
		entry_2(0xB6, ops::MOVZX,   true, OperandKind::Reg32, OperandKind::RegMem8),
		entry_2(0xB7, ops::MOVZX,   true, OperandKind::Reg32, OperandKind::RegMem16),

		// opcodes 0xB8 to 0xC7 use prefixes
		entry_none(0xB8), entry_none(0xB9), entry_none(0xBA), entry_none(0xBB),
		entry_none(0xBC), entry_none(0xBD), entry_none(0xBE), entry_none(0xBF),
		entry_none(0xC0), entry_none(0xC1), entry_none(0xC2), entry_none(0xC3),
		entry_none(0xC4), entry_none(0xC5), entry_none(0xC6), entry_none(0xC7),

		entry_lnri_1(0xC8, ops::BSWAP, OperandKind::RegMem32),
		entry_lnri_1(0xC9, ops::BSWAP, OperandKind::RegMem32),
		entry_lnri_1(0xCA, ops::BSWAP, OperandKind::RegMem32),
		entry_lnri_1(0xCB, ops::BSWAP, OperandKind::RegMem32),
		entry_lnri_1(0xCC, ops::BSWAP, OperandKind::RegMem32),
		entry_lnri_1(0xCD, ops::BSWAP, OperandKind::RegMem32),
		entry_lnri_1(0xCE, ops::BSWAP, OperandKind::RegMem32),
		entry_lnri_1(0xCF, ops::BSWAP, OperandKind::RegMem32),

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

	// i'm lazy to support this right now.
	constexpr TableEntry SecondaryOpcodeMap_F0_Prefix_None[] = {
		entry_none(0x00), entry_none(0x01), entry_none(0x02), entry_none(0x03),
		entry_none(0x04), entry_none(0x05), entry_none(0x06), entry_none(0x07),
		entry_none(0x08), entry_none(0x09), entry_none(0x0A), entry_none(0x0B),
		entry_none(0x0C), entry_none(0x0D), entry_none(0x0E), entry_none(0x0F),
		entry_none(0x10), entry_none(0x11), entry_none(0x12), entry_none(0x13),
		entry_none(0x14), entry_none(0x15), entry_none(0x16), entry_none(0x17),
		entry_none(0x18), entry_none(0x19), entry_none(0x1A), entry_none(0x1B),
		entry_none(0x1C), entry_none(0x1D), entry_none(0x1E), entry_none(0x1F),
		entry_none(0x20), entry_none(0x21), entry_none(0x22), entry_none(0x23),
		entry_none(0x24), entry_none(0x25), entry_none(0x26), entry_none(0x27),
		entry_none(0x28), entry_none(0x29), entry_none(0x2A), entry_none(0x2B),
		entry_none(0x2C), entry_none(0x2D), entry_none(0x2E), entry_none(0x2F),
		entry_none(0x30), entry_none(0x31), entry_none(0x32), entry_none(0x33),
		entry_none(0x34), entry_none(0x35), entry_none(0x36), entry_none(0x37),
		entry_none(0x38), entry_none(0x39), entry_none(0x3A), entry_none(0x3B),
		entry_none(0x3C), entry_none(0x3D), entry_none(0x3E), entry_none(0x3F),
		entry_none(0x40), entry_none(0x41), entry_none(0x42), entry_none(0x43),
		entry_none(0x44), entry_none(0x45), entry_none(0x46), entry_none(0x47),
		entry_none(0x48), entry_none(0x49), entry_none(0x4A), entry_none(0x4B),
		entry_none(0x4C), entry_none(0x4D), entry_none(0x4E), entry_none(0x4F),
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
		entry_none(0xAC), entry_none(0xAD),
		entry_ext(0xAE, &ModRMExt_0F_AE_Prefix_None[0]),
		entry_none(0xAF),
		entry_none(0xB0), entry_none(0xB1), entry_none(0xB2), entry_none(0xB3),
		entry_none(0xB4), entry_none(0xB5), entry_none(0xB6), entry_none(0xB7),
		entry_none(0xB8), entry_none(0xB9), entry_none(0xBA), entry_none(0xBB),
		entry_none(0xBC), entry_none(0xBD), entry_none(0xBE), entry_none(0xBF),
		entry_none(0xC0), entry_none(0xC1), entry_none(0xC2), entry_none(0xC3),
		entry_none(0xC4), entry_none(0xC5), entry_none(0xC6), entry_none(0xC7),
		entry_none(0xC8), entry_none(0xC9), entry_none(0xCA), entry_none(0xCB),
		entry_none(0xCC), entry_none(0xCD), entry_none(0xCE), entry_none(0xCF),
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



	constexpr TableEntry SecondaryOpcodeMap_F0_Prefix_66[] = {
		entry_none(0x00), entry_none(0x01), entry_none(0x02), entry_none(0x03),
		entry_none(0x04), entry_none(0x05), entry_none(0x06), entry_none(0x07),
		entry_none(0x08), entry_none(0x09), entry_none(0x0A), entry_none(0x0B),
		entry_none(0x0C), entry_none(0x0D), entry_none(0x0E), entry_none(0x0F),
		entry_none(0x10), entry_none(0x11), entry_none(0x12), entry_none(0x13),
		entry_none(0x14), entry_none(0x15), entry_none(0x16), entry_none(0x17),
		entry_none(0x18), entry_none(0x19), entry_none(0x1A), entry_none(0x1B),
		entry_none(0x1C), entry_none(0x1D), entry_none(0x1E), entry_none(0x1F),
		entry_none(0x20), entry_none(0x21), entry_none(0x22), entry_none(0x23),
		entry_none(0x24), entry_none(0x25), entry_none(0x26), entry_none(0x27),
		entry_none(0x28), entry_none(0x29), entry_none(0x2A), entry_none(0x2B),
		entry_none(0x2C), entry_none(0x2D), entry_none(0x2E), entry_none(0x2F),
		entry_none(0x30), entry_none(0x31), entry_none(0x32), entry_none(0x33),
		entry_none(0x34), entry_none(0x35), entry_none(0x36), entry_none(0x37),
		entry_none(0x38), entry_none(0x39), entry_none(0x3A), entry_none(0x3B),
		entry_none(0x3C), entry_none(0x3D), entry_none(0x3E), entry_none(0x3F),
		entry_none(0x40), entry_none(0x41), entry_none(0x42), entry_none(0x43),
		entry_none(0x44), entry_none(0x45), entry_none(0x46), entry_none(0x47),
		entry_none(0x48), entry_none(0x49), entry_none(0x4A), entry_none(0x4B),
		entry_none(0x4C), entry_none(0x4D), entry_none(0x4E), entry_none(0x4F),
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
		entry_none(0xB0), entry_none(0xB1), entry_none(0xB2), entry_none(0xB3),
		entry_none(0xB4), entry_none(0xB5), entry_none(0xB6), entry_none(0xB7),
		entry_none(0xB8), entry_none(0xB9), entry_none(0xBA), entry_none(0xBB),
		entry_none(0xBC), entry_none(0xBD), entry_none(0xBE), entry_none(0xBF),
		entry_none(0xC0), entry_none(0xC1), entry_none(0xC2), entry_none(0xC3),
		entry_none(0xC4), entry_none(0xC5), entry_none(0xC6), entry_none(0xC7),
		entry_none(0xC8), entry_none(0xC9), entry_none(0xCA), entry_none(0xCB),
		entry_none(0xCC), entry_none(0xCD), entry_none(0xCE), entry_none(0xCF),
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


	constexpr TableEntry SecondaryOpcodeMap_F0_Prefix_F2[] = {
		entry_none(0x00), entry_none(0x01), entry_none(0x02), entry_none(0x03),
		entry_none(0x04), entry_none(0x05), entry_none(0x06), entry_none(0x07),
		entry_none(0x08), entry_none(0x09), entry_none(0x0A), entry_none(0x0B),
		entry_none(0x0C), entry_none(0x0D), entry_none(0x0E), entry_none(0x0F),
		entry_none(0x10), entry_none(0x11), entry_none(0x12), entry_none(0x13),
		entry_none(0x14), entry_none(0x15), entry_none(0x16), entry_none(0x17),
		entry_none(0x18), entry_none(0x19), entry_none(0x1A), entry_none(0x1B),
		entry_none(0x1C), entry_none(0x1D), entry_none(0x1E), entry_none(0x1F),
		entry_none(0x20), entry_none(0x21), entry_none(0x22), entry_none(0x23),
		entry_none(0x24), entry_none(0x25), entry_none(0x26), entry_none(0x27),
		entry_none(0x28), entry_none(0x29), entry_none(0x2A), entry_none(0x2B),
		entry_none(0x2C), entry_none(0x2D), entry_none(0x2E), entry_none(0x2F),
		entry_none(0x30), entry_none(0x31), entry_none(0x32), entry_none(0x33),
		entry_none(0x34), entry_none(0x35), entry_none(0x36), entry_none(0x37),
		entry_none(0x38), entry_none(0x39), entry_none(0x3A), entry_none(0x3B),
		entry_none(0x3C), entry_none(0x3D), entry_none(0x3E), entry_none(0x3F),
		entry_none(0x40), entry_none(0x41), entry_none(0x42), entry_none(0x43),
		entry_none(0x44), entry_none(0x45), entry_none(0x46), entry_none(0x47),
		entry_none(0x48), entry_none(0x49), entry_none(0x4A), entry_none(0x4B),
		entry_none(0x4C), entry_none(0x4D), entry_none(0x4E), entry_none(0x4F),
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
		entry_none(0xB0), entry_none(0xB1), entry_none(0xB2), entry_none(0xB3),
		entry_none(0xB4), entry_none(0xB5), entry_none(0xB6), entry_none(0xB7),
		entry_none(0xB8), entry_none(0xB9), entry_none(0xBA), entry_none(0xBB),
		entry_none(0xBC), entry_none(0xBD), entry_none(0xBE), entry_none(0xBF),
		entry_none(0xC0), entry_none(0xC1), entry_none(0xC2), entry_none(0xC3),
		entry_none(0xC4), entry_none(0xC5), entry_none(0xC6), entry_none(0xC7),
		entry_none(0xC8), entry_none(0xC9), entry_none(0xCA), entry_none(0xCB),
		entry_none(0xCC), entry_none(0xCD), entry_none(0xCE), entry_none(0xCF),
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


	constexpr TableEntry SecondaryOpcodeMap_F0_Prefix_F3[] = {
		entry_none(0x00), entry_none(0x01), entry_none(0x02), entry_none(0x03),
		entry_none(0x04), entry_none(0x05), entry_none(0x06), entry_none(0x07),
		entry_none(0x08), entry_none(0x09), entry_none(0x0A), entry_none(0x0B),
		entry_none(0x0C), entry_none(0x0D), entry_none(0x0E), entry_none(0x0F),
		entry_none(0x10), entry_none(0x11), entry_none(0x12), entry_none(0x13),
		entry_none(0x14), entry_none(0x15), entry_none(0x16), entry_none(0x17),
		entry_none(0x18), entry_none(0x19), entry_none(0x1A), entry_none(0x1B),
		entry_none(0x1C), entry_none(0x1D), entry_none(0x1E), entry_none(0x1F),
		entry_none(0x20), entry_none(0x21), entry_none(0x22), entry_none(0x23),
		entry_none(0x24), entry_none(0x25), entry_none(0x26), entry_none(0x27),
		entry_none(0x28), entry_none(0x29), entry_none(0x2A), entry_none(0x2B),
		entry_none(0x2C), entry_none(0x2D), entry_none(0x2E), entry_none(0x2F),
		entry_none(0x30), entry_none(0x31), entry_none(0x32), entry_none(0x33),
		entry_none(0x34), entry_none(0x35), entry_none(0x36), entry_none(0x37),
		entry_none(0x38), entry_none(0x39), entry_none(0x3A), entry_none(0x3B),
		entry_none(0x3C), entry_none(0x3D), entry_none(0x3E), entry_none(0x3F),
		entry_none(0x40), entry_none(0x41), entry_none(0x42), entry_none(0x43),
		entry_none(0x44), entry_none(0x45), entry_none(0x46), entry_none(0x47),
		entry_none(0x48), entry_none(0x49), entry_none(0x4A), entry_none(0x4B),
		entry_none(0x4C), entry_none(0x4D), entry_none(0x4E), entry_none(0x4F),
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
		entry_none(0xAC), entry_none(0xAD),

		entry_ext(0xAE, &ModRMExt_0F_AE_Prefix_F3[0]),

		entry_none(0xAF),
		entry_none(0xB0), entry_none(0xB1), entry_none(0xB2), entry_none(0xB3),
		entry_none(0xB4), entry_none(0xB5), entry_none(0xB6), entry_none(0xB7),
		entry_none(0xB8), entry_none(0xB9), entry_none(0xBA), entry_none(0xBB),
		entry_none(0xBC), entry_none(0xBD), entry_none(0xBE), entry_none(0xBF),
		entry_none(0xC0), entry_none(0xC1), entry_none(0xC2), entry_none(0xC3),
		entry_none(0xC4), entry_none(0xC5), entry_none(0xC6), entry_none(0xC7),
		entry_none(0xC8), entry_none(0xC9), entry_none(0xCA), entry_none(0xCB),
		entry_none(0xCC), entry_none(0xCD), entry_none(0xCE), entry_none(0xCF),
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

	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Normal) == 256, "table invalid");
	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Prefix_None) == 256, "table invalid");
	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Prefix_66) == 256, "table invalid");
	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Prefix_F2) == 256, "table invalid");
	static_assert(ArrayLength(SecondaryOpcodeMap_F0_Prefix_F3) == 256, "table invalid");


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
