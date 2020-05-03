// x64.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <stdio.h>

#include "buffer.h"

#include "ops.h"
#include "regs.h"
#include "table.h"

namespace instrad::x64
{
	struct MemoryRef
	{
		constexpr MemoryRef() { }

		constexpr explicit MemoryRef(int bits, int64_t d) : m_bits(bits), m_displacement(d) { }
		constexpr explicit MemoryRef(int bits, const Register& b) : m_bits(bits), m_base(b) { }

		constexpr MemoryRef(int bits, const Register& b, int64_t d) : m_bits(bits), m_displacement(d), m_base(b) { }
		constexpr MemoryRef(int bits, const Register& b, const Register& i) : m_bits(bits), m_base(b), m_index(i) { }

		constexpr MemoryRef(int bits, const Register& b, const Register& i, int s, int64_t d)
			: m_bits(bits), m_scale(s), m_displacement(d), m_base(b), m_index(i) { }

		constexpr int bits() const { return this->m_bits; }
		constexpr int scale() const { return this->m_scale; }
		constexpr const Register& segment() const { return this->m_segment; }
		constexpr int64_t displacement() const { return this->m_displacement; }
		constexpr const Register& base() const { return this->m_base; }
		constexpr const Register& index() const { return this->m_index; }

		constexpr MemoryRef& setBits(int b) { this->m_bits = b; return *this; }
		constexpr MemoryRef& setScale(int s) { this->m_scale = s; return *this; }
		constexpr MemoryRef& setDisplacement(int64_t d) { this->m_displacement = d; return *this; }
		constexpr MemoryRef& setIndex(const Register& i) { this->m_index = i; return *this; }
		constexpr MemoryRef& setBase(const Register& b) { this->m_base = b; return *this; }
		constexpr MemoryRef& setSegment(const Register& b) { this->m_segment = b; return *this; }

	private:
		int m_bits = 0;
		Register m_segment = regs::NONE;

		// base + (index * scale) + displacement

		int m_scale = 1;
		int64_t m_displacement = 0;
		Register m_base = regs::NONE;
		Register m_index = regs::NONE;
	};

	struct Operand
	{
		constexpr Operand() { }
		constexpr Operand(const MemoryRef& mem) : m_type(TYPE_MEM), m_mem(mem) { }
		constexpr Operand(const Register& reg) : m_type(TYPE_REG), m_reg(reg) { }

		constexpr Operand(int64_t imm) : m_type(TYPE_IMM), m_imm(imm), m_immbits(64) { }
		constexpr Operand(int32_t imm) : m_type(TYPE_IMM), m_imm(imm), m_immbits(32) { }
		constexpr Operand(int16_t imm) : m_type(TYPE_IMM), m_imm(imm), m_immbits(16) { }
		constexpr Operand(int8_t imm) : m_type(TYPE_IMM), m_imm(imm), m_immbits(8) { }

		constexpr bool isImmediate() const { return this->m_type == TYPE_IMM; }
		constexpr bool isRegister() const { return this->m_type == TYPE_REG; }
		constexpr bool isMemory() const { return this->m_type == TYPE_MEM; }

		constexpr const MemoryRef& mem() const { return this->m_mem; }
		constexpr const Register& reg() const { return this->m_reg; }
		constexpr uint64_t imm() const { return this->m_imm; }

		constexpr MemoryRef& mem() { return this->m_mem; }
		constexpr Register& reg() { return this->m_reg; }

		constexpr int immediateSize() const { return this->m_immbits; }

	private:
		constexpr static int TYPE_REG = 0;
		constexpr static int TYPE_IMM = 1;
		constexpr static int TYPE_MEM = 2;

		int m_type = TYPE_IMM;
		union {
			Register m_reg;
			MemoryRef m_mem;
			int64_t m_imm = 0;
		};

		int m_immbits = 0;
	};

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

	struct RexPrefix
	{
		constexpr RexPrefix(uint8_t b) : byte(b) { }

		union {
			struct {
				uint8_t B : 1;
				uint8_t X : 1;
				uint8_t R : 1;
				uint8_t W : 1;
				uint8_t four : 4;
			};

			uint8_t byte;
		};

		constexpr bool present() const { return this->byte != 0; }

		static RexPrefix none() { return RexPrefix(0); }
	};

	struct ModRM
	{
		constexpr ModRM(uint8_t b) : byte(b) { }

		union {
			struct {
				uint8_t rm  : 3;
				uint8_t reg : 3;
				uint8_t mod : 2;
			};

			uint8_t byte;
		};

		constexpr bool present() const { return this->byte != 0; }
		static ModRM none() { return ModRM(0); }
	};

	struct InstrModifiers
	{
		uint8_t opcode = 0;

		ModRM modrm = ModRM::none();
		RexPrefix rex = RexPrefix::none();

		bool addressSizeOverride = false;
		bool operandSizeOverride = false;

