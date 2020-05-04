// operands.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "x64.h"
#include "immediates.h"

namespace instrad::x64
{
	enum class RegKind
	{
		GPR,
		Segment,
		Control,
		Debug,
		Vector,
	};

	constexpr Register decodeRegisterNumber(size_t bits, const InstrModifiers& mods, int index, RegKind rk)
	{
		// note: the regs::getX functions do the error checking for us,
		// and returns INVALID if the index is out of bounds
		if(rk == RegKind::GPR)
		{
			if(bits == 8)
			{
				// if the REX is present, then we should use this set of registers:
				if(mods.rex.present())
					return regs::get8Bit(index);

				else
					return regs::get8BitLegacy(index);
			}
			else
			{
				// if REX.W, then we have a 64-bit operand. this has precedence over
				// the 0x66 operand size prefix, so check for this first.
				if(mods.rex.W() || bits == 64)
					return regs::get64Bit(index);

				else if(mods.operandSizeOverride)
					return regs::get16Bit(index);

				else
					return regs::get32Bit(index);
			}
		}
		else if(rk == RegKind::Segment)
		{
			return regs::getSegment(index);
		}
		else if(rk == RegKind::Control)
		{
			return regs::getControl(index);
		}
		else if(rk == RegKind::Debug)
		{
			return regs::getDebug(index);
		}
		else
		{
			switch(bits)
			{
				case 64:    return regs::getMMX(index);
				case 128:   return regs::getXMM(index);
			}
		}

		return regs::INVALID;
	}


	constexpr MemoryRef decodeSIB(Buffer& buf, const InstrModifiers& mods, bool* didReadDisplacement)
	{
		union {
			struct {
				uint8_t base  : 3;
				uint8_t index : 3;
				uint8_t scale : 2;
			};

			uint8_t byte = 0;
		} sib;

		sib.byte = buf.pop();

		auto mem = MemoryRef();

		if(sib.base == 5)
		{
			if(mods.modrm.mod() == 0)
			{
				mem.setBase(regs::NONE);
				mem.setDisplacement(readSignedImm32(buf));
				*didReadDisplacement = true;
			}
			else if(mods.modrm.mod() == 1)
			{
				mem.setBase(mods.rex.B() ? regs::R13 : regs::RBP);
				mem.setDisplacement(readSignedImm8(buf));
				*didReadDisplacement = true;
			}
			else if(mods.modrm.mod() == 2)
			{
				mem.setBase(mods.rex.B() ? regs::R13 : regs::RBP);
				mem.setDisplacement(readSignedImm32(buf));
				*didReadDisplacement = true;
			}
			else
			{
				mem.setBase(regs::INVALID);
			}
		}
		else
		{
			switch(sib.base)
			{
				case 0: mem.setBase(mods.rex.B() ? regs::R8  : regs::RAX); break;
				case 1: mem.setBase(mods.rex.B() ? regs::R9  : regs::RCX); break;
				case 2: mem.setBase(mods.rex.B() ? regs::R10 : regs::RDX); break;
				case 3: mem.setBase(mods.rex.B() ? regs::R11 : regs::RBX); break;
				case 4: mem.setBase(mods.rex.B() ? regs::R12 : regs::RSP); break;
				case 6: mem.setBase(mods.rex.B() ? regs::R14 : regs::RSI); break;
				case 7: mem.setBase(mods.rex.B() ? regs::R15 : regs::RDI); break;

				default: mem.setBase(regs::INVALID); break;
			}
		}

		switch(sib.index)
		{
			case 0: mem.setIndex(mods.rex.X() ? regs::R8  : regs::RAX);  break;
			case 1: mem.setIndex(mods.rex.X() ? regs::R9  : regs::RCX);  break;
			case 2: mem.setIndex(mods.rex.X() ? regs::R10 : regs::RDX);  break;
			case 3: mem.setIndex(mods.rex.X() ? regs::R11 : regs::RBX);  break;
			case 4: mem.setIndex(mods.rex.X() ? regs::R12 : regs::NONE); break;   // apparently you can't use RSP as an index
			case 5: mem.setIndex(mods.rex.X() ? regs::R13 : regs::RBP);  break;
			case 6: mem.setIndex(mods.rex.X() ? regs::R14 : regs::RSI);  break;
			case 7: mem.setIndex(mods.rex.X() ? regs::R15 : regs::RDI);  break;

			default: mem.setIndex(regs::INVALID); break;
		}

		mem.setScale(1 << sib.scale);
		return mem;
	}


	constexpr Register getRegisterOperandFromModRM(size_t bits, const InstrModifiers& mods, RegKind rk)
	{
		return decodeRegisterNumber(bits, mods, mods.modrm.rm() | (mods.rex.B() << 3), rk);
	}

