// operands.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "x86.h"
#include "immediates.h"

namespace instrad::x86
{
	enum class RegKind
	{
		GPR,
		Segment,
		Control,
		Debug,
		Vector,
		X87,
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

				else if(mods.legacyAddressingMode || mods.operandSizeOverride)
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
		else if(rk == RegKind::X87)
		{
			return regs::getX87(index);
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


	template <typename Buffer>
	constexpr MemoryRef decodeSIB(Buffer& buf, const InstrModifiers& mods, bool* didReadDisplacement)
	{
		auto sib = buf.pop();

		uint8_t base  = ((sib & 0x07) >> 0);
		uint8_t index = ((sib & 0x38) >> 3);
		uint8_t scale = ((sib & 0xC0) >> 6);

		// btw, only 32-bit and 64-bit encodings can have SIBs, which is why real-mode stuff
		// is not handled here.
		auto compat = mods.compatibilityMode;

		auto mem = MemoryRef();

		if(base == 5)
		{
			if(mods.modrm.mod() == 0)
			{
				mem.setBase(regs::NONE);
				mem.setDisplacement(readUnsignedImm32(buf));
				*didReadDisplacement = true;
			}
			else if(mods.modrm.mod() == 1)
			{
				mem.setBase(mods.rex.B() ? regs::R13 : ((compat || mods.addressSizeOverride) ? regs::EBP : regs::RBP));
				mem.setDisplacement(readUnsignedImm8(buf));
				*didReadDisplacement = true;
			}
			else if(mods.modrm.mod() == 2)
			{
				mem.setBase(mods.rex.B() ? regs::R13 : ((compat || mods.addressSizeOverride) ? regs::EBP : regs::RBP));
				mem.setDisplacement(readUnsignedImm32(buf));
				*didReadDisplacement = true;
			}
			else
			{
				mem.setBase(regs::INVALID);
			}
		}
		else
		{
			mem.setBase(mods.legacyAddressingMode
				? regs::get16Bit(base)
				: ((compat || mods.addressSizeOverride)
					? regs::get32Bit(base)
					: regs::get64Bit(base | (mods.rex.B() << 3)))
			);
		}

		auto idxIdx = index | (mods.rex.X() << 3);
		mem.setIndex((compat || mods.addressSizeOverride)
			? regs::get32Bit(index)
			// you can't use RSP as an index
			: (idxIdx == 4 ? regs::NONE : regs::get64Bit(idxIdx))
		);

		mem.setScale(1 << scale);
		return mem;
	}

	template <typename Buffer>
	constexpr MemoryRef decodeVSIB(Buffer& buf, size_t vectorRegBits, const InstrModifiers& mods)
	{
		auto vsib = buf.pop();

		uint8_t base  = ((vsib & 0x07) >> 0) | (mods.vex.B() << 3);
		uint8_t index = ((vsib & 0x38) >> 3) | (mods.vex.X() << 3);
		uint8_t scale = ((vsib & 0xC0) >> 6);

		auto mem = MemoryRef();
		mem.setScale(1 << scale);

		mem.setIndex(vectorRegBits == 128
						? regs::getXMM(index)
						: regs::getYMM(index)
					);

		if(mods.modrm.mod() == 0)
		{
			mem.setBase(regs::NONE);
			mem.setDisplacement(readUnsignedImm32(buf));
		}
		else if(mods.modrm.mod() == 1)
		{
			mem.setDisplacement(readUnsignedImm8(buf));
			mem.setBase(mods.compatibilityMode
				? regs::get32Bit(base)
				: regs::get64Bit(base)
			);
		}
		else if(mods.modrm.mod() == 2)
		{
			mem.setDisplacement(readUnsignedImm32(buf));
			mem.setBase(mods.compatibilityMode
				? regs::get32Bit(base)
				: regs::get64Bit(base)
			);
		}

		return mem;
	}



	constexpr Register getRegisterOperandFromVVVV(size_t bits, const InstrModifiers& mods, RegKind rk)
	{
		return decodeRegisterNumber(bits, mods, mods.vex.vvvv(), rk);
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

	template <typename Buffer>
	constexpr MemoryRef getMemoryOperand(Buffer& buf, size_t bits, const InstrModifiers& mods)
	{
		// we need to handle "promotion".
		if(bits == 32 && mods.rex.W())
			bits = 64;

		else if(bits == 32 && mods.addressSizeOverride)
			bits = 16;

		else if(bits == 16 && mods.addressSizeOverride)
			bits = 32;

		// the size of the registers used (base/index)
		bool compat = mods.compatibilityMode;
		auto baseRegIndex = (mods.modrm.rm() | (mods.rex.B() << 3));

		// lazy to refactor this, so just wrap it in a lambda so we can extract the return value
		auto wrapper = [&]() -> auto {
			if(mods.modrm.mod() == 0)
			{
				// if 16-bit addressing:
				if(mods.legacyAddressingMode)
				{
					switch(mods.modrm.rm())
					{
						case 0: return MemoryRef(bits, regs::BX, regs::SI);
						case 1: return MemoryRef(bits, regs::BX, regs::DI);
						case 2: return MemoryRef(bits, regs::BP, regs::SI);
						case 3: return MemoryRef(bits, regs::BP, regs::DI);
						case 4: return MemoryRef(bits, regs::SI);
						case 5: return MemoryRef(bits, regs::DI);
						case 6: return MemoryRef(bits, readUnsignedImm16(buf));
						case 7: return MemoryRef(bits, regs::BX);
					}
				}
				else
				{
					if(mods.modrm.rm() == 4)
					{
						bool dummy = false;
						return decodeSIB(buf, mods, &dummy).setBits(bits);
					}
					else if(mods.modrm.rm() == 5)
					{
						// in 64-bit mode, this becomes rip-relative addressing. if not, it is just disp32.
						if(compat)
							return MemoryRef(bits, readUnsignedImm32(buf));

						else
							return MemoryRef(bits, regs::RIP, readUnsignedImm32(buf));
					}
					else
					{
						if(compat || mods.addressSizeOverride)
							return MemoryRef(bits, regs::get32Bit(baseRegIndex));

						else
							return MemoryRef(bits, regs::get64Bit(baseRegIndex));
					}
				}
			}
			else if(mods.modrm.mod() == 1)
			{
				// if 16-bit addressing:
				if(mods.legacyAddressingMode)
				{
					auto imm = readUnsignedImm8(buf);
					switch(mods.modrm.rm())
					{
						// note: scale should be 1
						case 0: return MemoryRef(bits, regs::BX, regs::SI, 1, imm);
						case 1: return MemoryRef(bits, regs::BX, regs::DI, 1, imm);
						case 2: return MemoryRef(bits, regs::BP, regs::SI, 1, imm);
						case 3: return MemoryRef(bits, regs::BP, regs::DI, 1, imm);
						case 4: return MemoryRef(bits, regs::SI, imm);
						case 5: return MemoryRef(bits, regs::DI, imm);
						case 6: return MemoryRef(bits, regs::BP, imm);
						case 7: return MemoryRef(bits, regs::BX, imm);
					}
				}
				else
				{
					if(mods.modrm.rm() == 4)
					{
						bool disp = false;
						auto ret = decodeSIB(buf, mods, &disp).setBits(bits);
						if(!disp) ret.setDisplacement(readUnsignedImm8(buf));

						return ret;
					}
					else
					{
						auto imm = readUnsignedImm8(buf);

						if(compat || mods.addressSizeOverride)
							return MemoryRef(bits, regs::get32Bit(baseRegIndex), imm);

						else
							return MemoryRef(bits, regs::get64Bit(baseRegIndex), imm);
					}
				}
			}
			else if(mods.modrm.mod() == 2)
			{
				// if 16-bit addressing:
				if(mods.legacyAddressingMode)
				{
					auto imm = readUnsignedImm16(buf);
					switch(mods.modrm.rm())
					{
						// note: scale should be 1
						case 0: return MemoryRef(bits, regs::BX, regs::SI, 1, imm);
						case 1: return MemoryRef(bits, regs::BX, regs::DI, 1, imm);
						case 2: return MemoryRef(bits, regs::BP, regs::SI, 1, imm);
						case 3: return MemoryRef(bits, regs::BP, regs::DI, 1, imm);
						case 4: return MemoryRef(bits, regs::SI, imm);
						case 5: return MemoryRef(bits, regs::DI, imm);
						case 6: return MemoryRef(bits, regs::BP, imm);
						case 7: return MemoryRef(bits, regs::BX, imm);
					}
				}
				else
				{
					if(mods.modrm.rm() == 4)
					{
						bool disp = false;
						auto ret = decodeSIB(buf, mods, &disp).setBits(bits);
						if(!disp) ret.setDisplacement(readUnsignedImm32(buf));

						return ret;
					}
					else
					{
						auto imm = readUnsignedImm32(buf);

						if(compat || mods.addressSizeOverride)
							return MemoryRef(bits, regs::get32Bit(baseRegIndex), imm);

						else
							return MemoryRef(bits, regs::get64Bit(baseRegIndex), imm);
					}
				}
			}

			return MemoryRef();
		};

		return wrapper().setSegment(getSegmentOfOverride(mods.segmentOverride));
	}

	template <typename Buffer>
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

	constexpr int getCurrentBits(InstrModifiers& mods)
	{
		if(mods.legacyAddressingMode)   return 16;
		else if(mods.compatibilityMode) return 32;
		else                            return 64;
	}

	template <typename Buffer>
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
			case OpKind::RegYmm:        return getRegisterOperand(256, mods, RegKind::Vector);

			case OpKind::RegMmx_Rm:     return getRegisterOperandFromModRM(64, mods, RegKind::Vector);
			case OpKind::RegXmm_Rm:     return getRegisterOperandFromModRM(128, mods, RegKind::Vector);
			case OpKind::RegYmm_Rm:     return getRegisterOperandFromModRM(256, mods, RegKind::Vector);

			case OpKind::RegMmxMem32:   return getRegisterOrMemoryOperand(buf, 64, 32, mods, RegKind::Vector);
			case OpKind::RegMmxMem64:   return getRegisterOrMemoryOperand(buf, 64, 64, mods, RegKind::Vector);

			case OpKind::RegXmmMem8:    return getRegisterOrMemoryOperand(buf, 128, 8, mods, RegKind::Vector);
			case OpKind::RegXmmMem16:   return getRegisterOrMemoryOperand(buf, 128, 16, mods, RegKind::Vector);
			case OpKind::RegXmmMem32:   return getRegisterOrMemoryOperand(buf, 128, 32, mods, RegKind::Vector);
			case OpKind::RegXmmMem64:   return getRegisterOrMemoryOperand(buf, 128, 64, mods, RegKind::Vector);
			case OpKind::RegXmmMem128:  return getRegisterOrMemoryOperand(buf, 128, 128, mods, RegKind::Vector);

			case OpKind::RegYmmMem256:  return getRegisterOrMemoryOperand(buf, 256, 256, mods, RegKind::Vector);

			case OpKind::Mem8:          return getMemoryOperand(buf, 8, mods);
			case OpKind::Mem16:         return getMemoryOperand(buf, 16, mods);
			case OpKind::Mem32:         return getMemoryOperand(buf, 32, mods);
			case OpKind::Mem64:         return getMemoryOperand(buf, 64, mods);
			case OpKind::Mem80:         return getMemoryOperand(buf, 80, mods);
			case OpKind::Mem128:        return getMemoryOperand(buf, 128, mods);
			case OpKind::Mem256:        return getMemoryOperand(buf, 256, mods);

			// the index for these special registers is always in modRM.reg
			case OpKind::SegmentReg:    return getRegisterOperand(16, mods, RegKind::Segment);
			case OpKind::ControlReg:    return getRegisterOperand(64, mods, RegKind::Control);
			case OpKind::DebugReg:      return getRegisterOperand(64, mods, RegKind::Debug);

			case OpKind::RegX87_Rm:     return getRegisterOperand(80, mods, RegKind::X87);

			// this is damn dumb
			case OpKind::Reg32Mem8:     return getRegisterOrMemoryOperand(buf, 32, 8, mods, RegKind::GPR);
			case OpKind::Reg32Mem16:    return getRegisterOrMemoryOperand(buf, 32, 16, mods, RegKind::GPR);


			case OpKind::Imm8:
				return readSignedImm8(buf);

			case OpKind::SignExtImm8: {
				auto bits = getCurrentBits(mods);

				if(bits == 16 || bits == 32)
				{
					if(mods.operandSizeOverride == (bits == 16))
						return (int32_t) readSignedImm8(buf);
					else
						return (int16_t) readSignedImm8(buf);
				}
				else
				{
					if(mods.operandSizeOverride)
						return (int16_t) readSignedImm8(buf);
					else if(mods.rex.W())
						return (int64_t) readSignedImm8(buf);
					else
						return (int32_t) readSignedImm8(buf);
				}
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
				if(mods.operandSizeOverride || mods.legacyAddressingMode)
					return readSignedImm16(buf);

				else if(kind == OpKind::Imm64 && mods.rex.W())
					return readSignedImm64(buf);

				else if(mods.rex.W())
					return (int64_t) readSignedImm32(buf);

				else
					return readSignedImm32(buf);
			}

			case OpKind::ImmNative: {
				auto bits = getCurrentBits(mods);

				if(bits == 16 || bits == 32)
				{
					if((bits == 16 && mods.operandSizeOverride) || (bits == 32 && !mods.operandSizeOverride))
						return readSignedImm32(buf);
					else
						return readSignedImm16(buf);
				}
				else
				{
					if(mods.rex.W())
						return (int64_t) readSignedImm32(buf);

					return readSignedImm32(buf);
				}
			}


			case OpKind::RegNative:
				return getRegisterOperand(getCurrentBits(mods), mods, RegKind::GPR);

			case OpKind::RegMemNative:
				return getRegisterOrMemoryOperand(buf, getCurrentBits(mods), getCurrentBits(mods),
					mods, RegKind::GPR);

			case OpKind::SignExtImm32: {
				auto bits = getCurrentBits(mods);

				if(bits == 16)
					return readSignedImm16(buf);
				else
					return static_cast<int64_t>(readSignedImm32(buf));
			}

			// TODO: need to handle relative offsets properly
			case OpKind::Rel8Offset:
				return RelOffset(readSignedImm8(buf));

			case OpKind::Rel16Offset:
			case OpKind::Rel32Offset: {
				if(mods.operandSizeOverride)
					return RelOffset(readSignedImm16(buf));

				else
					return RelOffset(readSignedImm32(buf));
			}

			case OpKind::RelNative_16or32_Offset: {
				// legacy && override -> 32; !legacy && !override -> 32
				if(mods.legacyAddressingMode == mods.operandSizeOverride)
					return RelOffset(readSignedImm32(buf));

				else
					return RelOffset(readSignedImm16(buf));
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

			case OpKind::ImplicitNativeAX: {
				auto bits = getCurrentBits(mods);
				if(bits == 16)
					return (mods.operandSizeOverride ? regs::EAX : regs::AX);

				else if(bits == 32)
					return (mods.operandSizeOverride ? regs::AX : regs::EAX);

				else
					return (mods.rex.W() ? regs::RAX : regs::EAX);
			}


			case OpKind::ImplicitXMM0:  return regs::XMM0;

			case OpKind::ImplicitST0:   return regs::ST0;
			case OpKind::ImplicitST1:   return regs::ST1;

			case OpKind::MemoryOfs8:
			case OpKind::MemoryOfs16:
			case OpKind::MemoryOfs32:
			case OpKind::MemoryOfs64: {
				// kekw
				int bits = 8 * (1 << ((int) kind - (int) OpKind::MemoryOfs8));
				if(bits == 32 && mods.rex.W())
					bits = 64;

				auto seg = getSegmentOfOverride(mods.segmentOverride);
				if(mods.legacyAddressingMode)
					return MemoryRef(bits, readUnsignedImm16(buf)).setSegment(seg);
				else if(mods.compatibilityMode)
					return MemoryRef(bits, readUnsignedImm32(buf)).setSegment(seg);
				else
					return MemoryRef(bits, readUnsignedImm64(buf)).setSegment(seg);
			}

			case OpKind::MemoryOfsNative: {
				int bits = getCurrentBits(mods);

				int membits = bits;
				if(membits == 32 && mods.rex.W())
					membits = 64;


				auto seg = getSegmentOfOverride(mods.segmentOverride);
				if(bits == 16 || bits == 32)
				{
					// in 16-bit, address override bumps to 32; in 32-bit, address override drops to 16
					if((bits == 16 && mods.addressSizeOverride) || (bits == 32 && !mods.addressSizeOverride))
						return MemoryRef(membits, readUnsignedImm32(buf)).setSegment(seg);

					else
						return MemoryRef(membits, readUnsignedImm16(buf)).setSegment(seg);
				}
				else
				{
					return MemoryRef(bits, readUnsignedImm64(buf)).setSegment(seg);
				}
			}



			case OpKind::ImplicitMem8_ES_EDI:
			case OpKind::ImplicitMemNative_ES_DI: {
				int bits = 0;
				auto reg = (mods.compatibilityMode ? regs::EDI : (mods.legacyAddressingMode ? regs::DI : regs::RDI));

				if(kind == OpKind::ImplicitMem8_ES_EDI)
				{
					bits = 8;
				}
				else if(mods.rex.W())
				{
					bits = 64;
				}
				else
				{
					if(mods.operandSizeOverride == mods.legacyAddressingMode)
						bits = 32;

					else
						bits = 16;
				}

				return MemoryRef(bits, reg).setSegment(regs::ES);
			}

			case OpKind::ImplicitMem8_ESI:
			case OpKind::ImplicitMemNative_SI: {

				int bits = 0;
				auto reg = (mods.compatibilityMode ? regs::ESI : (mods.legacyAddressingMode ? regs::SI : regs::RSI));

				if(kind == OpKind::ImplicitMem8_ESI)
				{
					bits = 8;
				}
				else if(mods.rex.W())
				{
					bits = 64;
				}
				else
				{
					if(mods.operandSizeOverride == mods.legacyAddressingMode)
						bits = 32;

					else
						bits = 16;
				}

				auto seg = regs::DS;
				if(auto s = getSegmentOfOverride(mods.segmentOverride); s.present())
					seg = s;

				return MemoryRef(bits, reg).setSegment(seg);
			}

			case OpKind::Reg32_vvvv:    return getRegisterOperandFromVVVV(32, mods, RegKind::GPR);
			case OpKind::Reg64_vvvv:    return getRegisterOperandFromVVVV(64, mods, RegKind::GPR);
			case OpKind::RegXmm_vvvv:   return getRegisterOperandFromVVVV(128, mods, RegKind::Vector);
			case OpKind::RegYmm_vvvv:   return getRegisterOperandFromVVVV(256, mods, RegKind::Vector);

			case OpKind::RegXmm_TrailingImm8HighNib:
				return decodeRegisterNumber(128, mods, (readSignedImm8(buf) & 0xF) >> 4, RegKind::Vector);

			case OpKind::RegYmm_TrailingImm8HighNib:
				return decodeRegisterNumber(256, mods, (readSignedImm8(buf) & 0xF) >> 4, RegKind::Vector);

			case OpKind::VSIB_Xmm32:
			case OpKind::VSIB_Xmm64:
				return decodeVSIB(buf, 128, mods);

			case OpKind::VSIB_Ymm32:
			case OpKind::VSIB_Ymm64:
				return decodeVSIB(buf, 256, mods);

			// this is either a 32-bit value (16+16), or a 48-bit value (16+32). this does not support 80 bit (16+64).
			// the size is determined by the operand-size override.
			case OpKind::ImmSegOfs: {
				int bits = getCurrentBits(mods);
				if((bits == 16 && mods.operandSizeOverride) || (bits >= 32 && !mods.operandSizeOverride))
				{
					auto ofs = readUnsignedImm32(buf);
					auto seg = readUnsignedImm16(buf);
					return FarOffset(seg, ofs);
				}
				else
				{
					auto ofs = readUnsignedImm16(buf);
					auto seg = readUnsignedImm16(buf);
					return FarOffset(seg, ofs);
				}
			}

			// this is either a 32-bit value (16+16), 48-bit (16+32), or 80-bit (16+64). the size is determined by the
			// operand-size override, and additionally REX.W to get a 64-bit offset.
			case OpKind::MemSegOfs: {
				int bits = getCurrentBits(mods);
				if(mods.rex.W())
					bits = 64;
				else if((bits == 16 && mods.operandSizeOverride) || (bits >= 32 && !mods.operandSizeOverride))
					bits = 32;
				else
					bits = 16;

				// while the actual read size is 32/48/80, that'll probably break everything, so just use
				// 16/32/64 and the extra 16-bits for the segment is implied.
				return FarOffset(getMemoryOperand(buf, bits, mods));
			}

			case OpKind::None:
				return { };

		}

		// gcc is too stupid to realise that the switch covers all options
		return { };
	}
}
