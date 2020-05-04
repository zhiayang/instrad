// decode.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "ops.h"
#include "regs.h"
#include "table.h"
#include "operands.h"

namespace instrad::x64
{
	struct Instruction
	{
		constexpr Instruction(Op op) : m_op(op) { }

		constexpr void setDst(const Operand& x) { this->m_dst = x; this->m_operandCount = 1; }
		constexpr void setSrc(const Operand& x) { this->m_src = x; this->m_operandCount = 2; }
		constexpr void setExt(const Operand& x) { this->m_ext = x; this->m_operandCount = 3; }
		constexpr void setBytes(const uint8_t* bytes, size_t n) { this->m_bytes = bytes; this->m_byteCount = n; }

		constexpr void addLockPrefix() { this->m_prefixLock = true; }
		constexpr void addRepPrefix() { this->m_prefixRep = true; }
		constexpr void addRepNZPrefix() { this->m_prefixRepnz = true; }

		constexpr bool lockPrefix() const { return this->m_prefixLock; }
		constexpr bool repPrefix() const { return this->m_prefixRep; }
		constexpr bool repnzPrefix() const { return this->m_prefixRepnz; }


		constexpr const Operand& src() const { return this->m_src; }
		constexpr const Operand& dst() const { return this->m_dst; }
		constexpr const Operand& ext() const { return this->m_ext; }
		constexpr const Op& op() const { return this->m_op; }

		constexpr int operandCount() const { return this->m_operandCount; }

		constexpr const uint8_t* bytes() const { return this->m_bytes; }
		constexpr size_t numBytes() const { return this->m_byteCount; }

	private:
		Op m_op;
		int m_operandCount = 0;

		bool m_prefixLock  = false;
		bool m_prefixRep   = false;
		bool m_prefixRepnz = false;

		// in "mov rax, $1", rax=dst, $1=src; ie. "mov $1, %rax"
		// for single-op instructions, eg. inc %rax, it is stored in dst.

		Operand m_dst = { };
		Operand m_src = { };
		Operand m_ext = { };

		const uint8_t* m_bytes = 0;
		size_t m_byteCount = 0;
	};

	constexpr Instruction decode(Buffer& xs, InstrModifiers& mods, const TableEntry* mainTable, const TableEntry* prefixTable)
	{
		// check the main table first
		mods.opcode = xs.pop();
		auto entry = mainTable[mods.opcode];

		// if there's no main entry but there's a prefix entry, then use that instead.
		if(!entry.present() && prefixTable)
			entry = prefixTable[mods.opcode];

		// make a potential modRM
		bool usedModRM = false;

		// chase down the tables.
		while(!entry.present())
		{
			auto ext = entry.extension();
			if(ext == nullptr)
				break;

			usedModRM = true;
			auto modrm = ModRM(xs.peek());

			// see what kind
			if(entry.extensionUsesRMBits() || entry.extensionUsesModBits())
			{
				// ugh. we *already* indexed by modRM.reg, so we only need to index by modRM.rm now.
				// BUT, only if modRM.mod == 3; if modRM.mod != 3, then we only use the first entry.
				// there's a second class of entries that only index by modRM.mod, and disregards modRM.rm;
				// check for that as well.

				if(modrm.mod() != 3)
				{
					entry = ext[0];
				}
				else
				{
					if(entry.extensionUsesRMBits())
						entry = ext[1 + modrm.rm()];

					else
						entry = ext[1];
				}
			}
			else if(entry.extensionUsesRexWBit())
			{
				// if !rex.W, then use the first entry; if rex.W, use the second.
				if(mods.rex.W())    entry = ext[1];
				else                entry = ext[0];
			}
			else
			{
				entry = ext[modrm.reg()];
			}
		}

		// entry is not present; cry.
		if(!entry.present())
			return Instruction(ops::INVALID);

		mods.directRegisterIndex = entry.isDirectRegisterIdx();

		// if we need a modrm, then consume it for real.
		if(usedModRM || entry.needsModRM())
			mods.modrm = ModRM(xs.pop());

		// handle NOP specially.
		if(mods.opcode == 0x90)
		{
			// don't forget pause.
			if(mods.repPrefix)  return Instruction(ops::PAUSE);
			else                return Instruction(ops::NOP);
		}
		else
		{
			auto ret = Instruction(entry.op());
			if(entry.numOperands() > 0)
				ret.setDst(getOperand(xs, entry.operands()[0], mods));

			if(entry.numOperands() > 1)
				ret.setSrc(getOperand(xs, entry.operands()[1], mods));

			if(entry.numOperands() > 2)
				ret.setExt(getOperand(xs, entry.operands()[2], mods));

			return ret;
		}
	}

