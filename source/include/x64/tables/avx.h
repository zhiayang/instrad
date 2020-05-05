// avx.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "entry.h"

namespace instrad::x64
{
	// VEX is a special child.
	// note: to keep things aligned, we use "mod0" and "mod3", even though the distinction
	// is actually mod == 3 and mod != 3. oh well.

	struct VexEntry
	{
	private:
		using TE = TableEntry;
		using VE = VexEntry;
	public:
		constexpr VexEntry() : m_opcode(0), m_present(false) { }
		constexpr explicit VexEntry(uint8_t opcode) : m_opcode(opcode), m_present(true) { }
		constexpr VexEntry(uint8_t opcode, const VexEntry* ext) : m_opcode(opcode), m_present(false), m_extension(ext) { }

		constexpr uint8_t opcode() const { return this->m_opcode; }
		constexpr bool present() const { return this->m_present; }

		constexpr VE& pNN_W0_L0_mod3(TE e)  { this->entries[0x00] = e; return *this; }
		constexpr VE& pNN_W0_L1_mod3(TE e)  { this->entries[0x01] = e; return *this; }
		constexpr VE& pNN_W1_L0_mod3(TE e)  { this->entries[0x02] = e; return *this; }
		constexpr VE& pNN_W1_L1_mod3(TE e)  { this->entries[0x03] = e; return *this; }
		constexpr VE& p66_W0_L0_mod3(TE e)  { this->entries[0x04] = e; return *this; }
		constexpr VE& p66_W0_L1_mod3(TE e)  { this->entries[0x05] = e; return *this; }
		constexpr VE& p66_W1_L0_mod3(TE e)  { this->entries[0x06] = e; return *this; }
		constexpr VE& p66_W1_L1_mod3(TE e)  { this->entries[0x07] = e; return *this; }
		constexpr VE& pF2_W0_L0_mod3(TE e)  { this->entries[0x08] = e; return *this; }
		constexpr VE& pF2_W0_L1_mod3(TE e)  { this->entries[0x09] = e; return *this; }
		constexpr VE& pF2_W1_L0_mod3(TE e)  { this->entries[0x0A] = e; return *this; }
		constexpr VE& pF2_W1_L1_mod3(TE e)  { this->entries[0x0B] = e; return *this; }
		constexpr VE& pF3_W0_L0_mod3(TE e)  { this->entries[0x0C] = e; return *this; }
		constexpr VE& pF3_W0_L1_mod3(TE e)  { this->entries[0x0D] = e; return *this; }
		constexpr VE& pF3_W1_L0_mod3(TE e)  { this->entries[0x0E] = e; return *this; }
		constexpr VE& pF3_W1_L1_mod3(TE e)  { this->entries[0x0F] = e; return *this; }
		constexpr VE& pNN_W0_L0_mod0(TE e)  { this->entries[0x10] = e; return *this; }
		constexpr VE& pNN_W0_L1_mod0(TE e)  { this->entries[0x11] = e; return *this; }
		constexpr VE& pNN_W1_L0_mod0(TE e)  { this->entries[0x12] = e; return *this; }
		constexpr VE& pNN_W1_L1_mod0(TE e)  { this->entries[0x13] = e; return *this; }
		constexpr VE& p66_W0_L0_mod0(TE e)  { this->entries[0x14] = e; return *this; }
		constexpr VE& p66_W0_L1_mod0(TE e)  { this->entries[0x15] = e; return *this; }
		constexpr VE& p66_W1_L0_mod0(TE e)  { this->entries[0x16] = e; return *this; }
		constexpr VE& p66_W1_L1_mod0(TE e)  { this->entries[0x17] = e; return *this; }
		constexpr VE& pF2_W0_L0_mod0(TE e)  { this->entries[0x18] = e; return *this; }
		constexpr VE& pF2_W0_L1_mod0(TE e)  { this->entries[0x19] = e; return *this; }
		constexpr VE& pF2_W1_L0_mod0(TE e)  { this->entries[0x1A] = e; return *this; }
		constexpr VE& pF2_W1_L1_mod0(TE e)  { this->entries[0x1B] = e; return *this; }
		constexpr VE& pF3_W0_L0_mod0(TE e)  { this->entries[0x1C] = e; return *this; }
		constexpr VE& pF3_W0_L1_mod0(TE e)  { this->entries[0x1D] = e; return *this; }
		constexpr VE& pF3_W1_L0_mod0(TE e)  { this->entries[0x1E] = e; return *this; }
		constexpr VE& pF3_W1_L1_mod0(TE e)  { this->entries[0x1F] = e; return *this; }

		// now, some convenience functions
		constexpr VE& pNN_L0_mod3(TE e)     { return pNN_W0_L0_mod3(e).pNN_W1_L0_mod3(e); }
		constexpr VE& pNN_L1_mod3(TE e)     { return pNN_W0_L1_mod3(e).pNN_W1_L1_mod3(e); }
		constexpr VE& pNN_W0_mod3(TE e)     { return pNN_W0_L0_mod3(e).pNN_W0_L1_mod3(e); }
		constexpr VE& pNN_W1_mod3(TE e)     { return pNN_W1_L0_mod3(e).pNN_W1_L1_mod3(e); }
		constexpr VE& p66_L0_mod3(TE e)     { return p66_W0_L0_mod3(e).p66_W1_L0_mod3(e); }
		constexpr VE& p66_L1_mod3(TE e)     { return p66_W0_L1_mod3(e).p66_W1_L1_mod3(e); }
		constexpr VE& p66_W0_mod3(TE e)     { return p66_W0_L0_mod3(e).p66_W0_L1_mod3(e); }
		constexpr VE& p66_W1_mod3(TE e)     { return p66_W1_L0_mod3(e).p66_W1_L1_mod3(e); }
		constexpr VE& pF2_L0_mod3(TE e)     { return pF2_W0_L0_mod3(e).pF2_W1_L0_mod3(e); }
		constexpr VE& pF2_L1_mod3(TE e)     { return pF2_W0_L1_mod3(e).pF2_W1_L1_mod3(e); }
		constexpr VE& pF2_W0_mod3(TE e)     { return pF2_W0_L0_mod3(e).pF2_W0_L1_mod3(e); }
		constexpr VE& pF2_W1_mod3(TE e)     { return pF2_W1_L0_mod3(e).pF2_W1_L1_mod3(e); }
		constexpr VE& pF3_L0_mod3(TE e)     { return pF3_W0_L0_mod3(e).pF3_W1_L0_mod3(e); }
		constexpr VE& pF3_L1_mod3(TE e)     { return pF3_W0_L1_mod3(e).pF3_W1_L1_mod3(e); }
		constexpr VE& pF3_W0_mod3(TE e)     { return pF3_W0_L0_mod3(e).pF3_W0_L1_mod3(e); }
		constexpr VE& pF3_W1_mod3(TE e)     { return pF3_W1_L0_mod3(e).pF3_W1_L1_mod3(e); }
		constexpr VE& pNN_L0_mod0(TE e)     { return pNN_W0_L0_mod0(e).pNN_W1_L0_mod0(e); }
		constexpr VE& pNN_L1_mod0(TE e)     { return pNN_W0_L1_mod0(e).pNN_W1_L1_mod0(e); }
		constexpr VE& pNN_W0_mod0(TE e)     { return pNN_W0_L0_mod0(e).pNN_W0_L1_mod0(e); }
		constexpr VE& pNN_W1_mod0(TE e)     { return pNN_W1_L0_mod0(e).pNN_W1_L1_mod0(e); }
		constexpr VE& p66_L0_mod0(TE e)     { return p66_W0_L0_mod0(e).p66_W1_L0_mod0(e); }
		constexpr VE& p66_L1_mod0(TE e)     { return p66_W0_L1_mod0(e).p66_W1_L1_mod0(e); }
		constexpr VE& p66_W0_mod0(TE e)     { return p66_W0_L0_mod0(e).p66_W0_L1_mod0(e); }
		constexpr VE& p66_W1_mod0(TE e)     { return p66_W1_L0_mod0(e).p66_W1_L1_mod0(e); }
		constexpr VE& pF2_L0_mod0(TE e)     { return pF2_W0_L0_mod0(e).pF2_W1_L0_mod0(e); }
		constexpr VE& pF2_L1_mod0(TE e)     { return pF2_W0_L1_mod0(e).pF2_W1_L1_mod0(e); }
		constexpr VE& pF2_W0_mod0(TE e)     { return pF2_W0_L0_mod0(e).pF2_W0_L1_mod0(e); }
		constexpr VE& pF2_W1_mod0(TE e)     { return pF2_W1_L0_mod0(e).pF2_W1_L1_mod0(e); }
		constexpr VE& pF3_L0_mod0(TE e)     { return pF3_W0_L0_mod0(e).pF3_W1_L0_mod0(e); }
		constexpr VE& pF3_L1_mod0(TE e)     { return pF3_W0_L1_mod0(e).pF3_W1_L1_mod0(e); }
		constexpr VE& pF3_W0_mod0(TE e)     { return pF3_W0_L0_mod0(e).pF3_W0_L1_mod0(e); }
		constexpr VE& pF3_W1_mod0(TE e)     { return pF3_W1_L0_mod0(e).pF3_W1_L1_mod0(e); }

		constexpr VE& pNN_W0_L0(TE e)       { return pNN_W0_L0_mod3(e).pNN_W0_L0_mod0(e); }
		constexpr VE& pNN_W0_L1(TE e)       { return pNN_W0_L1_mod3(e).pNN_W0_L1_mod0(e); }
		constexpr VE& pNN_W1_L0(TE e)       { return pNN_W1_L0_mod3(e).pNN_W1_L0_mod0(e); }
		constexpr VE& pNN_W1_L1(TE e)       { return pNN_W1_L1_mod3(e).pNN_W1_L1_mod0(e); }
		constexpr VE& p66_W0_L0(TE e)       { return p66_W0_L0_mod3(e).p66_W0_L0_mod0(e); }
		constexpr VE& p66_W0_L1(TE e)       { return p66_W0_L1_mod3(e).p66_W0_L1_mod0(e); }
		constexpr VE& p66_W1_L0(TE e)       { return p66_W1_L0_mod3(e).p66_W1_L0_mod0(e); }
		constexpr VE& p66_W1_L1(TE e)       { return p66_W1_L1_mod3(e).p66_W1_L1_mod0(e); }
		constexpr VE& pF2_W0_L0(TE e)       { return pF2_W0_L0_mod3(e).pF2_W0_L0_mod0(e); }
		constexpr VE& pF2_W0_L1(TE e)       { return pF2_W0_L1_mod3(e).pF2_W0_L1_mod0(e); }
		constexpr VE& pF2_W1_L0(TE e)       { return pF2_W1_L0_mod3(e).pF2_W1_L0_mod0(e); }
		constexpr VE& pF2_W1_L1(TE e)       { return pF2_W1_L1_mod3(e).pF2_W1_L1_mod0(e); }
		constexpr VE& pF3_W0_L0(TE e)       { return pF3_W0_L0_mod3(e).pF3_W0_L0_mod0(e); }
		constexpr VE& pF3_W0_L1(TE e)       { return pF3_W0_L1_mod3(e).pF3_W0_L1_mod0(e); }
		constexpr VE& pF3_W1_L0(TE e)       { return pF3_W1_L0_mod3(e).pF3_W1_L0_mod0(e); }
		constexpr VE& pF3_W1_L1(TE e)       { return pF3_W1_L1_mod3(e).pF3_W1_L1_mod0(e); }

