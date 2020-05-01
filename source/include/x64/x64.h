// x64.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include <stdio.h>

#include "defs.h"

#include "ops.h"
#include "regs.h"
#include "table.h"

namespace instrad::x64
{
	struct MemoryRef
	{
		constexpr MemoryRef() { }

		constexpr explicit MemoryRef(int64_t d) : m_displacement(d) { }
		constexpr explicit MemoryRef(const Register& b) : m_base(b) { }

		constexpr MemoryRef(const Register& b, int64_t d) : m_displacement(d), m_base(b) { }
		constexpr MemoryRef(const Register& b, const Register& i) : m_base(b), m_index(i) { }

		constexpr MemoryRef(const Register& b, const Register& i, int s, int64_t d)
			: m_scale(s), m_displacement(d), m_base(b), m_index(i) { }

		constexpr int scale() const { return this->m_scale; }
		constexpr int64_t displacement() const { return this->m_displacement; }
		constexpr const Register& base() const { return this->m_base; }
		constexpr const Register& index() const { return this->m_index; }

		constexpr void setScale(int s) { this->m_scale = s; }
		constexpr void setDisplacement(int64_t d) { this->m_displacement = d; }
		constexpr void setIndex(const Register& i) { this->m_index = i; }
		constexpr void setBase(const Register& b) { this->m_base = b; }

	private:
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

		// if true, we are in 32-bit mode; if false, then 64-bit long mode.
		bool legacyAddressingMode = false;

		// some instructions (push, call, etc.) default to 64-bit operands in long-mode;
		// set this flag so the register decoder will return eg. RAX instead of EAX, without
		// needing REX.W to be present.
		bool default64BitRegister = false;

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


	constexpr MemoryRef decodeSIB(Buffer& buf, const InstrModifiers& mods)
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