	constexpr Register getRegisterOperand(size_t bits, const InstrModifiers& mods, RegKind rk)
	{
		if(mods.directRegisterIndex)
			return decodeRegisterNumber(bits, mods, (mods.opcode & 0x07) | (mods.rex.B() << 3), rk);

		else
			return decodeRegisterNumber(bits, mods, mods.modrm.reg() | (mods.rex.R() << 3), rk);
	}

	constexpr Register getSegmentOfOverride(int override)
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

	constexpr MemoryRef getMemoryOperand(Buffer& buf, size_t bits, const InstrModifiers& mods)
	{
		// we need to handle "promotion".
		if(bits == 32 && mods.rex.W())
			bits = 64;

		else if(bits == 32 && mods.operandSizeOverride)
			bits = 16;

		// lazy to refactor this, so just wrap it in a lambda so we can extract the return value

		auto wrapper = [&]() -> auto {
			if(mods.modrm.mod() == 0)
			{
				// if 16-bit addressing:
				if(mods.legacyAddressingMode && mods.addressSizeOverride)
				{
					switch(mods.modrm.rm())
					{
						case 0: return MemoryRef(bits, regs::BX, regs::SI);
						case 1: return MemoryRef(bits, regs::BX, regs::DI);
						case 2: return MemoryRef(bits, regs::BP, regs::SI);
						case 3: return MemoryRef(bits, regs::BP, regs::DI);
						case 4: return MemoryRef(bits, regs::SI);
						case 5: return MemoryRef(bits, regs::DI);
						case 6: return MemoryRef(bits, readSignedImm16(buf));
						case 7: return MemoryRef(bits, regs::BX);
					}
				}
				else
				{
					// 32/64 bit addressing. we test REX.B inline here.
					switch(mods.modrm.rm())
					{
						case 0:  return MemoryRef(bits, mods.rex.B() ? regs::R8  : regs::RAX);
						case 1:  return MemoryRef(bits, mods.rex.B() ? regs::R9  : regs::RCX);
						case 2:  return MemoryRef(bits, mods.rex.B() ? regs::R10 : regs::RDX);
						case 3:  return MemoryRef(bits, mods.rex.B() ? regs::R11 : regs::RBX);

						// in 64-bit mode, this becomes rip-relative addressing. if not, it is just disp32.
						case 5:  return !mods.compatibilityMode ? MemoryRef(bits, regs::RIP, readSignedImm32(buf))
																	: MemoryRef(bits, readSignedImm32(buf));

						case 6:  return MemoryRef(bits, mods.rex.B() ? regs::R14 : regs::RSI);
						case 7:  return MemoryRef(bits, mods.rex.B() ? regs::R15 : regs::RDI);

						// SIB madness
						case 4: {
							bool dummy = false;
							return decodeSIB(buf, mods, &dummy).setBits(bits);
						}
					}
				}
			}
			else if(mods.modrm.mod() == 1)
			{
				// if 16-bit addressing:
				if(mods.legacyAddressingMode && mods.addressSizeOverride)
				{
					switch(mods.modrm.rm())
					{
						case 0: return MemoryRef(bits, regs::BX, regs::SI, 0, readSignedImm8(buf));
						case 1: return MemoryRef(bits, regs::BX, regs::DI, 0, readSignedImm8(buf));
						case 2: return MemoryRef(bits, regs::BP, regs::SI, 0, readSignedImm8(buf));
						case 3: return MemoryRef(bits, regs::BP, regs::DI, 0, readSignedImm8(buf));
						case 4: return MemoryRef(bits, regs::SI, readSignedImm8(buf));
						case 5: return MemoryRef(bits, regs::DI, readSignedImm8(buf));
						case 6: return MemoryRef(bits, regs::BP, readSignedImm8(buf));
						case 7: return MemoryRef(bits, regs::BX, readSignedImm8(buf));
					}
				}
				else
				{
					// 32/64 bit addressing
					switch(mods.modrm.rm())
					{
						case 0:  return MemoryRef(bits, mods.rex.B() ? regs::R8  : regs::RAX, readSignedImm8(buf));
						case 1:  return MemoryRef(bits, mods.rex.B() ? regs::R9  : regs::RCX, readSignedImm8(buf));
						case 2:  return MemoryRef(bits, mods.rex.B() ? regs::R10 : regs::RDX, readSignedImm8(buf));
						case 3:  return MemoryRef(bits, mods.rex.B() ? regs::R11 : regs::RBX, readSignedImm8(buf));
						case 5:  return MemoryRef(bits, mods.rex.B() ? regs::R13 : regs::RBP, readSignedImm8(buf));
						case 6:  return MemoryRef(bits, mods.rex.B() ? regs::R14 : regs::RSI, readSignedImm8(buf));
						case 7:  return MemoryRef(bits, mods.rex.B() ? regs::R15 : regs::RDI, readSignedImm8(buf));

						case 4: {
							bool disp = false;
							auto ret = decodeSIB(buf, mods, &disp).setBits(bits);
							if(!disp) ret.setDisplacement(readSignedImm8(buf));

							return ret;
						}
					}
				}
			}
			else if(mods.modrm.mod() == 2)
			{
				// if 16-bit addressing:
				if(mods.legacyAddressingMode && mods.addressSizeOverride)
				{
					switch(mods.modrm.rm())
					{
						case 0: return MemoryRef(bits, regs::BX, regs::SI, 0, readSignedImm16(buf));
						case 1: return MemoryRef(bits, regs::BX, regs::DI, 0, readSignedImm16(buf));
						case 2: return MemoryRef(bits, regs::BP, regs::SI, 0, readSignedImm16(buf));
						case 3: return MemoryRef(bits, regs::BP, regs::DI, 0, readSignedImm16(buf));
						case 4: return MemoryRef(bits, regs::SI, readSignedImm16(buf));
						case 5: return MemoryRef(bits, regs::DI, readSignedImm16(buf));
						case 6: return MemoryRef(bits, regs::BP, readSignedImm16(buf));
						case 7: return MemoryRef(bits, regs::BX, readSignedImm16(buf));
					}
				}
				else
				{
					// 32/64 bit addressing
					switch(mods.modrm.rm())
					{
						case 0:  return MemoryRef(bits, mods.rex.B() ? regs::R8  : regs::RAX, readSignedImm32(buf));
						case 1:  return MemoryRef(bits, mods.rex.B() ? regs::R9  : regs::RCX, readSignedImm32(buf));
						case 2:  return MemoryRef(bits, mods.rex.B() ? regs::R10 : regs::RDX, readSignedImm32(buf));
						case 3:  return MemoryRef(bits, mods.rex.B() ? regs::R11 : regs::RBX, readSignedImm32(buf));
						case 5:  return MemoryRef(bits, mods.rex.B() ? regs::R13 : regs::RBP, readSignedImm32(buf));
						case 6:  return MemoryRef(bits, mods.rex.B() ? regs::R14 : regs::RSI, readSignedImm32(buf));
						case 7:  return MemoryRef(bits, mods.rex.B() ? regs::R15 : regs::RDI, readSignedImm32(buf));

						case 4: {
							bool disp = false;
							auto ret = decodeSIB(buf, mods, &disp).setBits(bits);
							if(!disp) ret.setDisplacement(readSignedImm32(buf));

							return ret;
						}
					}
				}
			}

			return MemoryRef();
		};

		return wrapper()
			.setSegment(getSegmentOfOverride(mods.segmentOverride));
	}

