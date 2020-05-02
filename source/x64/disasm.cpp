// disasm.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include "disasm.h"
#include "x64/x64.h"

#include <stdio.h>

namespace instrad::x64
{
	Register getSegmentOfOverride(int override)
	{
		switch(override)
		{
			case InstrModifiers::SEG_CS: return regs::CS;
			case InstrModifiers::SEG_DS: return regs::DS;
			case InstrModifiers::SEG_ES: return regs::ES;
			case InstrModifiers::SEG_FS: return regs::FS;
			case InstrModifiers::SEG_GS: return regs::GS;
			case InstrModifiers::SEG_SS: return regs::SS;

			default: return regs::NONE;
		}
	}

	Operand getOperand(Buffer& buf, OpKind kind, InstrModifiers& mods)
	{
		// TODO: we should not promote Mem16/RegMem16/Reg16!!
		switch(kind)
		{
			// byte-sized things are not promoted
			case OpKind::Reg8:
				return getRegisterOperand(buf, /* byte; */ true, mods);

			case OpKind::Reg64: mods.default64BitRegister = true; [[fallthrough]];
			case OpKind::Reg16:
			case OpKind::Reg32:
				return getRegisterOperand(buf, /* byte: */ false, mods);

			case OpKind::Reg8_Rm:
				return getRegisterOperandFromModRM(buf, /* byte: */ true, mods);

			case OpKind::Reg64_Rm: mods.default64BitRegister = true; [[fallthrough]];
			case OpKind::Reg16_Rm:
			case OpKind::Reg32_Rm:
				return getRegisterOperandFromModRM(buf, /* byte: */ false, mods);


			case OpKind::RegMem8:
				return getRegisterOrMemoryOperand(buf, /* byte; */ true, mods);

			case OpKind::RegMem64: mods.default64BitRegister = true; [[fallthrough]];
			case OpKind::RegMem16:
			case OpKind::RegMem32:
				return getRegisterOrMemoryOperand(buf, /* byte; */ false, mods);


			case OpKind::Mem8:
			case OpKind::Mem16:
			case OpKind::Mem32:
			case OpKind::Mem64:
				return getMemoryOperand(buf, /* byte: */ kind == OpKind::Mem8, mods);

			case OpKind::Mem128:    return getMemoryOperand(buf, /* byte: */ false, mods, /* bits: */ 128);
			case OpKind::Mem256:    return getMemoryOperand(buf, /* byte: */ false, mods, /* bits: */ 256);


			case OpKind::Imm8:
				return readSignedImm8(buf);

			case OpKind::SignExtImm8: {
				if(mods.operandSizeOverride)
					return (int16_t) readSignedImm8(buf);

				else if(mods.rex.W)
					return (int64_t) readSignedImm8(buf);

				else
					return (int32_t) readSignedImm8(buf);
			}

			case OpKind::Imm16:
			case OpKind::Imm32:
			case OpKind::Imm64: {
				// need to promote/demote
				if(mods.operandSizeOverride)
					return readSignedImm16(buf);

				else if(kind == OpKind::Imm64 && mods.rex.W)
					return readSignedImm64(buf);

				else if(mods.rex.W)
					return (int64_t) readSignedImm32(buf);

				else
					return readSignedImm32(buf);
			}

			case OpKind::SignExtImm32:
				return (int64_t) readSignedImm32(buf);

			// TODO: need to handle relative offsets properly
			case OpKind::Rel8Offset:
				return readSignedImm8(buf);

			case OpKind::Rel16Offset:
			case OpKind::Rel32Offset: {
				if(mods.operandSizeOverride)
					return readSignedImm16(buf);

				else
					return readSignedImm32(buf);
			}

			case OpKind::Memory:
				return getMemoryOperand(buf, /* byte: */ false, mods);

			case OpKind::ImplicitCS:   return regs::CS;
			case OpKind::ImplicitDS:   return regs::DS;
			case OpKind::ImplicitES:   return regs::ES;
			case OpKind::ImplicitFS:   return regs::FS;
			case OpKind::ImplicitGS:   return regs::GS;
			case OpKind::ImplicitSS:   return regs::SS;
			case OpKind::ImplicitCL:   return regs::CL;
			case OpKind::ImplicitCX:   return regs::CX;
			case OpKind::ImplicitECX:  return regs::ECX;
			case OpKind::ImplicitDX:   return regs::DX;

			case OpKind::ImplicitAL:   return regs::AL;

			case OpKind::ImplicitAX:
			case OpKind::ImplicitEAX:
			case OpKind::ImplicitRAX: {
				if(mods.operandSizeOverride)
					return regs::AX;
				else if(mods.rex.W)
					return regs::RAX;
				else
					return regs::EAX;
			}


			case OpKind::SegmentReg:   return getSegmentRegister(mods);
			case OpKind::ControlReg:   return getControlRegister(mods);
			case OpKind::DebugReg:     return getDebugRegister(mods);

			case OpKind::MemoryOfs8:
			case OpKind::MemoryOfs16:
			case OpKind::MemoryOfs32:
			case OpKind::MemoryOfs64: {
				// kekw
				int bits = 8 * (1 << ((int) kind - (int) OpKind::MemoryOfs8));

				auto seg = getSegmentOfOverride(mods.segmentOverride);
				if(mods.compatibilityMode)
					return MemoryRef(bits, (uint32_t) readSignedImm32(buf)).setSegment(seg);

				else
					return MemoryRef(bits, (uint64_t) readSignedImm64(buf)).setSegment(seg);
			}

			// this is damn dumb
			case OpKind::Reg32Mem16: {
				auto ret = getRegisterOrMemoryOperand(buf, /* byte: */ false, mods);
				if(ret.isMemory())
					ret.mem().setBits(16);

				return ret;
			}


			case OpKind::RegMmx_Rm:
			case OpKind::RegXmm_Rm:

			case OpKind::RegMmx:
			case OpKind::RegMmxMem32:
			case OpKind::RegMmxMem64:

			case OpKind::RegXmm:
			case OpKind::RegXmmMem32:
			case OpKind::RegXmmMem64:
			case OpKind::RegXmmMem128:

			case OpKind::RegYmm:
			case OpKind::RegYmmMem256:


			case OpKind::Ptr16_16:
			case OpKind::Ptr16_32:
				// not supported
				printf("unsupported operand!\n");
				return regs::R15;

			case OpKind::None:
				return { };
		}
	}