		constexpr VE& pNN_L0(TE e)          { return pNN_W0_L0_mod3(e).pNN_W1_L0_mod0(e); }
		constexpr VE& pNN_L1(TE e)          { return pNN_W0_L1_mod3(e).pNN_W1_L1_mod0(e); }
		constexpr VE& pNN_W0(TE e)          { return pNN_W0_L0_mod3(e).pNN_W0_L1_mod0(e); }
		constexpr VE& pNN_W1(TE e)          { return pNN_W1_L0_mod3(e).pNN_W1_L1_mod0(e); }
		constexpr VE& p66_L0(TE e)          { return p66_W0_L0_mod3(e).p66_W1_L0_mod0(e); }
		constexpr VE& p66_L1(TE e)          { return p66_W0_L1_mod3(e).p66_W1_L1_mod0(e); }
		constexpr VE& p66_W0(TE e)          { return p66_W0_L0_mod3(e).p66_W0_L1_mod0(e); }
		constexpr VE& p66_W1(TE e)          { return p66_W1_L0_mod3(e).p66_W1_L1_mod0(e); }
		constexpr VE& pF2_L0(TE e)          { return pF2_W0_L0_mod3(e).pF2_W1_L0_mod0(e); }
		constexpr VE& pF2_L1(TE e)          { return pF2_W0_L1_mod3(e).pF2_W1_L1_mod0(e); }
		constexpr VE& pF2_W0(TE e)          { return pF2_W0_L0_mod3(e).pF2_W0_L1_mod0(e); }
		constexpr VE& pF2_W1(TE e)          { return pF2_W1_L0_mod3(e).pF2_W1_L1_mod0(e); }
		constexpr VE& pF3_L0(TE e)          { return pF3_W0_L0_mod3(e).pF3_W1_L0_mod0(e); }
		constexpr VE& pF3_L1(TE e)          { return pF3_W0_L1_mod3(e).pF3_W1_L1_mod0(e); }
		constexpr VE& pF3_W0(TE e)          { return pF3_W0_L0_mod3(e).pF3_W0_L1_mod0(e); }
		constexpr VE& pF3_W1(TE e)          { return pF3_W1_L0_mod3(e).pF3_W1_L1_mod0(e); }

		constexpr VE& pNN_mod3(TE e)        { return pNN_W0_L0_mod3(e).pNN_W0_L1_mod3(e).pNN_W1_L0_mod3(e).pNN_W1_L1_mod3(e); }
		constexpr VE& p66_mod3(TE e)        { return p66_W0_L0_mod3(e).p66_W0_L1_mod3(e).p66_W1_L0_mod3(e).p66_W1_L1_mod3(e); }
		constexpr VE& pF2_mod3(TE e)        { return pF2_W0_L0_mod3(e).pF2_W0_L1_mod3(e).pF2_W1_L0_mod3(e).pF2_W1_L1_mod3(e); }
		constexpr VE& pF3_mod3(TE e)        { return pF3_W0_L0_mod3(e).pF3_W0_L1_mod3(e).pF3_W1_L0_mod3(e).pF3_W1_L1_mod3(e); }
		constexpr VE& pNN_mod0(TE e)        { return pNN_W0_L0_mod0(e).pNN_W0_L1_mod0(e).pNN_W1_L0_mod0(e).pNN_W1_L1_mod0(e); }
		constexpr VE& p66_mod0(TE e)        { return p66_W0_L0_mod0(e).p66_W0_L1_mod0(e).p66_W1_L0_mod0(e).p66_W1_L1_mod0(e); }
		constexpr VE& pF2_mod0(TE e)        { return pF2_W0_L0_mod0(e).pF2_W0_L1_mod0(e).pF2_W1_L0_mod0(e).pF2_W1_L1_mod0(e); }
		constexpr VE& pF3_mod0(TE e)        { return pF3_W0_L0_mod0(e).pF3_W0_L1_mod0(e).pF3_W1_L0_mod0(e).pF3_W1_L1_mod0(e); }
		constexpr VE& pNN(TE e)             { return pNN_mod3(e).pNN_mod0(e); }
		constexpr VE& p66(TE e)             { return p66_mod3(e).p66_mod0(e); }
		constexpr VE& pF2(TE e)             { return pF2_mod3(e).pF2_mod0(e); }
		constexpr VE& pF3(TE e)             { return pF3_mod3(e).pF3_mod0(e); }

		constexpr const VexEntry* extension() const { return this->m_extension; }
		constexpr VE& noModRM() { this->m_needsModRM = false; return *this; }

		constexpr bool needsModRM() const { return this->m_needsModRM; }

	private:
		uint8_t m_opcode;
		bool m_present;

		bool m_needsModRM = true;
		const VexEntry* m_extension = nullptr;

		// keep an internal table of 32 values. see the methods for the indices.
		TableEntry entries[32] = { };

		friend struct VexEntryDecoder;
	};

	struct VexEntryDecoder
	{
		constexpr VexEntryDecoder(const VexEntry& ent) : m_ent(ent) { }

		constexpr VexEntryDecoder& setMod3()    { this->m_mod3 = true; return *this; }
		constexpr VexEntryDecoder& setModNot3() { this->m_mod3 = false; return *this; }
		constexpr VexEntryDecoder& setVexW()    { this->m_W = true; return *this; }
		constexpr VexEntryDecoder& setVexL()    { this->m_L = true; return *this; }
		constexpr VexEntryDecoder& setPref66()  { this->m_66 = true; return *this; }
		constexpr VexEntryDecoder& setPrefF2()  { this->m_F2 = true; return *this; }
		constexpr VexEntryDecoder& setPrefF3()  { this->m_F3 = true; return *this; }

		constexpr TableEntry get()
		{
			size_t idx = 0;
			if(!m_mod3) idx |= 0x10;
			if(m_66)    idx |= 0x04;
			if(m_F2)    idx |= 0x08;
			if(m_F3)    idx |= 0x0C;
			if(m_W)     idx |= 0x02;
			if(m_L)     idx |= 0x01;

			return m_ent.entries[idx];
		}

	private:
		bool m_mod3 = false;
		bool m_W = false;
		bool m_L = false;
		bool m_66 = false;
		bool m_F2 = false;
		bool m_F3 = false;