	constexpr Operand getRegisterOrMemoryOperand(Buffer& buf, size_t regBits, size_t memBits, const InstrModifiers& mods, RegKind rk)
	{
		if(mods.directRegisterIndex)
		{
			auto idx = (mods.opcode & 0x0F) | (mods.rex.B() << 3);
			return decodeRegisterNumber(regBits, mods, idx, rk);
		}
		else if(mods.modrm.mod() != 3)
		{
			return getMemoryOperand(buf, memBits, mods);
		}
		else if(mods.modrm.mod() == 3)
		{
			// not the same as the one above -- this uses the modrm.rm field, not the modrm.reg field.
			auto idx = mods.modrm.rm() | (mods.rex.B() << 3);
			return decodeRegisterNumber(regBits, mods, idx, rk);
		}

		// return a poison value
		return regs::INVALID;
	}

	constexpr Operand getOperand(Buffer& buf, OpKind kind, InstrModifiers& mods)
	{
		switch(kind)
		{
			case OpKind::Reg8:          return getRegisterOperand(8,  mods, RegKind::GPR);
			case OpKind::Reg16:         return getRegisterOperand(16, mods, RegKind::GPR);
			case OpKind::Reg32:         return getRegisterOperand(32, mods, RegKind::GPR);
			case OpKind::Reg64:         return getRegisterOperand(64, mods, RegKind::GPR);

			case OpKind::Reg8_Rm:       return getRegisterOperandFromModRM(8,  mods, RegKind::GPR);
			case OpKind::Reg16_Rm:      return getRegisterOperandFromModRM(16, mods, RegKind::GPR);
			case OpKind::Reg32_Rm:      return getRegisterOperandFromModRM(32, mods, RegKind::GPR);
			case OpKind::Reg64_Rm:      return getRegisterOperandFromModRM(64, mods, RegKind::GPR);

			case OpKind::RegMem8:       return getRegisterOrMemoryOperand(buf, 8,  8,  mods, RegKind::GPR);
			case OpKind::RegMem16:      return getRegisterOrMemoryOperand(buf, 16, 16, mods, RegKind::GPR);
			case OpKind::RegMem32:      return getRegisterOrMemoryOperand(buf, 32, 32, mods, RegKind::GPR);
			case OpKind::RegMem64:      return getRegisterOrMemoryOperand(buf, 64, 64, mods, RegKind::GPR);

			case OpKind::RegMmx:        return getRegisterOperand(64, mods, RegKind::Vector);
			case OpKind::RegXmm:        return getRegisterOperand(128, mods, RegKind::Vector);

			case OpKind::RegMmx_Rm:     return getRegisterOperandFromModRM(64, mods, RegKind::Vector);
			case OpKind::RegXmm_Rm:     return getRegisterOperandFromModRM(128, mods, RegKind::Vector);

			case OpKind::RegMmxMem32:   return getRegisterOrMemoryOperand(buf, 64, 32, mods, RegKind::Vector);
			case OpKind::RegMmxMem64:   return getRegisterOrMemoryOperand(buf, 64, 64, mods, RegKind::Vector);

			case OpKind::RegXmmMem32:   return getRegisterOrMemoryOperand(buf, 128, 32, mods, RegKind::Vector);
			case OpKind::RegXmmMem64:   return getRegisterOrMemoryOperand(buf, 128, 64, mods, RegKind::Vector);
			case OpKind::RegXmmMem128:  return getRegisterOrMemoryOperand(buf, 128, 128, mods, RegKind::Vector);

			case OpKind::Mem8:          return getMemoryOperand(buf, 8, mods);
			case OpKind::Mem16:         return getMemoryOperand(buf, 16, mods);
			case OpKind::Mem32:         return getMemoryOperand(buf, 32, mods);
			case OpKind::Mem64:         return getMemoryOperand(buf, 64, mods);
			case OpKind::Mem128:        return getMemoryOperand(buf, 128, mods);
			case OpKind::Mem256:        return getMemoryOperand(buf, 256, mods);

			// the index for these special registers is always in modRM.reg
			case OpKind::SegmentReg:   return getRegisterOperand(16, mods, RegKind::Segment);
			case OpKind::ControlReg:   return getRegisterOperand(64, mods, RegKind::Control);
			case OpKind::DebugReg:     return getRegisterOperand(64, mods, RegKind::Debug);


			// this is damn dumb
			case OpKind::Reg32Mem16:
				return getRegisterOrMemoryOperand(buf, 32, 16, mods, RegKind::GPR);


			case OpKind::Imm8:
				return readSignedImm8(buf);

			case OpKind::SignExtImm8: {
				if(mods.operandSizeOverride)
					return (int16_t) readSignedImm8(buf);

				else if(mods.rex.W())
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

				else if(kind == OpKind::Imm64 && mods.rex.W())
					return readSignedImm64(buf);

				else if(mods.rex.W())
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
				return getMemoryOperand(buf, 0, mods);

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

			// this is never promoted
			case OpKind::ImplicitAL:   return regs::AL;

			case OpKind::ImplicitAX:
			case OpKind::ImplicitEAX:
			case OpKind::ImplicitRAX: {
				if(mods.operandSizeOverride)
					return regs::AX;
				else if(mods.rex.W())
					return regs::RAX;
				else
					return regs::EAX;
			}

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

			case OpKind::ImplicitMem8_ES_EDI:
			case OpKind::ImplicitMem32_ES_EDI: {
				int bits = 0;
				if(kind == OpKind::ImplicitMem8_ES_EDI) bits = 8;
				else if(mods.operandSizeOverride)       bits = 16;
				else if(mods.rex.W())                   bits = 64;
				else                                    bits = 32;

				return MemoryRef(bits, mods.compatibilityMode ? regs::EDI : regs::RDI).setSegment(regs::ES);
			}

			case OpKind::ImplicitMem8_ESI:
			case OpKind::ImplicitMem32_ESI: {
				int bits = 0;
				if(kind == OpKind::ImplicitMem8_ESI)    bits = 8;
				else if(mods.operandSizeOverride)       bits = 16;
				else if(mods.rex.W())                   bits = 64;
				else                                    bits = 32;

				auto seg = regs::DS;
				if(auto s = getSegmentOfOverride(mods.segmentOverride); s.present())
					seg = s;

				return MemoryRef(bits, mods.compatibilityMode ? regs::ESI : regs::RSI).setSegment(seg);
			}

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
}