		// if true, we are in 16-bit mode; if false, then 32/64-bit mode.
		bool legacyAddressingMode = false;

		// if true, we are in 32-bit mode; if false, then 64-bit mode.
		bool compatibilityMode = false;

		// low nibble register index.
		bool directRegisterIndex = false;

		static constexpr int SEG_NONE = 0;
		static constexpr int SEG_CS = 1;
		static constexpr int SEG_DS = 2;
		static constexpr int SEG_ES = 3;
		static constexpr int SEG_FS = 4;
		static constexpr int SEG_GS = 5;
		static constexpr int SEG_SS = 6;

		int segmentOverride = SEG_NONE;

		bool lockPrefix = false;
		bool repPrefix = false;
		bool repnzPrefix = false;
	};

	static_assert(sizeof(ModRM) == 1);
	static_assert(sizeof(RexPrefix) == 1);

	constexpr int8_t readSignedImm8(Buffer& buf)
	{
		return (int8_t) buf.pop();
	}

	constexpr int16_t readSignedImm16(Buffer& buf)
	{
		uint16_t a = buf.pop();
		uint16_t b = buf.pop();

		return (int16_t) ((a << 0) | (b << 8));
	}

	constexpr int32_t readSignedImm32(Buffer& buf)
	{
		uint32_t a = buf.pop();
		uint32_t b = buf.pop();
		uint32_t c = buf.pop();
		uint32_t d = buf.pop();
		return (int32_t) ((a << 0) | (b << 8) | (c << 16) | (d << 24));
	}