		if(mods.rex.B)
		{
			if(sib.base == 5)
			{
				if(mods.modrm.mod == 0)
				{
					mem.setBase(regs::NONE);
					mem.setDisplacement(readSignedImm32(buf));
				}
				else if(mods.modrm.mod == 1)
				{
					mem.setBase(regs::R13);
					mem.setDisplacement(readSignedImm8(buf));
				}
				else if(mods.modrm.mod == 2)
				{
					mem.setBase(regs::R13);
					mem.setDisplacement(readSignedImm32(buf));
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
					case 0: mem.setBase(regs::R8); break;
					case 1: mem.setBase(regs::R9); break;
					case 2: mem.setBase(regs::R10); break;
					case 3: mem.setBase(regs::R11); break;
					case 4: mem.setBase(regs::R12); break;
					case 6: mem.setBase(regs::R14); break;
					case 7: mem.setBase(regs::R15); break;

					default: mem.setBase(regs::INVALID); break;
				}
			}
		}
		else
		{
			if(sib.base == 5)
			{
				if(mods.modrm.mod == 0)
				{
					mem.setBase(regs::NONE);
					mem.setDisplacement(readSignedImm32(buf));
				}
				else if(mods.modrm.mod == 1)
				{
					mem.setBase(regs::RBP);
					mem.setDisplacement(readSignedImm8(buf));
				}
				else if(mods.modrm.mod == 2)
				{
					mem.setBase(regs::RBP);
					mem.setDisplacement(readSignedImm32(buf));
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
					case 0: mem.setBase(regs::RAX); break;
					case 1: mem.setBase(regs::RCX); break;
					case 2: mem.setBase(regs::RDX); break;
					case 3: mem.setBase(regs::RBX); break;
					case 4: mem.setBase(regs::RSP); break;
					case 6: mem.setBase(regs::RSI); break;
					case 7: mem.setBase(regs::RDI); break;

					default: mem.setBase(regs::INVALID); break;
				}
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

	constexpr Register decodeRegisterNumber(bool isByteMode, const InstrModifiers& mods, int index)
	{
		if(isByteMode)
		{
			// if the REX is present, then we should use this set of registers:
			if(mods.rex.present())
			{
				switch(index)
				{
					case 0:  return regs::AL;
					case 1:  return regs::CL;
					case 2:  return regs::DL;
					case 3:  return regs::BL;
					case 4:  return regs::SPL;
					case 5:  return regs::BPL;
					case 6:  return regs::SIL;
					case 7:  return regs::DIL;
					case 8:  return regs::R8B;
					case 9:  return regs::R9B;
					case 10: return regs::R10B;
					case 11: return regs::R11B;
					case 12: return regs::R12B;
					case 13: return regs::R13B;
					case 14: return regs::R14B;
					case 15: return regs::R15B;
				}
			}
			else
			{
				// else use the other set.
				switch(index)
				{
					case 0: return regs::AL;
					case 1: return regs::CL;
					case 2: return regs::DL;
					case 3: return regs::BL;
					case 4: return regs::AH;
					case 5: return regs::CH;
					case 6: return regs::DH;
					case 7: return regs::BH;
				}
			}
		}
		else
		{
			if(mods.rex.W || mods.default64BitRegister)
			{
				// if REX.W, then we have a 64-bit operand. this has precedence over
				// the 0x66 operand size prefix, so check for this first.
				switch(index)
				{
					case 0:  return regs::RAX;
					case 1:  return regs::RCX;
					case 2:  return regs::RDX;
					case 3:  return regs::RBX;
					case 4:  return regs::RSP;
					case 5:  return regs::RBP;
					case 6:  return regs::RSI;
					case 7:  return regs::RDI;
					case 8:  return regs::R8;
					case 9:  return regs::R9;
					case 10: return regs::R10;
					case 11: return regs::R11;
					case 12: return regs::R12;
					case 13: return regs::R13;
					case 14: return regs::R14;
					case 15: return regs::R15;
				}
			}
			else if(mods.operandSizeOverride)
			{
				// no REX.W, so 16-bit
				switch(index)
				{
					case 0:  return regs::AX;
					case 1:  return regs::CX;
					case 2:  return regs::DX;
					case 3:  return regs::BX;
					case 4:  return regs::SP;
					case 5:  return regs::BP;
					case 6:  return regs::SI;
					case 7:  return regs::DI;
					case 8:  return regs::R8W;
					case 9:  return regs::R9W;
					case 10: return regs::R10W;
					case 11: return regs::R11W;
					case 12: return regs::R12W;
					case 13: return regs::R13W;
					case 14: return regs::R14W;
					case 15: return regs::R15W;
				}
			}
			else
			{
				// none of either prefix, so 32-bit.
				switch(index)
				{
					case 0:  return regs::EAX;
					case 1:  return regs::ECX;
					case 2:  return regs::EDX;
					case 3:  return regs::EBX;
					case 4:  return regs::ESP;
					case 5:  return regs::EBP;
					case 6:  return regs::ESI;
					case 7:  return regs::EDI;
					case 8:  return regs::R8D;
					case 9:  return regs::R9D;
					case 10: return regs::R10D;
					case 11: return regs::R11D;
					case 12: return regs::R12D;
					case 13: return regs::R13D;
					case 14: return regs::R14D;
					case 15: return regs::R15D;
				}
			}
		}

		return regs::INVALID;
	}


	constexpr Register getRegisterFromModRM(Buffer& buf, bool isByteMode, const InstrModifiers& mods)
	{
		(void) buf;

		if(mods.directRegisterIndex)
		{
			return decodeRegisterNumber(isByteMode, mods, (mods.opcode & 0x07) | (mods.rex.R << 3));
		}
		else
		{
			return decodeRegisterNumber(isByteMode, mods, mods.modrm.reg | (mods.rex.R << 3));
		}
	}

	constexpr MemoryRef getMemoryFromModRM(Buffer& buf, bool isByteMode, const InstrModifiers& mods)
	{
		if(mods.modrm.mod == 0)
		{
			// if 16-bit addressing:
			if(mods.legacyAddressingMode && mods.addressSizeOverride)
			{
				switch(mods.modrm.rm)
				{
					case 0: return MemoryRef(regs::BX, regs::SI);
					case 1: return MemoryRef(regs::BX, regs::DI);
					case 2: return MemoryRef(regs::BP, regs::SI);
					case 3: return MemoryRef(regs::BP, regs::DI);
					case 4: return MemoryRef(regs::SI);
					case 5: return MemoryRef(regs::DI);
					case 6: return MemoryRef(readSignedImm16(buf));
					case 7: return MemoryRef(regs::BX);
				}
			}
			else
			{
				// 32/64 bit addressing. we test REX.B inline here.
				switch(mods.modrm.rm)
				{
					case 0:  return MemoryRef(mods.rex.B ? regs::R8  : regs::RAX);
					case 1:  return MemoryRef(mods.rex.B ? regs::R9  : regs::RCX);
					case 2:  return MemoryRef(mods.rex.B ? regs::R10 : regs::RDX);
					case 3:  return MemoryRef(mods.rex.B ? regs::R11 : regs::RBX);
					case 4:  return decodeSIB(buf, mods);

					// in 64-bit mode, this becomes rip-relative addressing. if not, it is just disp32.
					case 5:  return mods.rex.present()  ? MemoryRef(regs::RIP, readSignedImm32(buf))
														: MemoryRef(readSignedImm32(buf));

					case 6:  return MemoryRef(mods.rex.B ? regs::R14 : regs::RSI);
					case 7:  return MemoryRef(mods.rex.B ? regs::R15 : regs::RDI);
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
					case 0: return MemoryRef(regs::BX, regs::SI, 0, readSignedImm8(buf));
					case 1: return MemoryRef(regs::BX, regs::DI, 0, readSignedImm8(buf));
					case 2: return MemoryRef(regs::BP, regs::SI, 0, readSignedImm8(buf));
					case 3: return MemoryRef(regs::BP, regs::DI, 0, readSignedImm8(buf));
					case 4: return MemoryRef(regs::SI, readSignedImm8(buf));
					case 5: return MemoryRef(regs::DI, readSignedImm8(buf));
					case 6: return MemoryRef(regs::BP, readSignedImm8(buf));
					case 7: return MemoryRef(regs::BX, readSignedImm8(buf));
				}
			}
			else
			{
				// 32/64 bit addressing
				switch(mods.modrm.rm)
				{
					case 0:  return MemoryRef(mods.rex.B ? regs::R8  : regs::RAX, readSignedImm8(buf));
					case 1:  return MemoryRef(mods.rex.B ? regs::R9  : regs::RCX, readSignedImm8(buf));
					case 2:  return MemoryRef(mods.rex.B ? regs::R10 : regs::RDX, readSignedImm8(buf));
					case 3:  return MemoryRef(mods.rex.B ? regs::R11 : regs::RBX, readSignedImm8(buf));
					case 4:  return decodeSIB(buf, mods);
					case 5:  return MemoryRef(mods.rex.B ? regs::R13 : regs::RBP, readSignedImm8(buf));
					case 6:  return MemoryRef(mods.rex.B ? regs::R14 : regs::RSI, readSignedImm8(buf));
					case 7:  return MemoryRef(mods.rex.B ? regs::R15 : regs::RDI, readSignedImm8(buf));
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
					case 0: return MemoryRef(regs::BX, regs::SI, 0, readSignedImm16(buf));
					case 1: return MemoryRef(regs::BX, regs::DI, 0, readSignedImm16(buf));
					case 2: return MemoryRef(regs::BP, regs::SI, 0, readSignedImm16(buf));
					case 3: return MemoryRef(regs::BP, regs::DI, 0, readSignedImm16(buf));
					case 4: return MemoryRef(regs::SI, readSignedImm16(buf));
					case 5: return MemoryRef(regs::DI, readSignedImm16(buf));
					case 6: return MemoryRef(regs::BP, readSignedImm16(buf));
					case 7: return MemoryRef(regs::BX, readSignedImm16(buf));
				}
			}
			else
			{
				// 32/64 bit addressing
				switch(mods.modrm.rm)
				{
					case 0:  return MemoryRef(mods.rex.B ? regs::R8  : regs::RAX, readSignedImm32(buf));
					case 1:  return MemoryRef(mods.rex.B ? regs::R9  : regs::RCX, readSignedImm32(buf));
					case 2:  return MemoryRef(mods.rex.B ? regs::R10 : regs::RDX, readSignedImm32(buf));
					case 3:  return MemoryRef(mods.rex.B ? regs::R11 : regs::RBX, readSignedImm32(buf));
					case 4:  return decodeSIB(buf, mods);
					case 5:  return MemoryRef(mods.rex.B ? regs::R13 : regs::RBP, readSignedImm32(buf));
					case 6:  return MemoryRef(mods.rex.B ? regs::R14 : regs::RSI, readSignedImm32(buf));
					case 7:  return MemoryRef(mods.rex.B ? regs::R15 : regs::RDI, readSignedImm32(buf));
				}
			}
		}

		return MemoryRef();
	}

	constexpr Operand getRegisterOrMemoryFromModRM(Buffer& buf, bool isByteMode, const InstrModifiers& mods)
	{
		if(mods.directRegisterIndex)
		{
			auto idx = (mods.opcode & 0x0F) | (mods.rex.B << 3);
			return decodeRegisterNumber(/* byte: */ false, mods, idx);
		}
		else if(mods.modrm.mod != 3)
		{
			return getMemoryFromModRM(buf, isByteMode, mods);
		}
		else if(mods.modrm.mod == 3)
		{
			// not the same as the one above -- this uses the modrm.rm field, not the modrm.reg field.
			return decodeRegisterNumber(isByteMode, mods, mods.modrm.rm | (mods.rex.B << 3));
		}

		// return a poison value
		return regs::INVALID;
	}

	Instruction read(Buffer& xs);
}