	enum class ExecMode
	{
		Legacy,
		Compat,
		Long
	};

	// this is mostly a state machine; see figure 1-1 in the AMD manual, volume 3.
	constexpr Instruction read(Buffer& xs, ExecMode mode)
	{
		auto begin = xs.ptr();

		auto modifiers = InstrModifiers();
		switch(mode)
		{
			case ExecMode::Legacy:
				modifiers.legacyAddressingMode = true;
				break;

			case ExecMode::Compat:
				modifiers.compatibilityMode = true;
				break;

			case ExecMode::Long:
				break;
		}

		// first, check for legacy prefixes
		while(true)
		{
			if(xs.match(0x66))      modifiers.operandSizeOverride = true;
			else if(xs.match(0x67)) modifiers.addressSizeOverride = true;
			else if(xs.match(0x2E)) modifiers.segmentOverride = InstrModifiers::SEG_CS;
			else if(xs.match(0x3E)) modifiers.segmentOverride = InstrModifiers::SEG_DS;
			else if(xs.match(0x26)) modifiers.segmentOverride = InstrModifiers::SEG_ES;
			else if(xs.match(0x64)) modifiers.segmentOverride = InstrModifiers::SEG_FS;
			else if(xs.match(0x65)) modifiers.segmentOverride = InstrModifiers::SEG_GS;
			else if(xs.match(0x36)) modifiers.segmentOverride = InstrModifiers::SEG_SS;
			else if(xs.match(0xF0)) modifiers.lockPrefix = true;
			else if(xs.match(0xF3)) modifiers.repnzPrefix = false, modifiers.repPrefix = true;
			else if(xs.match(0xF2)) modifiers.repnzPrefix = true, modifiers.repPrefix = false;
			else                    break;
		}

		// next, REX prefix.
		if(mode == ExecMode::Long && (xs.peek() & 0xF0) == 0x40)
			modifiers.rex = RexPrefix(xs.pop());

		auto table = PrimaryOpcodeMap;
		const TableEntry* prefixedTable = nullptr;

		// next, check for escape
		if(xs.match(0x0F))
		{
			if(xs.match(0x0F))
			{
				// 3dnow
			}
			else if(xs.match(0x38))
			{
				// sse
			}
			else if(xs.match(0x3A))
			{
				// also sse
			}
			else
			{
				table = SecondaryOpcodeMap_F0_Normal;

				// check what kind of prefixes we have.
				if(modifiers.repnzPrefix)
					prefixedTable = SecondaryOpcodeMap_F0_Prefix_F2;

				else if(modifiers.repPrefix)
					prefixedTable = SecondaryOpcodeMap_F0_Prefix_F3;

				else if(modifiers.operandSizeOverride)
					prefixedTable = SecondaryOpcodeMap_F0_Prefix_66;

				else
					prefixedTable = SecondaryOpcodeMap_F0_Prefix_None;
			}
		}

		auto ret = decode(xs, modifiers, table, prefixedTable);
		auto len = xs.ptr() - begin;

		ret.setBytes(begin, len);

		if(modifiers.lockPrefix)    ret.addLockPrefix();
		if(modifiers.repPrefix)     ret.addRepPrefix();
		if(modifiers.repnzPrefix)   ret.addRepNZPrefix();

		return ret;
	}
}