	constexpr int64_t readSignedImm64(Buffer& buf)
	{
		uint64_t a = buf.pop();
		uint64_t b = buf.pop();
		uint64_t c = buf.pop();
		uint64_t d = buf.pop();
		uint64_t e = buf.pop();
		uint64_t f = buf.pop();
		uint64_t g = buf.pop();
		uint64_t h = buf.pop();
		return (int64_t) ((a << 0) | (b << 8) | (c << 16) | (d << 24) | (e << 32) | (f << 40) | (g << 48) | (h << 56));
	}

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
				if(mods.rex.W || bits == 64)
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
			if(mods.modrm.mod == 0)
			{
				mem.setBase(regs::NONE);
				mem.setDisplacement(readSignedImm32(buf));
				*didReadDisplacement = true;
			}
			else if(mods.modrm.mod == 1)
			{
				mem.setBase(mods.rex.B ? regs::R13 : regs::RBP);
				mem.setDisplacement(readSignedImm8(buf));
				*didReadDisplacement = true;
			}
			else if(mods.modrm.mod == 2)
			{
				mem.setBase(mods.rex.B ? regs::R13 : regs::RBP);
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
				case 0: mem.setBase(mods.rex.B ? regs::R8  : regs::RAX); break;
				case 1: mem.setBase(mods.rex.B ? regs::R9  : regs::RCX); break;
				case 2: mem.setBase(mods.rex.B ? regs::R10 : regs::RDX); break;
				case 3: mem.setBase(mods.rex.B ? regs::R11 : regs::RBX); break;
				case 4: mem.setBase(mods.rex.B ? regs::R12 : regs::RSP); break;
				case 6: mem.setBase(mods.rex.B ? regs::R14 : regs::RSI); break;
				case 7: mem.setBase(mods.rex.B ? regs::R15 : regs::RDI); break;

				default: mem.setBase(regs::INVALID); break;
			}
		}

		switch(sib.index)
		{
			case 0: mem.setIndex(mods.rex.X ? regs::R8  : regs::RAX);  break;
			case 1: mem.setIndex(mods.rex.X ? regs::R9  : regs::RCX);  break;
			case 2: mem.setIndex(mods.rex.X ? regs::R10 : regs::RDX);  break;
			case 3: mem.setIndex(mods.rex.X ? regs::R11 : regs::RBX);  break;
			case 4: mem.setIndex(mods.rex.X ? regs::R12 : regs::NONE); break;   // apparently you can't use RSP as an index
			case 5: mem.setIndex(mods.rex.X ? regs::R13 : regs::RBP);  break;
			case 6: mem.setIndex(mods.rex.X ? regs::R14 : regs::RSI);  break;
			case 7: mem.setIndex(mods.rex.X ? regs::R15 : regs::RDI);  break;

			default: mem.setIndex(regs::INVALID); break;
		}

		mem.setScale(1 << sib.scale);
		return mem;
	}


	constexpr Register getRegisterOperandFromModRM(size_t bits, const InstrModifiers& mods, RegKind rk)
	{
		return decodeRegisterNumber(bits, mods, mods.modrm.rm | (mods.rex.B << 3), rk);
	}

	constexpr Register getRegisterOperand(size_t bits, const InstrModifiers& mods, RegKind rk)
	{
		if(mods.directRegisterIndex)
			return decodeRegisterNumber(bits, mods, (mods.opcode & 0x07) | (mods.rex.B << 3), rk);

		else
			return decodeRegisterNumber(bits, mods, mods.modrm.reg | (mods.rex.R << 3), rk);
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
		if(bits == 32 && mods.rex.W)
			bits = 64;

		else if(bits == 32 && mods.operandSizeOverride)
			bits = 16;

		// lazy to refactor this, so just wrap it in a lambda so we can extract the return value

		auto wrapper = [&]() -> auto {
			if(mods.modrm.mod == 0)
			{
				// if 16-bit addressing:
				if(mods.legacyAddressingMode && mods.addressSizeOverride)
				{
					switch(mods.modrm.rm)
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
					switch(mods.modrm.rm)
					{
						case 0:  return MemoryRef(bits, mods.rex.B ? regs::R8  : regs::RAX);
						case 1:  return MemoryRef(bits, mods.rex.B ? regs::R9  : regs::RCX);
						case 2:  return MemoryRef(bits, mods.rex.B ? regs::R10 : regs::RDX);
						case 3:  return MemoryRef(bits, mods.rex.B ? regs::R11 : regs::RBX);

						// in 64-bit mode, this becomes rip-relative addressing. if not, it is just disp32.
						case 5:  return !mods.compatibilityMode ? MemoryRef(bits, regs::RIP, readSignedImm32(buf))
																	: MemoryRef(bits, readSignedImm32(buf));

						case 6:  return MemoryRef(bits, mods.rex.B ? regs::R14 : regs::RSI);
						case 7:  return MemoryRef(bits, mods.rex.B ? regs::R15 : regs::RDI);

						// SIB madness
						case 4: {
							bool dummy = false;
							return decodeSIB(buf, mods, &dummy).setBits(bits);
						}
					}
				}
			}
			else if(mods.modrm.mod == 1)
			{
				// if 16-bit addressing:
				if(mods.legacyAddressingMode && mods.addressSizeOverride)
				{
					switch(mods.modrm.rm)
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
					switch(mods.modrm.rm)
					{
						case 0:  return MemoryRef(bits, mods.rex.B ? regs::R8  : regs::RAX, readSignedImm8(buf));
						case 1:  return MemoryRef(bits, mods.rex.B ? regs::R9  : regs::RCX, readSignedImm8(buf));
						case 2:  return MemoryRef(bits, mods.rex.B ? regs::R10 : regs::RDX, readSignedImm8(buf));
						case 3:  return MemoryRef(bits, mods.rex.B ? regs::R11 : regs::RBX, readSignedImm8(buf));
						case 5:  return MemoryRef(bits, mods.rex.B ? regs::R13 : regs::RBP, readSignedImm8(buf));
						case 6:  return MemoryRef(bits, mods.rex.B ? regs::R14 : regs::RSI, readSignedImm8(buf));
						case 7:  return MemoryRef(bits, mods.rex.B ? regs::R15 : regs::RDI, readSignedImm8(buf));

						case 4: {
							bool disp = false;
							auto ret = decodeSIB(buf, mods, &disp).setBits(bits);
							if(!disp) ret.setDisplacement(readSignedImm8(buf));

							return ret;
						}
					}
				}
			}
			else if(mods.modrm.mod == 2)
			{
				// if 16-bit addressing:
				if(mods.legacyAddressingMode && mods.addressSizeOverride)
				{
					switch(mods.modrm.rm)
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
					switch(mods.modrm.rm)
					{
						case 0:  return MemoryRef(bits, mods.rex.B ? regs::R8  : regs::RAX, readSignedImm32(buf));
						case 1:  return MemoryRef(bits, mods.rex.B ? regs::R9  : regs::RCX, readSignedImm32(buf));
						case 2:  return MemoryRef(bits, mods.rex.B ? regs::R10 : regs::RDX, readSignedImm32(buf));
						case 3:  return MemoryRef(bits, mods.rex.B ? regs::R11 : regs::RBX, readSignedImm32(buf));
						case 5:  return MemoryRef(bits, mods.rex.B ? regs::R13 : regs::RBP, readSignedImm32(buf));
						case 6:  return MemoryRef(bits, mods.rex.B ? regs::R14 : regs::RSI, readSignedImm32(buf));
						case 7:  return MemoryRef(bits, mods.rex.B ? regs::R15 : regs::RDI, readSignedImm32(buf));

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
			auto idx = (mods.opcode & 0x0F) | (mods.rex.B << 3);
			return decodeRegisterNumber(regBits, mods, idx, rk);
		}
		else if(mods.modrm.mod != 3)
		{
			return getMemoryOperand(buf, memBits, mods);
		}
		else if(mods.modrm.mod == 3)
		{
			// not the same as the one above -- this uses the modrm.rm field, not the modrm.reg field.
			auto idx = mods.modrm.rm | (mods.rex.B << 3);
			return decodeRegisterNumber(regBits, mods, idx, rk);
		}

		// return a poison value
		return regs::INVALID;
	}

	Instruction read(Buffer& xs);
}