	static Instruction decode(Buffer& xs, InstrModifiers& mods, const TableEntry* mainTable, const TableEntry* prefixTable)
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

				if(modrm.mod != 3)
				{
					entry = ext[0];
				}
				else
				{
					if(entry.extensionUsesRMBits())
						entry = ext[1 + modrm.rm];

					else
						entry = ext[1];
				}
			}
			else if(entry.extensionUsesRexWBit())
			{
				// if !rex.W, then use the first entry; if rex.W, use the second.
				if(mods.rex.W)  entry = ext[1];
				else            entry = ext[0];
			}
			else
			{
				entry = ext[modrm.reg];
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


	// this is mostly a state machine; see figure 1-1 in the AMD manual, volume 3.
	Instruction read(Buffer& xs)
	{
		auto begin = xs.ptr();

		InstrModifiers modifiers;

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
		if((xs.peek() & 0xF0) == 0x40)
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



#if 0

auto opcode = xs.peek();
		if(opcode == 0x00)
		{
			// ADD reg/mem8, reg8
			xs.pop();

			auto op = ops::ADD;
			mods.modrm = ModRM(xs.pop());

			auto dst = getRegisterOrMemoryFromModRM(xs, /* isByteMode: */ true, mods);
			auto src = getRegisterFromModRM(xs, /* isByteMode: */ true, mods);

			auto instr = Instruction(op);
			instr.setSrc(src);
			instr.setDst(dst);

			return instr;
		}
		else if(opcode == 0x01)
		{
			// ADD reg/mem16, reg16
			// ADD reg/mem32, reg32
			// ADD reg/mem64, reg64

			xs.pop();

			auto op = ops::ADD;
			mods.modrm = ModRM(xs.pop());

			auto dst = getRegisterOrMemoryFromModRM(xs, /* isByteMode: */ false, mods);
			auto src = getRegisterFromModRM(xs, /* isByteMode: */ false, mods);

			auto instr = Instruction(op);
			instr.setSrc(src);
			instr.setDst(dst);

			return instr;
		}
		else if(opcode == 0x02)
		{
			// ADD reg8, reg/mem8
			xs.pop();

			auto op = ops::ADD;
			mods.modrm = ModRM(xs.pop());

			auto dst = getRegisterFromModRM(xs, /* isByteMode: */ true, mods);
			auto src = getRegisterOrMemoryFromModRM(xs, /* isByteMode: */ true, mods);

			auto instr = Instruction(op);
			instr.setSrc(src);
			instr.setDst(dst);

			return instr;
		}
		else if(opcode == 0x03)
		{
			// ADD reg16, reg/mem16
			// ADD reg32, reg/mem32
			// ADD reg64, reg/mem64

			xs.pop();

			auto op = ops::ADD;
			mods.modrm = ModRM(xs.pop());

			auto dst = getRegisterFromModRM(xs, /* isByteMode: */ false, mods);
			auto src = getRegisterOrMemoryFromModRM(xs, /* isByteMode: */ false, mods);

			auto instr = Instruction(op);
			instr.setSrc(src);
			instr.setDst(dst);

			return instr;
		}

		while(true);
#endif