		const VexEntry& m_ent;
	};

namespace tables
{
	constexpr VexEntry Vex_Map_1_ModRMExt_71[] = {
		[2] = VexEntry(0x71)
				.p66_L0(entry_3(0x71, ops::VPSRLW, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x71, ops::VPSRLW, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,
		[4] = VexEntry(0x71)
				.p66_L0(entry_3(0x71, ops::VPSRAW, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x71, ops::VPSRAW, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,
		[6] = VexEntry(0x71)
				.p66_L0(entry_3(0x71, ops::VPSLLW, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x71, ops::VPSLLW, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,

		[7] = VexEntry(0),
	};

	constexpr VexEntry Vex_Map_1_ModRMExt_72[] = {
		[2] = VexEntry(0x72)
				.p66_L0(entry_3(0x72, ops::VPSRLD, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x72, ops::VPSRLD, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,
		[4] = VexEntry(0x72)
				.p66_L0(entry_3(0x72, ops::VPSRAD, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x72, ops::VPSRAD, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,
		[6] = VexEntry(0x72)
				.p66_L0(entry_3(0x72, ops::VPSLLD, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x72, ops::VPSLLD, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,

		[7] = VexEntry(0),
	};

	constexpr VexEntry Vex_Map_1_ModRMExt_73[] = {
		[2] = VexEntry(0x73)
				.p66_L0(entry_3(0x73, ops::VPSRLQ, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x73, ops::VPSRLQ, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,
		[3] = VexEntry(0x73)
				.p66_L0(entry_3(0x73, ops::VPSRLDQ, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x73, ops::VPSRLDQ, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,
		[6] = VexEntry(0x73)
				.p66_L0(entry_3(0x73, ops::VPSLLQ, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x73, ops::VPSLLQ, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,
		[7] = VexEntry(0x73)
				.p66_L0(entry_3(0x73, ops::VPSLLDQ, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm, OpKind::Imm8))
				.p66_L1(entry_3(0x73, ops::VPSLLDQ, OpKind::RegYmm_vvvv, OpKind::RegYmm_Rm, OpKind::Imm8))
				,
	};


	constexpr VexEntry Vex_Map_1_ModRMExt_AE[] = {
		[2] = VexEntry(0xAE)
				.pNN_L0(entry_1(0xAE, ops::VLDMXCSR, OpKind::Mem32))
				,
		[3] = VexEntry(0xAE)
				.pNN_L0(entry_1(0xAE, ops::VSTMXCSR, OpKind::Mem32))
				,

		[7] = VexEntry(0)
	};




	constexpr VexEntry VEX_Map_1[] = {
		[0x10] = VexEntry(0x10)
					.pNN_L0(entry_2(0x10, ops::VMOVUPS, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pNN_L1(entry_2(0x10, ops::VMOVUPS, OpKind::RegYmm, OpKind::RegYmmMem256))
					.p66_L0(entry_2(0x10, ops::VMOVUPD, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x10, ops::VMOVUPD, OpKind::RegYmm, OpKind::RegYmmMem256))
					.pF2_mod3(entry_3(0x10, ops::VMOVSD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm))
					.pF2_mod0(entry_2(0x10, ops::VMOVSD, OpKind::RegXmm, OpKind::Mem64))
					.pF3_mod3(entry_3(0x10, ops::VMOVSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmm_Rm))
					.pF3_mod0(entry_2(0x10, ops::VMOVSS, OpKind::RegXmm, OpKind::Mem32))
					,

		[0x11] = VexEntry(0x11)
					.pNN_L0(entry_2(0x11, ops::VMOVUPS, OpKind::RegXmmMem128, OpKind::RegXmm))
					.pNN_L1(entry_2(0x11, ops::VMOVUPS, OpKind::RegYmmMem256, OpKind::RegYmm))
					.p66_L0(entry_2(0x11, ops::VMOVUPD, OpKind::RegXmmMem128, OpKind::RegXmm))
					.p66_L1(entry_2(0x11, ops::VMOVUPD, OpKind::RegYmmMem256, OpKind::RegYmm))
					.pF2_mod3(entry_3(0x11, ops::VMOVSD, OpKind::RegXmm_Rm, OpKind::RegXmm_vvvv, OpKind::RegXmm))
					.pF2_mod0(entry_2(0x11, ops::VMOVSD, OpKind::Mem64, OpKind::RegXmm))
					.pF3_mod3(entry_3(0x11, ops::VMOVSS, OpKind::RegXmm_Rm, OpKind::RegXmm_vvvv, OpKind::RegXmm))
					.pF3_mod0(entry_2(0x11, ops::VMOVSS, OpKind::Mem32, OpKind::RegXmm))
					,

		[0x12] = VexEntry(0x12)
					.pNN_L0(entry_3(0x12, ops::VMOVLPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::Mem64))
					.p66_L0(entry_3(0x12, ops::VMOVLPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::Mem64))
					.pF2_L0(entry_2(0x12, ops::VMOVDDUP, OpKind::RegXmm, OpKind::RegXmmMem64))
					.pF2_L1(entry_2(0x12, ops::VMOVDDUP, OpKind::RegYmm, OpKind::RegYmmMem256))
					.pF3_L0(entry_2(0x12, ops::VMOVSLDUP, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pF3_L1(entry_2(0x12, ops::VMOVSLDUP, OpKind::RegYmm, OpKind::RegYmmMem256))
					,

		[0x13] = VexEntry(0x13)
					.pNN_L0(entry_2(0x13, ops::VMOVLPS, OpKind::Mem64, OpKind::RegXmm))
					.p66_L0(entry_2(0x13, ops::VMOVLPD, OpKind::Mem64, OpKind::RegXmm))
					,

		[0x14] = VexEntry(0x14)
					.pNN_L0(entry_3(0x14, ops::VUNPCKLPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x14, ops::VUNPCKLPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x14, ops::VUNPCKLPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x14, ops::VUNPCKLPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x15] = VexEntry(0x15)
					.pNN_L0(entry_3(0x15, ops::VUNPCKHPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x15, ops::VUNPCKHPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x15, ops::VUNPCKHPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x15, ops::VUNPCKHPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x16] = VexEntry(0x16)
					.pNN_L0(entry_3(0x16, ops::VMOVHPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::Mem64))
					.p66_L0(entry_3(0x16, ops::VMOVHPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::Mem64))
					.pF3_L0(entry_3(0x16, ops::VMOVSHDUP, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pF3_L1(entry_3(0x16, ops::VMOVSHDUP, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x17] = VexEntry(0x17)
					.pNN_L0(entry_2(0x17, ops::VMOVHPS, OpKind::Mem64, OpKind::RegXmm))
					.p66_L0(entry_2(0x17, ops::VMOVHPD, OpKind::Mem64, OpKind::RegXmm))
					,



		[0x28] = VexEntry(0x28)
					.pNN_L0(entry_2(0x28, ops::VMOVAPS, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pNN_L1(entry_2(0x28, ops::VMOVAPS, OpKind::RegYmm, OpKind::RegYmmMem256))
					.p66_L0(entry_2(0x28, ops::VMOVAPD, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x28, ops::VMOVAPD, OpKind::RegYmm, OpKind::RegYmmMem256))
					,

		[0x29] = VexEntry(0x29)
					.pNN_L0(entry_2(0x29, ops::VMOVAPS, OpKind::RegYmmMem256, OpKind::RegXmm))
					.pNN_L1(entry_2(0x29, ops::VMOVAPS, OpKind::RegXmmMem128, OpKind::RegYmm))
					.p66_L0(entry_2(0x29, ops::VMOVAPD, OpKind::RegYmmMem256, OpKind::RegXmm))
					.p66_L1(entry_2(0x29, ops::VMOVAPD, OpKind::RegXmmMem128, OpKind::RegYmm))
					,

		[0x2A] = VexEntry(0x2A)
					.pF2_W0(entry_3(0x2A, ops::VCVTSI2SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegMem32))
					.pF2_W1(entry_3(0x2A, ops::VCVTSI2SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegMem64))
					.pF3_W0(entry_3(0x2A, ops::VCVTSI2SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegMem32))
					.pF3_W1(entry_3(0x2A, ops::VCVTSI2SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegMem64))
					,

		[0x2B] = VexEntry(0x2B)
					.pNN_L0(entry_2(0x2B, ops::VMOVNTPS, OpKind::Mem128, OpKind::RegXmm))
					.p66_L1(entry_2(0x2B, ops::VMOVNTPD, OpKind::Mem256, OpKind::RegYmm))
					,

		[0x2C] = VexEntry(0x2C)
					.pF2_W0(entry_2(0x2C, ops::VCVTTSD2SI, OpKind::Reg32, OpKind::RegXmmMem64))
					.pF2_W1(entry_2(0x2C, ops::VCVTTSD2SI, OpKind::Reg64, OpKind::RegXmmMem64))
					.pF3_W0(entry_2(0x2C, ops::VCVTTSS2SI, OpKind::Reg32, OpKind::RegXmmMem64))
					.pF3_W1(entry_2(0x2C, ops::VCVTTSS2SI, OpKind::Reg64, OpKind::RegXmmMem64))
					,

		[0x2D] = VexEntry(0x2D)
					.pF2_W0(entry_2(0x2D, ops::VCVTSD2SI, OpKind::Reg32, OpKind::RegXmmMem64))
					.pF2_W1(entry_2(0x2D, ops::VCVTSD2SI, OpKind::Reg64, OpKind::RegXmmMem64))
					.pF3_W0(entry_2(0x2D, ops::VCVTSS2SI, OpKind::Reg32, OpKind::RegXmmMem64))
					.pF3_W1(entry_2(0x2D, ops::VCVTSS2SI, OpKind::Reg64, OpKind::RegXmmMem64))
					,

		[0x2E] = VexEntry(0x2E)
					.pNN(entry_2(0x2E, ops::VUCOMISS, OpKind::RegXmm, OpKind::RegXmmMem32))
					.p66(entry_2(0x2E, ops::VUCOMISD, OpKind::RegXmm, OpKind::RegXmmMem64))
					,

		[0x2F] = VexEntry(0x2F)
					.pNN(entry_2(0x2F, ops::VCOMISS, OpKind::RegXmm, OpKind::RegXmmMem32))
					.p66(entry_2(0x2F, ops::VCOMISD, OpKind::RegXmm, OpKind::RegXmmMem64))
					,




		[0x50] = VexEntry(0x50)
					.pNN_L0(entry_2(0x50, ops::VMOVMSKPS, OpKind::Reg32, OpKind::RegXmm_Rm))
					.pNN_L1(entry_2(0x50, ops::VMOVMSKPS, OpKind::Reg32, OpKind::RegYmm_Rm))
					.p66_L0(entry_2(0x50, ops::VMOVMSKPD, OpKind::Reg32, OpKind::RegXmm_Rm))
					.p66_L1(entry_2(0x50, ops::VMOVMSKPD, OpKind::Reg32, OpKind::RegYmm_Rm))
					,

		[0x51] = VexEntry(0x51)
					.pNN_L0(entry_2(0x51, ops::VSQRTPS, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pNN_L1(entry_2(0x51, ops::VSQRTPS, OpKind::RegYmm, OpKind::RegYmmMem256))
					.p66_L0(entry_2(0x51, ops::VSQRTPD, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x51, ops::VSQRTPD, OpKind::RegYmm, OpKind::RegYmmMem256))
					.pF2(entry_3(0x51, ops::VSQRTSD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem64))
					.pF2(entry_3(0x51, ops::VSQRTSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x52] = VexEntry(0x52)
					.pNN_L0(entry_2(0x52, ops::VRSQRTPS, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pNN_L1(entry_2(0x52, ops::VRSQRTPS, OpKind::RegYmm, OpKind::RegYmmMem256))
					.pF3(entry_3(0x52, ops::VRSQRTSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x53] = VexEntry(0x53)
					.pNN_L0(entry_2(0x53, ops::VRCPPS, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pNN_L1(entry_2(0x53, ops::VRCPPS, OpKind::RegYmm, OpKind::RegYmmMem256))
					.pF3(entry_3(0x53, ops::VRCPSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x54] = VexEntry(0x54)
					.pNN_L0(entry_3(0x54, ops::VANDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x54, ops::VANDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x54, ops::VANDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x54, ops::VANDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x55] = VexEntry(0x55)
					.pNN_L0(entry_3(0x55, ops::VANDNPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x55, ops::VANDNPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x55, ops::VANDNPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x55, ops::VANDNPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x56] = VexEntry(0x56)
					.pNN_L0(entry_3(0x56, ops::VORPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x56, ops::VORPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x56, ops::VORPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x56, ops::VORPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x57] = VexEntry(0x57)
					.pNN_L0(entry_3(0x57, ops::VXORPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x57, ops::VXORPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x57, ops::VXORPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x57, ops::VXORPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,



		[0x58] = VexEntry(0x58)
					.pNN_L0(entry_3(0x58, ops::VADDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x58, ops::VADDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x58, ops::VADDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x58, ops::VADDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.pF2(entry_3(0x58, ops::VADDSD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem64))
					.pF3(entry_3(0x58, ops::VADDSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x59] = VexEntry(0x59)
					.pNN_L0(entry_3(0x59, ops::VMULPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x59, ops::VMULPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x59, ops::VMULPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x59, ops::VMULPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.pF2(entry_3(0x59, ops::VMULSD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem64))
					.pF3(entry_3(0x59, ops::VMULSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x5A] = VexEntry(0x5A)
					.pNN_L0(entry_2(0x5A, ops::VCVTPS2PD, OpKind::RegXmm, OpKind::RegXmmMem64))
					.pNN_L1(entry_2(0x5A, ops::VCVTPS2PD, OpKind::RegYmm, OpKind::RegXmmMem128))
					.p66_L0(entry_2(0x5A, ops::VCVTPD2PS, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x5A, ops::VCVTPD2PS, OpKind::RegXmm, OpKind::RegYmmMem256))
					.pF3(entry_3(0x5A, ops::VCVTSD2SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem64))
					.pF2(entry_3(0x5A, ops::VCVTSS2SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x5B] = VexEntry(0x5B)
					.pNN_L0(entry_2(0x5B, ops::VCVTDQ2PS, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pNN_L1(entry_2(0x5B, ops::VCVTDQ2PS, OpKind::RegYmm, OpKind::RegYmmMem256))
					.p66_L0(entry_2(0x5B, ops::VCVTPS2DQ, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x5B, ops::VCVTPS2DQ, OpKind::RegYmm, OpKind::RegYmmMem256))
					.pF3_L0(entry_2(0x5B, ops::VCVTTPS2DQ, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pF3_L1(entry_2(0x5B, ops::VCVTTPS2DQ, OpKind::RegYmm, OpKind::RegYmmMem256))
					,

		[0x5C] = VexEntry(0x5C)
					.pNN_L0(entry_3(0x5C, ops::VSUBPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x5C, ops::VSUBPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x5C, ops::VSUBPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x5C, ops::VSUBPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.pF2(entry_3(0x5C, ops::VSUBSD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem64))
					.pF3(entry_3(0x5C, ops::VSUBSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x5D] = VexEntry(0x5D)
					.pNN_L0(entry_3(0x5D, ops::VMINPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x5D, ops::VMINPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x5D, ops::VMINPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x5D, ops::VMINPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.pF2(entry_3(0x5D, ops::VMINSD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem64))
					.pF3(entry_3(0x5D, ops::VMINSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x5E] = VexEntry(0x5E)
					.pNN_L0(entry_3(0x5E, ops::VDIVPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x5E, ops::VDIVPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x5E, ops::VDIVPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x5E, ops::VDIVPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.pF2(entry_3(0x5E, ops::VDIVSD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem64))
					.pF3(entry_3(0x5E, ops::VDIVSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x5F] = VexEntry(0x5F)
					.pNN_L0(entry_3(0x5F, ops::VMAXPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pNN_L1(entry_3(0x5F, ops::VMAXPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_L0(entry_3(0x5F, ops::VMAXPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x5F, ops::VMAXPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.pF2(entry_3(0x5F, ops::VMAXSD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem64))
					.pF3(entry_3(0x5F, ops::VMAXSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x60] = VexEntry(0x60)
					.p66_L0(entry_3(0x60, ops::VPUNPCKLBW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x60, ops::VPUNPCKLBW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x61] = VexEntry(0x61)
					.p66_L0(entry_3(0x61, ops::VPUNPCKLWD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x61, ops::VPUNPCKLWD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x62] = VexEntry(0x62)
					.p66_L0(entry_3(0x62, ops::VPUNPCKLDQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x62, ops::VPUNPCKLDQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x63] = VexEntry(0x63)
					.p66_L0(entry_3(0x63, ops::VPACKSSWB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x63, ops::VPACKSSWB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x64] = VexEntry(0x64)
					.p66_L0(entry_3(0x64, ops::VPCMPGTB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x64, ops::VPCMPGTB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x65] = VexEntry(0x65)
					.p66_L0(entry_3(0x65, ops::VPCMPGTW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x65, ops::VPCMPGTW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x66] = VexEntry(0x66)
					.p66_L0(entry_3(0x66, ops::VPCMPGTD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x66, ops::VPCMPGTD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x67] = VexEntry(0x67)
					.p66_L0(entry_3(0x67, ops::VPACKUSWB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x67, ops::VPACKUSWB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,



		[0x68] = VexEntry(0x68)
					.p66_L0(entry_3(0x68, ops::VPUNPCKHBW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x68, ops::VPUNPCKHBW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x69] = VexEntry(0x69)
					.p66_L0(entry_3(0x69, ops::VPUNPCKHWD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x69, ops::VPUNPCKHWD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x6A] = VexEntry(0x6A)
					.p66_L0(entry_3(0x6A, ops::VPUNPCKHDQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x6A, ops::VPUNPCKHDQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x6B] = VexEntry(0x6B)
					.p66_L0(entry_3(0x6B, ops::VPACKSSDW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x6B, ops::VPACKSSDW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x6C] = VexEntry(0x6C)
					.p66_L0(entry_3(0x6C, ops::VPUNPCKLQDQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x6C, ops::VPUNPCKLQDQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x6D] = VexEntry(0x6D)
					.p66_L0(entry_3(0x6D, ops::VPUNPCKHQDQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x6D, ops::VPUNPCKHQDQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x6E] = VexEntry(0x6E)
					.p66_W0(entry_2(0x6E, ops::VMOVD, OpKind::RegXmm, OpKind::RegMem32))
					.p66_W1(entry_2(0x6E, ops::VMOVQ, OpKind::RegXmm, OpKind::RegMem64))
					,

		[0x6F] = VexEntry(0x6F)
					.p66_L0(entry_2(0x6F, ops::VMOVDQA, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x6F, ops::VMOVDQA, OpKind::RegYmm, OpKind::RegYmmMem256))
					.pF3_L0(entry_2(0x6F, ops::VMOVDQU, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pF3_L1(entry_2(0x6F, ops::VMOVDQU, OpKind::RegYmm, OpKind::RegYmmMem256))
					,



		[0x70] = VexEntry(0x70)
					.p66_L0(entry_3(0x70, ops::VPSHUFD, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_3(0x70, ops::VPSHUFD, OpKind::RegYmm, OpKind::RegYmmMem256, OpKind::Imm8))
					.pF2_L0(entry_3(0x70, ops::VPSHUFLW, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8))
					.pF2_L1(entry_3(0x70, ops::VPSHUFLW, OpKind::RegYmm, OpKind::RegYmmMem256, OpKind::Imm8))
					.pF3_L0(entry_3(0x70, ops::VPSHUFHW, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8))
					.pF3_L1(entry_3(0x70, ops::VPSHUFHW, OpKind::RegYmm, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x71] = VexEntry(0x71, &Vex_Map_1_ModRMExt_71[0]),
		[0x72] = VexEntry(0x72, &Vex_Map_1_ModRMExt_72[0]),
		[0x73] = VexEntry(0x73, &Vex_Map_1_ModRMExt_73[0]),

		[0x74] = VexEntry(0x74)
					.p66_L0(entry_3(0x74, ops::VPCMPEQB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x74, ops::VPCMPEQB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x75] = VexEntry(0x75)
					.p66_L0(entry_3(0x75, ops::VPCMPEQW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x75, ops::VPCMPEQW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x76] = VexEntry(0x76)
					.p66_L0(entry_3(0x76, ops::VPCMPEQD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x76, ops::VPCMPEQD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x77] = VexEntry(0x77)
					.pNN_L0(entry_0(0x77, ops::VZEROUPPER))
					.pNN_L1(entry_0(0x77, ops::VZEROALL))
					.noModRM()
					,



		[0x7C] = VexEntry(0x7C)
					.p66_L0(entry_3(0x7C, ops::VHADDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x7C, ops::VHADDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.pF2_L0(entry_3(0x7C, ops::VHADDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pF2_L1(entry_3(0x7C, ops::VHADDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x7D] = VexEntry(0x7D)
					.p66_L0(entry_3(0x7D, ops::VHSUBPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x7D, ops::VHSUBPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.pF2_L0(entry_3(0x7D, ops::VHSUBPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pF2_L1(entry_3(0x7D, ops::VHSUBPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x7E] = VexEntry(0x7E)
					.p66_W0(entry_2(0x7E, ops::VMOVD, OpKind::RegMem32, OpKind::RegXmm))
					.p66_W1(entry_2(0x7E, ops::VMOVQ, OpKind::RegMem64, OpKind::RegXmm))
					,

		[0x7F] = VexEntry(0x7F)
					.p66_L0(entry_2(0x7F, ops::VMOVDQA, OpKind::RegXmmMem128, OpKind::RegXmm))
					.p66_L1(entry_2(0x7F, ops::VMOVDQA, OpKind::RegYmmMem256, OpKind::RegYmm))
					.pF3_L0(entry_2(0x7F, ops::VMOVDQU, OpKind::RegXmmMem128, OpKind::RegXmm))
					.pF3_L1(entry_2(0x7F, ops::VMOVDQU, OpKind::RegYmmMem256, OpKind::RegYmm))
					,


		[0xAE] = VexEntry(0xAE, &Vex_Map_1_ModRMExt_AE[0]),

		[0xC2] = VexEntry(0xC2)
					.pNN_L0(entry_4(0xC2, ops::VCMPPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.pNN_L1(entry_4(0xC2, ops::VCMPPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					.p66_L0(entry_4(0xC2, ops::VCMPPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_4(0xC2, ops::VCMPPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					.pF2(entry_4(0xC2, ops::VCMPPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem64, OpKind::Imm8))
					.pF3(entry_4(0xC2, ops::VCMPSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32, OpKind::Imm8))
					,

		[0xC4] = VexEntry(0xC4)
					.p66_W0_L0(entry_4(0xC4, ops::VPINSRW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::Reg32Mem16, OpKind::Imm8))
					,

		[0xC5] = VexEntry(0xC5)
					.p66_W0_L0(entry_3(0xC5, ops::VPEXTRW, OpKind::Reg32, OpKind::RegXmm_Rm, OpKind::Imm8))
					,

		[0xC6] = VexEntry(0xC6)
					.pNN_L0(entry_4(0xC6, ops::VSHUFPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.pNN_L1(entry_4(0xC6, ops::VSHUFPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					.p66_L0(entry_4(0xC6, ops::VSHUFPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_4(0xC6, ops::VSHUFPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,



		[0xD0] = VexEntry(0xD0)
					.p66_L0(entry_3(0xD0, ops::VADDSUBPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xD0, ops::VADDSUBPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.pF2_L0(entry_3(0xD0, ops::VADDSUBPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.pF2_L1(entry_3(0xD0, ops::VADDSUBPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xD1] = VexEntry(0xD1)
					.p66_L0(entry_3(0xD1, ops::VPSRLW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xD1, ops::VPSRLW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128))
					,

		[0xD2] = VexEntry(0xD2)
					.p66_L0(entry_3(0xD2, ops::VPSRLD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xD2, ops::VPSRLD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128))
					,

		[0xD3] = VexEntry(0xD3)
					.p66_L0(entry_3(0xD3, ops::VPSRLQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xD3, ops::VPSRLQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128))
					,

		[0xD4] = VexEntry(0xD4)
					.p66_L0(entry_3(0xD4, ops::VPADDQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xD4, ops::VPADDQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xD5] = VexEntry(0xD5)
					.p66_L0(entry_3(0xD5, ops::VPMULLW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xD5, ops::VPMULLW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xD6] = VexEntry(0xD6)
					.p66_L0(entry_2(0xD6, ops::VMOVQ, OpKind::RegXmmMem64, OpKind::RegXmm))
					,

		[0xD7] = VexEntry(0xD7)
					.p66_L0(entry_2(0xD7, ops::VPMOVMSKB, OpKind::Reg32, OpKind::RegXmm_Rm))
					.p66_L1(entry_2(0xD7, ops::VPMOVMSKB, OpKind::Reg32, OpKind::RegYmm_Rm))
					,



		[0xD8] = VexEntry(0xD8)
					.p66_L0(entry_3(0xD8, ops::VPSUBUSB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xD8, ops::VPSUBUSB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xD9] = VexEntry(0xD9)
					.p66_L0(entry_3(0xD9, ops::VPSUBUSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xD9, ops::VPSUBUSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xDA] = VexEntry(0xDA)
					.p66_L0(entry_3(0xDA, ops::VPMINUB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xDA, ops::VPMINUB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xDB] = VexEntry(0xDB)
					.p66_L0(entry_3(0xDB, ops::VPAND, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xDB, ops::VPAND, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xDC] = VexEntry(0xDC)
					.p66_L0(entry_3(0xDC, ops::VPADDUSB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xDC, ops::VPADDUSB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xDD] = VexEntry(0xDD)
					.p66_L0(entry_3(0xDD, ops::VPADDUSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xDD, ops::VPADDUSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xDE] = VexEntry(0xDE)
					.p66_L0(entry_3(0xDE, ops::VPMAXUB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xDE, ops::VPMAXUB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xDF] = VexEntry(0xDF)
					.p66_L0(entry_3(0xDF, ops::VPANDN, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xDF, ops::VPANDN, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,



		[0xE0] = VexEntry(0xE0)
					.p66_L0(entry_3(0xE0, ops::VPAVGB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xE0, ops::VPAVGB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xE1] = VexEntry(0xE1)
					.p66_L0(entry_3(0xE1, ops::VPSRAW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xE1, ops::VPSRAW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128))
					,

		[0xE2] = VexEntry(0xE2)
					.p66_L0(entry_3(0xE2, ops::VPSRAD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xE2, ops::VPSRAD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128))
					,

		[0xE3] = VexEntry(0xE3)
					.p66_L0(entry_3(0xE3, ops::VPAVGW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xE3, ops::VPAVGW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xE4] = VexEntry(0xE4)
					.p66_L0(entry_3(0xE4, ops::VPMULHUW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xE4, ops::VPMULHUW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xE5] = VexEntry(0xE5)
					.p66_L0(entry_3(0xE5, ops::VPMULHW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xE5, ops::VPMULHW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xE6] = VexEntry(0xE6)
					.p66_L0(entry_2(0xE6, ops::VCVTTPD2DQ, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0xE6, ops::VCVTTPD2DQ, OpKind::RegYmm, OpKind::RegYmmMem256))
					.pF2_L0(entry_2(0xE6, ops::VCVTDQ2PD, OpKind::RegXmm, OpKind::RegXmmMem128))
					.pF2_L1(entry_2(0xE6, ops::VCVTDQ2PD, OpKind::RegXmm, OpKind::RegYmmMem256))
					.pF3_L0(entry_2(0xE6, ops::VCVTDQ2PD, OpKind::RegXmm, OpKind::RegXmmMem64))
					.pF3_L1(entry_2(0xE6, ops::VCVTDQ2PD, OpKind::RegYmm, OpKind::RegXmmMem128))
					,

		[0xE7] = VexEntry(0xE7)
					.p66_L0(entry_2(0xE7, ops::VPMULHW, OpKind::Mem128, OpKind::RegXmm))
					.p66_L1(entry_2(0xE7, ops::VPMULHW, OpKind::Mem256, OpKind::RegYmm))
					,



		[0xE8] = VexEntry(0xE8)
					.p66_L0(entry_3(0xE8, ops::VPSUBSB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xE8, ops::VPSUBSB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xE9] = VexEntry(0xE9)
					.p66_L0(entry_3(0xE9, ops::VPSUBSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xE9, ops::VPSUBSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xEA] = VexEntry(0xEA)
					.p66_L0(entry_3(0xEA, ops::VPMINSB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xEA, ops::VPMINSB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xEB] = VexEntry(0xEB)
					.p66_L0(entry_3(0xEB, ops::VPOR, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xEB, ops::VPOR, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xEC] = VexEntry(0xEC)
					.p66_L0(entry_3(0xEC, ops::VPADDSB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xEC, ops::VPADDSB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xED] = VexEntry(0xED)
					.p66_L0(entry_3(0xED, ops::VPADDSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xED, ops::VPADDSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xEE] = VexEntry(0xEE)
					.p66_L0(entry_3(0xEE, ops::VPMAXSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xEE, ops::VPMAXSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xEF] = VexEntry(0xEF)
					.p66_L0(entry_3(0xEF, ops::VPXOR, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xEF, ops::VPXOR, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,



		[0xF0] = VexEntry(0xF0)
					.pF2_L0(entry_2(0xF0, ops::VLDDQU, OpKind::RegXmm, OpKind::Mem128))
					.pF2_L1(entry_2(0xF0, ops::VLDDQU, OpKind::RegYmm, OpKind::Mem256))
					,

		[0xF1] = VexEntry(0xF1)
					.p66_L0(entry_3(0xF1, ops::VPSLLW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xF1, ops::VPSLLW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128))
					,

		[0xF2] = VexEntry(0xF2)
					.p66_L0(entry_3(0xF2, ops::VPSLLD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xF2, ops::VPSLLD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128))
					,

		[0xF3] = VexEntry(0xF3)
					.p66_L0(entry_3(0xF3, ops::VPSLLQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xF3, ops::VPSLLQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128))
					,

		[0xF4] = VexEntry(0xF4)
					.p66_L0(entry_3(0xF4, ops::VPMULUDQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xF4, ops::VPMULUDQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xF5] = VexEntry(0xF5)
					.p66_L0(entry_3(0xF5, ops::VPMADDWD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xF5, ops::VPMADDWD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xF6] = VexEntry(0xF6)
					.p66_L0(entry_3(0xF6, ops::VPSADBW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xF6, ops::VPSADBW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xF7] = VexEntry(0xF7)
					.p66_L0(entry_2(0xF7, ops::VMASKMOVDQU, OpKind::RegXmm, OpKind::RegXmm_Rm))
					,



		[0xF8] = VexEntry(0xF8)
					.p66_L0(entry_3(0xF8, ops::VPSUBB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xF8, ops::VPSUBB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xF9] = VexEntry(0xF9)
					.p66_L0(entry_3(0xF9, ops::VPSUBW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xF9, ops::VPSUBW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xFA] = VexEntry(0xFA)
					.p66_L0(entry_3(0xFA, ops::VPSUBD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xFA, ops::VPSUBD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xFB] = VexEntry(0xFB)
					.p66_L0(entry_3(0xFB, ops::VPSUBQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xFB, ops::VPSUBQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xFC] = VexEntry(0xFC)
					.p66_L0(entry_3(0xFC, ops::VPADDB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xFC, ops::VPADDB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xFD] = VexEntry(0xFD)
					.p66_L0(entry_3(0xFD, ops::VPADDW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xFD, ops::VPADDW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xFE] = VexEntry(0xFE)
					.p66_L0(entry_3(0xFE, ops::VPADDD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0xFE, ops::VPADDD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,


		[0xFF] = VexEntry(0)
	};



	constexpr VexEntry Vex_Map_2_ModRMExt_F3[] = {
		[1] = VexEntry(0xF3)
				.pNN_W0_L0(entry_2(0xF3, ops::BLSR, OpKind::Reg32, OpKind::RegMem32))
				.pNN_W1_L0(entry_2(0xF3, ops::BLSR, OpKind::Reg64, OpKind::RegMem64))
				,

		[2] = VexEntry(0xF3)
				.pNN_W0_L0(entry_2(0xF3, ops::BLSMSK, OpKind::Reg32, OpKind::RegMem32))
				.pNN_W1_L0(entry_2(0xF3, ops::BLSMSK, OpKind::Reg64, OpKind::RegMem64))
				,

		[3] = VexEntry(0xF3)
				.pNN_W0_L0(entry_2(0xF3, ops::BLSI, OpKind::Reg32, OpKind::RegMem32))
				.pNN_W1_L0(entry_2(0xF3, ops::BLSI, OpKind::Reg64, OpKind::RegMem64))
				,


		[7] = VexEntry(0)
	};


	constexpr VexEntry VEX_Map_2[] = {
		[0x00] = VexEntry(0x00)
					.p66_L0(entry_3(0x00, ops::VPSHUFB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x00, ops::VPSHUFB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x01] = VexEntry(0x01)
					.p66_L0(entry_3(0x01, ops::VPHADDW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x01, ops::VPHADDW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x02] = VexEntry(0x02)
					.p66_L0(entry_3(0x02, ops::VPHADDD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x02, ops::VPHADDD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x03] = VexEntry(0x03)
					.p66_L0(entry_3(0x03, ops::VPHADDSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x03, ops::VPHADDSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x04] = VexEntry(0x04)
					.p66_L0(entry_3(0x04, ops::VPMADDUBSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x04, ops::VPMADDUBSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x05] = VexEntry(0x05)
					.p66_L0(entry_3(0x05, ops::VPHSUBW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x05, ops::VPHSUBW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x06] = VexEntry(0x06)
					.p66_L0(entry_3(0x06, ops::VPHSUBD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x06, ops::VPHSUBD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x07] = VexEntry(0x07)
					.p66_L0(entry_3(0x07, ops::VPHSUBSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x07, ops::VPHSUBSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,



		[0x08] = VexEntry(0x08)
					.p66_L0(entry_3(0x08, ops::VPSIGNB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x08, ops::VPSIGNB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x09] = VexEntry(0x09)
					.p66_L0(entry_3(0x09, ops::VPSIGNW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x09, ops::VPSIGNW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x0A] = VexEntry(0x0A)
					.p66_L0(entry_3(0x0A, ops::VPSIGND, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x0A, ops::VPSIGND, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x0B] = VexEntry(0x0B)
					.p66_L0(entry_3(0x0B, ops::VPMULHRSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x0B, ops::VPMULHRSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x0C] = VexEntry(0x0C)
					.p66_W0_L0(entry_3(0x0C, ops::VPERMILPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x0C, ops::VPERMILPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x0D] = VexEntry(0x0D)
					.p66_W0_L0(entry_3(0x0D, ops::VPERMILPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x0D, ops::VPERMILPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x0E] = VexEntry(0x0E)
					.p66_W0_L0(entry_2(0x0E, ops::VTESTPS, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_2(0x0E, ops::VTESTPS, OpKind::RegYmm, OpKind::RegYmmMem256))
					,

		[0x0F] = VexEntry(0x0F)
					.p66_W0_L0(entry_2(0x0F, ops::VTESTPD, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_2(0x0F, ops::VTESTPD, OpKind::RegYmm, OpKind::RegYmmMem256))
					,



		[0x13] = VexEntry(0x13)
					.p66_W0_L0(entry_2(0x13, ops::VCVTPH2PS, OpKind::RegXmm, OpKind::RegXmmMem64))
					.p66_W0_L1(entry_2(0x13, ops::VCVTPH2PS, OpKind::RegYmm, OpKind::RegXmmMem128))
					,

		[0x16] = VexEntry(0x16)
					.p66_W0_L1(entry_3(0x16, ops::VPERMPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x17] = VexEntry(0x17)
					.p66_L0(entry_2(0x17, ops::VPERMPS, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x17, ops::VPERMPS, OpKind::RegYmm, OpKind::RegYmmMem256))
					,



		[0x18] = VexEntry(0x18)
					.p66_W0_L0_mod3(entry_2(0x18, ops::VBROADCASTSS, OpKind::RegXmm, OpKind::RegXmm_Rm))
					.p66_W0_L0_mod0(entry_2(0x18, ops::VBROADCASTSS, OpKind::RegXmm, OpKind::Mem32))
					.p66_W0_L1_mod3(entry_2(0x18, ops::VBROADCASTSS, OpKind::RegYmm, OpKind::RegXmm_Rm))
					.p66_W0_L1_mod0(entry_2(0x18, ops::VBROADCASTSS, OpKind::RegYmm, OpKind::Mem32))
					,

		[0x19] = VexEntry(0x19)
					.p66_W0_L1_mod3(entry_2(0x19, ops::VBROADCASTSD, OpKind::RegYmm, OpKind::RegXmm_Rm))
					.p66_W0_L1_mod0(entry_2(0x19, ops::VBROADCASTSD, OpKind::RegYmm, OpKind::Mem64))
					,

		[0x1A] = VexEntry(0x1A)
					.p66_W0_L1(entry_2(0x1A, ops::VBROADCASTF128, OpKind::RegYmm, OpKind::Mem128))
					,

		[0x1C] = VexEntry(0x1C)
					.p66_L0(entry_2(0x1C, ops::VPABSB, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x1C, ops::VPABSB, OpKind::RegYmm, OpKind::RegYmmMem256))
					,

		[0x1D] = VexEntry(0x1D)
					.p66_L0(entry_2(0x1D, ops::VPABSW, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x1D, ops::VPABSW, OpKind::RegYmm, OpKind::RegYmmMem256))
					,

		[0x1E] = VexEntry(0x1E)
					.p66_L0(entry_2(0x1E, ops::VPABSD, OpKind::RegXmm, OpKind::RegXmmMem128))
					.p66_L1(entry_2(0x1E, ops::VPABSD, OpKind::RegYmm, OpKind::RegYmmMem256))
					,



		[0x20] = VexEntry(0x20)
					.p66_L0(entry_2(0x20, ops::VPMOVSXBW, OpKind::RegXmm, OpKind::RegXmmMem64))
					.p66_L1(entry_2(0x20, ops::VPMOVSXBW, OpKind::RegYmm, OpKind::RegXmmMem128))
					,

		[0x21] = VexEntry(0x21)
					.p66_L0(entry_2(0x21, ops::VPMOVSXBD, OpKind::RegXmm, OpKind::RegXmmMem32))
					.p66_L1(entry_2(0x21, ops::VPMOVSXBD, OpKind::RegYmm, OpKind::RegXmmMem64))
					,

		[0x22] = VexEntry(0x22)
					.p66_L0(entry_2(0x22, ops::VPMOVSXBQ, OpKind::RegXmm, OpKind::RegXmmMem16))
					.p66_L1(entry_2(0x22, ops::VPMOVSXBQ, OpKind::RegYmm, OpKind::RegXmmMem32))
					,

		[0x23] = VexEntry(0x23)
					.p66_L0(entry_2(0x23, ops::VPMOVSXWD, OpKind::RegXmm, OpKind::RegXmmMem64))
					.p66_L1(entry_2(0x23, ops::VPMOVSXWD, OpKind::RegYmm, OpKind::RegXmmMem128))
					,

		[0x24] = VexEntry(0x24)
					.p66_L0(entry_2(0x24, ops::VPMOVSXWQ, OpKind::RegXmm, OpKind::RegXmmMem32))
					.p66_L1(entry_2(0x24, ops::VPMOVSXWQ, OpKind::RegYmm, OpKind::RegXmmMem64))
					,

		[0x25] = VexEntry(0x25)
					.p66_L0(entry_2(0x25, ops::VPMOVSXDQ, OpKind::RegXmm, OpKind::RegXmmMem64))
					.p66_L1(entry_2(0x25, ops::VPMOVSXDQ, OpKind::RegYmm, OpKind::RegXmmMem128))
					,



		[0x28] = VexEntry(0x28)
					.p66_L0(entry_3(0x28, ops::VPMULDQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x28, ops::VPMULDQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x29] = VexEntry(0x29)
					.p66_L0(entry_3(0x29, ops::VPCMPEQQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x29, ops::VPCMPEQQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x2A] = VexEntry(0x2A)
					.p66_L0(entry_2(0x2A, ops::VMOVNTDQA, OpKind::RegXmm, OpKind::Mem128))
					.p66_L1(entry_2(0x2A, ops::VMOVNTDQA, OpKind::RegYmm, OpKind::Mem256))
					,

		[0x2B] = VexEntry(0x2B)
					.p66_L0(entry_3(0x2B, ops::VPACKUSDW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x2B, ops::VPACKUSDW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x2C] = VexEntry(0x2C)
					.p66_W0_L0(entry_3(0x2C, ops::VMASKMOVPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::Mem128))
					.p66_W0_L1(entry_3(0x2C, ops::VMASKMOVPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::Mem256))
					,

		[0x2D] = VexEntry(0x2D)
					.p66_W0_L0(entry_3(0x2D, ops::VMASKMOVPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::Mem128))
					.p66_W0_L1(entry_3(0x2D, ops::VMASKMOVPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::Mem256))
					,

		[0x2E] = VexEntry(0x2E)
					.p66_W0_L0(entry_3(0x2E, ops::VMASKMOVPS, OpKind::Mem128, OpKind::RegXmm_vvvv, OpKind::RegXmm))
					.p66_W0_L1(entry_3(0x2E, ops::VMASKMOVPS, OpKind::Mem256, OpKind::RegYmm_vvvv, OpKind::RegYmm))
					,

		[0x2F] = VexEntry(0x2F)
					.p66_W0_L0(entry_3(0x2F, ops::VMASKMOVPD, OpKind::Mem128, OpKind::RegXmm_vvvv, OpKind::RegXmm))
					.p66_W0_L1(entry_3(0x2F, ops::VMASKMOVPD, OpKind::Mem256, OpKind::RegYmm_vvvv, OpKind::RegYmm))
					,




		[0x30] = VexEntry(0x30)
					.p66_L0(entry_2(0x30, ops::VPMOVZXBW, OpKind::RegXmm, OpKind::RegXmmMem64))
					.p66_L1(entry_2(0x30, ops::VPMOVZXBW, OpKind::RegYmm, OpKind::RegXmmMem128))
					,

		[0x31] = VexEntry(0x31)
					.p66_L0(entry_2(0x31, ops::VPMOVZXBD, OpKind::RegXmm, OpKind::RegXmmMem32))
					.p66_L1(entry_2(0x31, ops::VPMOVZXBD, OpKind::RegYmm, OpKind::RegXmmMem64))
					,

		[0x32] = VexEntry(0x32)
					.p66_L0(entry_2(0x32, ops::VPMOVZXBQ, OpKind::RegXmm, OpKind::RegXmmMem16))
					.p66_L1(entry_2(0x32, ops::VPMOVZXBQ, OpKind::RegYmm, OpKind::RegXmmMem32))
					,

		[0x33] = VexEntry(0x33)
					.p66_L0(entry_2(0x33, ops::VPMOVZXWD, OpKind::RegXmm, OpKind::RegXmmMem64))
					.p66_L1(entry_2(0x33, ops::VPMOVZXWD, OpKind::RegYmm, OpKind::RegXmmMem128))
					,

		[0x34] = VexEntry(0x34)
					.p66_L0(entry_2(0x34, ops::VPMOVZXWQ, OpKind::RegXmm, OpKind::RegXmmMem32))
					.p66_L1(entry_2(0x34, ops::VPMOVZXWQ, OpKind::RegYmm, OpKind::RegXmmMem64))
					,

		[0x35] = VexEntry(0x35)
					.p66_L0(entry_2(0x35, ops::VPMOVZXDQ, OpKind::RegXmm, OpKind::RegXmmMem64))
					.p66_L1(entry_2(0x35, ops::VPMOVZXDQ, OpKind::RegYmm, OpKind::RegXmmMem128))
					,

		[0x36] = VexEntry(0x36)
					.p66_W0_L1(entry_3(0x36, ops::VPERMD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x37] = VexEntry(0x37)
					.p66_L0(entry_3(0x37, ops::VPCMPGTQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x37, ops::VPCMPGTQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,



		[0x38] = VexEntry(0x38)
					.p66_L0(entry_3(0x38, ops::VPMINSB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x38, ops::VPMINSB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x39] = VexEntry(0x39)
					.p66_L0(entry_3(0x39, ops::VPMINSW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x39, ops::VPMINSW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x3A] = VexEntry(0x3A)
					.p66_L0(entry_3(0x3A, ops::VPMINUW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x3A, ops::VPMINUW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x3B] = VexEntry(0x3B)
					.p66_L0(entry_3(0x3B, ops::VPMINUD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x3B, ops::VPMINUD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x3C] = VexEntry(0x3C)
					.p66_L0(entry_3(0x3C, ops::VPMAXSB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x3C, ops::VPMAXSB, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x3D] = VexEntry(0x3D)
					.p66_L0(entry_3(0x3D, ops::VPMAXSD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x3D, ops::VPMAXSD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x3E] = VexEntry(0x3E)
					.p66_L0(entry_3(0x3E, ops::VPMAXUW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x3E, ops::VPMAXUW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x3F] = VexEntry(0x3F)
					.p66_L0(entry_3(0x3F, ops::VPMAXUD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x3F, ops::VPMAXUD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,



		[0x40] = VexEntry(0x40)
					.p66_L0(entry_3(0x40, ops::VPMULLD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_L1(entry_3(0x40, ops::VPMULLD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x41] = VexEntry(0x41)
					.p66_L0(entry_2(0x41, ops::VPHMINPOSUW, OpKind::RegXmm, OpKind::RegXmmMem128))
					,

		[0x45] = VexEntry(0x45)
					.p66_W0_L0(entry_3(0x45, ops::VPSRLVD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x45, ops::VPSRLVD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0x45, ops::VPSRLVQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x45, ops::VPSRLVQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x46] = VexEntry(0x46)
					.p66_W0_L0(entry_3(0x46, ops::VPRAVD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x46, ops::VPRAVD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x47] = VexEntry(0x47)
					.p66_W0_L0(entry_3(0x47, ops::VPSLLVD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x47, ops::VPSLLVD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0x47, ops::VPSLLVQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x47, ops::VPSLLVQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,



		[0x58] = VexEntry(0x58)
					.p66_W0_L0(entry_2(0x58, ops::VPBROADCASTD, OpKind::RegXmm, OpKind::RegXmmMem32))
					.p66_W0_L1(entry_2(0x58, ops::VPBROADCASTD, OpKind::RegYmm, OpKind::RegXmmMem32))
					,

		[0x59] = VexEntry(0x59)
					.p66_W0_L0(entry_2(0x59, ops::VPBROADCASTD, OpKind::RegXmm, OpKind::RegXmmMem64))
					.p66_W0_L1(entry_2(0x59, ops::VPBROADCASTD, OpKind::RegYmm, OpKind::RegXmmMem64))
					,

		[0x5A] = VexEntry(0x5A)
					.p66_W0_L1(entry_2(0x5A, ops::VPBROADCASTI128, OpKind::RegYmm, OpKind::Mem128))
					,



		[0x78] = VexEntry(0x78)
					.p66_W0_L0(entry_2(0x78, ops::VPBROADCASTB, OpKind::RegXmm, OpKind::RegXmmMem8))
					.p66_W0_L1(entry_2(0x78, ops::VPBROADCASTB, OpKind::RegYmm, OpKind::RegXmmMem8))
					,

		[0x79] = VexEntry(0x79)
					.p66_W0_L0(entry_2(0x79, ops::VPBROADCASTW, OpKind::RegXmm, OpKind::RegXmmMem16))
					.p66_W0_L1(entry_2(0x79, ops::VPBROADCASTW, OpKind::RegYmm, OpKind::RegXmmMem16))
					,



		[0x8C] = VexEntry(0x8C)
					.p66_W0_L0(entry_3(0x8C, ops::VPMASKMOVD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::Mem128))
					.p66_W0_L1(entry_3(0x8C, ops::VPMASKMOVD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::Mem256))
					.p66_W1_L0(entry_3(0x8C, ops::VPMASKMOVQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x8C, ops::VPMASKMOVQ, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x8E] = VexEntry(0x8E)
					.p66_W0_L0(entry_3(0x8E, ops::VPMASKMOVD, OpKind::Mem128, OpKind::RegXmm_vvvv, OpKind::RegXmm))
					.p66_W0_L1(entry_3(0x8E, ops::VPMASKMOVD, OpKind::Mem256, OpKind::RegYmm_vvvv, OpKind::RegYmm))
					.p66_W1_L0(entry_3(0x8E, ops::VPMASKMOVQ, OpKind::RegXmmMem128, OpKind::RegXmm_vvvv, OpKind::RegXmm))
					.p66_W1_L1(entry_3(0x8E, ops::VPMASKMOVQ, OpKind::RegYmmMem256, OpKind::RegYmm_vvvv, OpKind::RegYmm))
					,



		[0x90] = VexEntry(0x90)
					.p66_W0_L0(entry_3(0x90, ops::VPGATHERDD, OpKind::RegXmm, OpKind::VSIB_Xmm32, OpKind::RegXmm_vvvv))
					.p66_W0_L1(entry_3(0x90, ops::VPGATHERDD, OpKind::RegYmm, OpKind::VSIB_Ymm32, OpKind::RegYmm_vvvv))
					,

		[0x91] = VexEntry(0x91)
					.p66_W0_L0(entry_3(0x91, ops::VPGATHERQD, OpKind::RegXmm, OpKind::VSIB_Xmm64, OpKind::RegXmm_vvvv))
					.p66_W0_L1(entry_3(0x91, ops::VPGATHERQD, OpKind::RegYmm, OpKind::VSIB_Ymm64, OpKind::RegYmm_vvvv))
					,

		[0x92] = VexEntry(0x92)
					.p66_W0_L0(entry_3(0x92, ops::VGATHERDPS, OpKind::RegXmm, OpKind::VSIB_Xmm32, OpKind::RegXmm_vvvv))
					.p66_W0_L1(entry_3(0x92, ops::VGATHERDPS, OpKind::RegYmm, OpKind::VSIB_Ymm32, OpKind::RegYmm_vvvv))
					,

		[0x93] = VexEntry(0x93)
					.p66_W0_L0(entry_3(0x93, ops::VGATHERQPS, OpKind::RegXmm, OpKind::VSIB_Xmm64, OpKind::RegXmm_vvvv))
					.p66_W0_L1(entry_3(0x93, ops::VGATHERQPS, OpKind::RegYmm, OpKind::VSIB_Ymm64, OpKind::RegYmm_vvvv))
					,

		[0x96] = VexEntry(0x96)
					.p66_W0_L0(entry_3(0x96, ops::VFMADDSUB132PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x96, ops::VFMADDSUB132PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0x96, ops::VFMADDSUB132PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x96, ops::VFMADDSUB132PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x97] = VexEntry(0x97)
					.p66_W0_L0(entry_3(0x97, ops::VFMSUBADD132PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x97, ops::VFMSUBADD132PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0x97, ops::VFMSUBADD132PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x97, ops::VFMSUBADD132PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,



		[0x98] = VexEntry(0x98)
					.p66_W0_L0(entry_3(0x98, ops::VFMADD132PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x98, ops::VFMADD132PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0x98, ops::VFMADD132PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x98, ops::VFMADD132PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x99] = VexEntry(0x99)
					.p66_W0(entry_3(0x99, ops::VFMADD132SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0x99, ops::VFMADD132SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x9A] = VexEntry(0x9A)
					.p66_W0_L0(entry_3(0x9A, ops::VFMSUB132PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x9A, ops::VFMSUB132PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0x9A, ops::VFMSUB132PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x9A, ops::VFMSUB132PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x9B] = VexEntry(0x9B)
					.p66_W0(entry_3(0x9B, ops::VFMSUB132SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0x9B, ops::VFMSUB132SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x9C] = VexEntry(0x9C)
					.p66_W0_L0(entry_3(0x9C, ops::VFNMADD132PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x9C, ops::VFNMADD132PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0x9C, ops::VFNMADD132PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x9C, ops::VFNMADD132PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x9D] = VexEntry(0x9D)
					.p66_W0(entry_3(0x9D, ops::VFNMADD132SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0x9D, ops::VFNMADD132SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0x9E] = VexEntry(0x9E)
					.p66_W0_L0(entry_3(0x9E, ops::VFNMSUB132PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x9E, ops::VFNMSUB132PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0x9E, ops::VFNMSUB132PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x9E, ops::VFNMSUB132PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0x9F] = VexEntry(0x9F)
					.p66_W0(entry_3(0x9F, ops::VFNMSUB132SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0x9F, ops::VFNMSUB132SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,




		[0xA6] = VexEntry(0xA6)
					.p66_W0_L0(entry_3(0xA6, ops::VFMADDSUB213PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xA6, ops::VFMADDSUB213PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xA6, ops::VFMADDSUB213PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xA6, ops::VFMADDSUB213PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xA7] = VexEntry(0xA7)
					.p66_W0_L0(entry_3(0xA7, ops::VFMSUBADD213PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xA7, ops::VFMSUBADD213PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xA7, ops::VFMSUBADD213PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xA7, ops::VFMSUBADD213PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xA8] = VexEntry(0xA8)
					.p66_W0_L0(entry_3(0xA8, ops::VFMADD213PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xA8, ops::VFMADD213PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xA8, ops::VFMADD213PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xA8, ops::VFMADD213PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xA9] = VexEntry(0xA9)
					.p66_W0(entry_3(0xA9, ops::VFMADD213SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0xA9, ops::VFMADD213SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0xAA] = VexEntry(0xAA)
					.p66_W0_L0(entry_3(0xAA, ops::VFMSUB213PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xAA, ops::VFMSUB213PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xAA, ops::VFMSUB213PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xAA, ops::VFMSUB213PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xAB] = VexEntry(0xAB)
					.p66_W0(entry_3(0xAB, ops::VFMSUB213SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0xAB, ops::VFMSUB213SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0xAC] = VexEntry(0xAC)
					.p66_W0_L0(entry_3(0xAC, ops::VFNMADD213PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xAC, ops::VFNMADD213PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xAC, ops::VFNMADD213PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xAC, ops::VFNMADD213PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xAD] = VexEntry(0xAD)
					.p66_W0(entry_3(0xAD, ops::VFNMADD213SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0xAD, ops::VFNMADD213SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0xAE] = VexEntry(0xAE)
					.p66_W0_L0(entry_3(0xAE, ops::VFNMSUB213PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xAE, ops::VFNMSUB213PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xAE, ops::VFNMSUB213PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xAE, ops::VFNMSUB213PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xAF] = VexEntry(0xAF)
					.p66_W0(entry_3(0xAF, ops::VFNMSUB213SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0xAF, ops::VFNMSUB213SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,




		[0xB6] = VexEntry(0xB6)
					.p66_W0_L0(entry_3(0xB6, ops::VFMADDSUB231PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0x96, ops::VFMADDSUB231PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xB6, ops::VFMADDSUB231PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0x96, ops::VFMADDSUB231PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xB7] = VexEntry(0xB7)
					.p66_W0_L0(entry_3(0xB7, ops::VFMSUBADD231PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xB7, ops::VFMSUBADD231PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xB7, ops::VFMSUBADD231PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xB7, ops::VFMSUBADD231PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xB8] = VexEntry(0xB8)
					.p66_W0_L0(entry_3(0xB8, ops::VFMADD231PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xB8, ops::VFMADD231PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xB8, ops::VFMADD231PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xB8, ops::VFMADD231PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xB9] = VexEntry(0xB9)
					.p66_W0(entry_3(0xB9, ops::VFMADD231SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0xB9, ops::VFMADD231SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0xBA] = VexEntry(0xBA)
					.p66_W0_L0(entry_3(0xBA, ops::VFMSUB231PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xBA, ops::VFMSUB231PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xBA, ops::VFMSUB231PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xBA, ops::VFMSUB231PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xBB] = VexEntry(0xBB)
					.p66_W0(entry_3(0xBB, ops::VFMSUB231SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0xBB, ops::VFMSUB231SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0xBC] = VexEntry(0xBC)
					.p66_W0_L0(entry_3(0xBC, ops::VFNMADD231PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xBC, ops::VFNMADD231PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xBC, ops::VFNMADD231PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xBC, ops::VFNMADD231PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xBD] = VexEntry(0xBD)
					.p66_W0(entry_3(0xBD, ops::VFNMADD231SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0xBD, ops::VFNMADD231SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,

		[0xBE] = VexEntry(0xBE)
					.p66_W0_L0(entry_3(0xBE, ops::VFNMSUB231PS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W0_L1(entry_3(0xBE, ops::VFNMSUB231PS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					.p66_W1_L0(entry_3(0xBE, ops::VFNMSUB231PD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128))
					.p66_W1_L1(entry_3(0xBE, ops::VFNMSUB231PD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256))
					,

		[0xBF] = VexEntry(0xBF)
					.p66_W0(entry_3(0xBF, ops::VFNMSUB231SS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					.p66_W1(entry_3(0xBF, ops::VFNMSUB231SD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32))
					,



		[0xDB] = VexEntry(0xDB).p66_L0(entry_2(0xDB, ops::VAESIMC, OpKind::RegXmm, OpKind::RegXmmMem128)),
		[0xDC] = VexEntry(0xDC).p66_L0(entry_3(0xDC, ops::VAESIMC, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128)),
		[0xDD] = VexEntry(0xDD).p66_L0(entry_3(0xDD, ops::VAESENCLAST, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128)),
		[0xDE] = VexEntry(0xDE).p66_L0(entry_3(0xDE, ops::VAESDEC, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128)),
		[0xDF] = VexEntry(0xDF).p66_L0(entry_3(0xDF, ops::VAESDECLAST, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128)),

		[0xF2] = VexEntry(0xF2)
					.pNN_W0_L0(entry_3(0xF2, ops::ANDN, OpKind::Reg32, OpKind::Reg32_vvvv, OpKind::RegMem32))
					.pNN_W1_L0(entry_3(0xF2, ops::ANDN, OpKind::Reg64, OpKind::Reg64_vvvv, OpKind::RegMem64))
					,

		[0xF3] = VexEntry(0xF3, &Vex_Map_2_ModRMExt_F3[0]),

		[0xF5] = VexEntry(0xF5)
					.pNN_W0_L0(entry_3(0xF5, ops::BZHI, OpKind::Reg32, OpKind::RegMem32, OpKind::Reg32_vvvv))
					.pNN_W1_L0(entry_3(0xF5, ops::BZHI, OpKind::Reg64, OpKind::RegMem64, OpKind::Reg64_vvvv))
					.pF2_W0_L0(entry_3(0xF5, ops::PDEP, OpKind::Reg32, OpKind::Reg32_vvvv, OpKind::RegMem32))
					.pF2_W1_L0(entry_3(0xF5, ops::PDEP, OpKind::Reg64, OpKind::Reg64_vvvv, OpKind::RegMem64))
					.pF3_W0_L0(entry_3(0xF5, ops::PEXT, OpKind::Reg32, OpKind::Reg32_vvvv, OpKind::RegMem32))
					.pF3_W1_L0(entry_3(0xF5, ops::PEXT, OpKind::Reg64, OpKind::Reg64_vvvv, OpKind::RegMem64))
					,

		[0xF6] = VexEntry(0xF6)
					.pF2_W0_L0(entry_3(0xF6, ops::PDEP, OpKind::Reg32, OpKind::Reg32_vvvv, OpKind::RegMem32))
					.pF2_W1_L0(entry_3(0xF6, ops::PDEP, OpKind::Reg64, OpKind::Reg64_vvvv, OpKind::RegMem64))
					,


		[0xF7] = VexEntry(0xF7)
					.pNN_W0_L0(entry_3(0xF7, ops::BEXTR, OpKind::Reg32, OpKind::RegMem32, OpKind::Reg32_vvvv))
					.pNN_W1_L0(entry_3(0xF7, ops::BEXTR, OpKind::Reg64, OpKind::RegMem64, OpKind::Reg64_vvvv))
					.p66_W0_L0(entry_3(0xF7, ops::SHLX, OpKind::Reg32, OpKind::RegMem32, OpKind::Reg32_vvvv))
					.p66_W1_L0(entry_3(0xF7, ops::SHLX, OpKind::Reg64, OpKind::RegMem64, OpKind::Reg64_vvvv))
					.pF2_W0_L0(entry_3(0xF7, ops::SHRX, OpKind::Reg32, OpKind::RegMem32, OpKind::Reg32_vvvv))
					.pF2_W1_L0(entry_3(0xF7, ops::SHRX, OpKind::Reg64, OpKind::RegMem64, OpKind::Reg64_vvvv))
					.pF3_W0_L0(entry_3(0xF7, ops::SARX, OpKind::Reg32, OpKind::RegMem32, OpKind::Reg32_vvvv))
					.pF3_W1_L0(entry_3(0xF7, ops::SARX, OpKind::Reg64, OpKind::RegMem64, OpKind::Reg64_vvvv))
					,

		[0xFF] = VexEntry(0)
	};


	constexpr VexEntry VEX_Map_3[] = {
		[0x00] = VexEntry(0x00)
					.p66_W1_L1(entry_3(0x00, ops::VPERMQ, OpKind::RegYmm, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x01] = VexEntry(0x01)
					.p66_W1_L1(entry_3(0x01, ops::VPERMPD, OpKind::RegYmm, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x02] = VexEntry(0x02)
					.p66_W0_L0(entry_4(0x02, ops::VPBLENDD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_W0_L1(entry_4(0x02, ops::VPBLENDD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x04] = VexEntry(0x04)
					.p66_W0_L0(entry_3(0x04, ops::VPERMILPS, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_W0_L1(entry_3(0x04, ops::VPERMILPS, OpKind::RegYmm, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x05] = VexEntry(0x05)
					.p66_W0_L0(entry_3(0x05, ops::VPERMILPD, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_W0_L1(entry_3(0x05, ops::VPERMILPD, OpKind::RegYmm, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x06] = VexEntry(0x06)
					.p66_W0_L1(entry_4(0x06, ops::VPERM2F128, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,



		[0x08] = VexEntry(0x08)
					.p66_L0(entry_3(0x08, ops::VROUNDPS, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_3(0x08, ops::VROUNDPS, OpKind::RegYmm, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x09] = VexEntry(0x09)
					.p66_L0(entry_3(0x09, ops::VROUNDPD, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_3(0x09, ops::VROUNDPD, OpKind::RegYmm, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x0A] = VexEntry(0x0A)
					.p66(entry_4(0x0A, ops::VROUNDSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32, OpKind::Imm8))
					,

		[0x0B] = VexEntry(0x0B)
					.p66(entry_4(0x0B, ops::VROUNDSS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32, OpKind::Imm8))
					,

		[0x0C] = VexEntry(0x0C)
					.p66_L0(entry_4(0x0C, ops::VBLENDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_4(0x0C, ops::VBLENDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x0D] = VexEntry(0x0D)
					.p66_L0(entry_4(0x0D, ops::VBLENDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_4(0x0D, ops::VBLENDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x0E] = VexEntry(0x0E)
					.p66_L0(entry_4(0x0E, ops::VBLENDW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_4(0x0E, ops::VBLENDW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x0F] = VexEntry(0x0F)
					.p66_L0(entry_4(0x0F, ops::VPALIGNR, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_4(0x0F, ops::VPALIGNR, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,



		[0x14] = VexEntry(0x14)
					.p66_W0_L0(entry_3(0x14, ops::VPEXTRB, OpKind::RegMem8, OpKind::RegXmm, OpKind::Imm8))
					,

		[0x15] = VexEntry(0x15)
					.p66_W0_L0(entry_3(0x15, ops::VPEXTRW, OpKind::RegMem16, OpKind::RegXmm, OpKind::Imm8))
					,

		[0x16] = VexEntry(0x16)
					.p66_W0_L0(entry_3(0x16, ops::VPEXTRD, OpKind::RegMem32, OpKind::RegXmm, OpKind::Imm8))
					.p66_W1_L0(entry_3(0x16, ops::VPEXTRQ, OpKind::RegMem64, OpKind::RegXmm, OpKind::Imm8))
					,

		[0x17] = VexEntry(0x17)
					.p66_L0(entry_3(0x17, ops::VEXTRACTPS, OpKind::RegMem32, OpKind::RegXmm, OpKind::Imm8))
					,

		[0x18] = VexEntry(0x18)
					.p66_W0_L1(entry_4(0x18, ops::VINSERTF128, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					,

		[0x19] = VexEntry(0x19)
					.p66_W0_L1(entry_3(0x19, ops::VINSERTF128, OpKind::RegXmmMem128, OpKind::RegYmm, OpKind::Imm8))
					,

		[0x1D] = VexEntry(0x1D)
					.p66_W0_L0(entry_3(0x1D, ops::VCVTPS2PH, OpKind::RegXmmMem64, OpKind::RegXmm, OpKind::Imm8))
					.p66_W0_L1(entry_3(0x1D, ops::VCVTPS2PH, OpKind::RegXmmMem128, OpKind::RegYmm, OpKind::Imm8))
					,



		[0x20] = VexEntry(0x20)
					.p66_W0_L0(entry_4(0x20, ops::VPINSRB, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::Reg32Mem8, OpKind::Imm8))
					,

		[0x21] = VexEntry(0x21)
					.p66_L0(entry_4(0x21, ops::VINSERTPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem32, OpKind::Imm8))
					,

		[0x22] = VexEntry(0x22)
					.p66_W0_L0(entry_4(0x22, ops::VPINSRD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegMem32, OpKind::Imm8))
					.p66_W1_L0(entry_4(0x22, ops::VPINSRQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegMem64, OpKind::Imm8))
					,



		[0x38] = VexEntry(0x38)
					.p66_W0_L1(entry_4(0x38, ops::VINSERTI128, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					,

		[0x39] = VexEntry(0x39)
					.p66_W0_L1(entry_3(0x39, ops::VEXTRACTI128, OpKind::RegXmmMem128, OpKind::RegYmm, OpKind::Imm8))
					,



		[0x40] = VexEntry(0x40)
					.p66_L0(entry_4(0x40, ops::VDPPS, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_4(0x40, ops::VDPPS, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x41] = VexEntry(0x41)
					.p66_L0(entry_4(0x41, ops::VDPPD, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					,

		[0x42] = VexEntry(0x42)
					.p66_L0(entry_4(0x42, ops::VMPSADBW, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					.p66_L1(entry_4(0x42, ops::VMPSADBW, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		[0x44] = VexEntry(0x44)
					.p66_L0(entry_4(0x44, ops::VPCLMULQDQ, OpKind::RegXmm, OpKind::RegXmm_vvvv, OpKind::RegXmmMem128, OpKind::Imm8))
					,

		[0x46] = VexEntry(0x46)
					.p66_W0_L1(entry_4(0x46, ops::VPERM2I128, OpKind::RegYmm, OpKind::RegYmm_vvvv, OpKind::RegYmmMem256, OpKind::Imm8))
					,

		// VPERMILzz2PS and VPERMILzz2PD are apparently not real instructions... uwu

		[0x4A] = VexEntry(0x4A)
					.p66_L0(entry_4(0x4A, ops::VBLENDVPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_L1(entry_4(0x4A, ops::VBLENDVPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					,

		[0x4B] = VexEntry(0x4B)
					.p66_L0(entry_4(0x4B, ops::VBLENDVPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_L1(entry_4(0x4B, ops::VBLENDVPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					,

		[0x4C] = VexEntry(0x4C)
					.p66_L0(entry_4(0x4C, ops::VPBLENDVB, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_L1(entry_4(0x4C, ops::VPBLENDVB, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					,



		// this insanity is from FMA4, which... isn't really real anyway
		[0x5C] = VexEntry(0x5C)
					.p66_W0_L0(entry_4(0x5C, ops::VFMADDSUBPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x5C, ops::VFMADDSUBPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x5C, ops::VFMADDSUBPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x5C, ops::VFMADDSUBPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x5D] = VexEntry(0x5D)
					.p66_W0_L0(entry_4(0x5D, ops::VFMADDSUBPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x5D, ops::VFMADDSUBPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x5D, ops::VFMADDSUBPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x5D, ops::VFMADDSUBPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x5E] = VexEntry(0x5E)
					.p66_W0_L0(entry_4(0x5E, ops::VFMSUBADDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x5E, ops::VFMSUBADDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x5E, ops::VFMSUBADDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x5E, ops::VFMSUBADDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x5F] = VexEntry(0x5F)
					.p66_W0_L0(entry_4(0x5F, ops::VFMSUBADDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x5F, ops::VFMSUBADDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x5F, ops::VFMSUBADDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x5F, ops::VFMSUBADDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,


		[0x60] = VexEntry(0x60).p66_L0(entry_3(0x60, ops::VPCMPESTRM, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8)),
		[0x61] = VexEntry(0x61).p66_L0(entry_3(0x61, ops::VPCMPESTRI, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8)),
		[0x62] = VexEntry(0x62).p66_L0(entry_3(0x62, ops::VPCMPISTRM, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8)),
		[0x63] = VexEntry(0x63).p66_L0(entry_3(0x63, ops::VPCMPISTRI, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8)),



		[0x68] = VexEntry(0x68)
					.p66_W0_L0(entry_4(0x68, ops::VFMADDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x68, ops::VFMADDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x68, ops::VFMADDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x68, ops::VFMADDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x69] = VexEntry(0x69)
					.p66_W0_L0(entry_4(0x69, ops::VFMADDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x69, ops::VFMADDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x69, ops::VFMADDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x69, ops::VFMADDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x6A] = VexEntry(0x6A)
					.p66_W0_L0(entry_4(0x6A, ops::VFMADDSS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x6A, ops::VFMADDSS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x6A, ops::VFMADDSS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x6A, ops::VFMADDSS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x6B] = VexEntry(0x6B)
					.p66_W0_L0(entry_4(0x6B, ops::VFMADDSD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x6B, ops::VFMADDSD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x6B, ops::VFMADDSD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x6B, ops::VFMADDSD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x6C] = VexEntry(0x6C)
					.p66_W0_L0(entry_4(0x6C, ops::VFMSUBPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x6C, ops::VFMSUBPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x6C, ops::VFMSUBPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x6C, ops::VFMSUBPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x6D] = VexEntry(0x6D)
					.p66_W0_L0(entry_4(0x6D, ops::VFMSUBPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x6D, ops::VFMSUBPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x6D, ops::VFMSUBPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x6D, ops::VFMSUBPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x6E] = VexEntry(0x6E)
					.p66_W0_L0(entry_4(0x6E, ops::VFMSUBSS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x6E, ops::VFMSUBSS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x6E, ops::VFMSUBSS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x6E, ops::VFMSUBSS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x6F] = VexEntry(0x6F)
					.p66_W0_L0(entry_4(0x6F, ops::VFMSUBSD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x6F, ops::VFMSUBSD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x6F, ops::VFMSUBSD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x6F, ops::VFMSUBSD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,



		[0x78] = VexEntry(0x78)
					.p66_W0_L0(entry_4(0x78, ops::VFNMADDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x78, ops::VFNMADDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x78, ops::VFNMADDPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x78, ops::VFNMADDPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x79] = VexEntry(0x79)
					.p66_W0_L0(entry_4(0x79, ops::VFNMADDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x79, ops::VFNMADDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x79, ops::VFNMADDPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x79, ops::VFNMADDPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x7A] = VexEntry(0x7A)
					.p66_W0_L0(entry_4(0x7A, ops::VFNMADDSS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x7A, ops::VFNMADDSS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x7A, ops::VFNMADDSS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x7A, ops::VFNMADDSS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x7B] = VexEntry(0x7B)
					.p66_W0_L0(entry_4(0x7B, ops::VFNMADDSD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x7B, ops::VFNMADDSD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x7B, ops::VFNMADDSD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x7B, ops::VFNMADDSD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x7C] = VexEntry(0x7C)
					.p66_W0_L0(entry_4(0x7C, ops::VFNMSUBPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x7C, ops::VFNMSUBPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x7C, ops::VFNMSUBPS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x7C, ops::VFNMSUBPS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x7D] = VexEntry(0x7D)
					.p66_W0_L0(entry_4(0x7D, ops::VFNMSUBPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x7D, ops::VFNMSUBPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x7D, ops::VFNMSUBPD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x7D, ops::VFNMSUBPD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x7E] = VexEntry(0x7E)
					.p66_W0_L0(entry_4(0x7E, ops::VFNMSUBSS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x7E, ops::VFNMSUBSS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x7E, ops::VFNMSUBSS, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x7E, ops::VFNMSUBSS, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0x7F] = VexEntry(0x7F)
					.p66_W0_L0(entry_4(0x7F, ops::VFNMSUBSD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmmMem128, OpKind::RegXmm_TrailingImm8HighNib
					))
					.p66_W0_L1(entry_4(0x7F, ops::VFNMSUBSD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmmMem256, OpKind::RegYmm_TrailingImm8HighNib
					))
					.p66_W1_L0(entry_4(0x7F, ops::VFNMSUBSD, OpKind::RegXmm, OpKind::RegXmm_vvvv,
						OpKind::RegXmm_TrailingImm8HighNib, OpKind::RegXmmMem128
					))
					.p66_W1_L1(entry_4(0x7F, ops::VFNMSUBSD, OpKind::RegYmm, OpKind::RegYmm_vvvv,
						OpKind::RegYmm_TrailingImm8HighNib, OpKind::RegYmmMem256
					))
					,

		[0xDF] = VexEntry(0xDF)
					.p66_L0(entry_3(0xDF, ops::RORX, OpKind::RegXmm, OpKind::RegXmmMem128, OpKind::Imm8))
					,

		[0xF0] = VexEntry(0xF0)
					.pF2_W0_L0(entry_3(0xF0, ops::RORX, OpKind::Reg32, OpKind::RegMem32, OpKind::Imm8))
					.pF2_W1_L0(entry_3(0xF0, ops::RORX, OpKind::Reg64, OpKind::RegMem64, OpKind::Imm8))
					,

		[0xFF] = VexEntry(0)
	};
}
}

