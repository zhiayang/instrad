// 3dnow.h
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#pragma once

#include "entry.h"

namespace instrad::x64::tables
{
	constexpr TableEntry SecondaryOpcodeMap_0F_0F_3DNow[] = {
		[0x0C] = entry_2(0x0C, ops::PI2FW, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0x0D] = entry_2(0x0D, ops::PI2FD, OpKind::RegMmx, OpKind::RegMmxMem64),

		[0x1C] = entry_2(0x1C, ops::PF2IW, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0x1D] = entry_2(0x1D, ops::PF2ID, OpKind::RegMmx, OpKind::RegMmxMem64),

		[0x8A] = entry_2(0x8A, ops::PFNACC, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0x8E] = entry_2(0x8E, ops::PFPNACC, OpKind::RegMmx, OpKind::RegMmxMem64),

		[0x90] = entry_2(0x90, ops::PFCMPGE, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0x94] = entry_2(0x94, ops::PFMIN, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0x96] = entry_2(0x96, ops::PFRCP, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0x97] = entry_2(0x97, ops::PFRSQRT, OpKind::RegMmx, OpKind::RegMmxMem64),

		[0x9A] = entry_2(0x9A, ops::PFSUB, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0x9E] = entry_2(0x9E, ops::PFADD, OpKind::RegMmx, OpKind::RegMmxMem64),

		[0xA0] = entry_2(0xA0, ops::PFCMPGT, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0xA4] = entry_2(0xA4, ops::PFMAX, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0xA6] = entry_2(0xA6, ops::PFRCPIT1, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0xA7] = entry_2(0xA7, ops::PFRSQIT1, OpKind::RegMmx, OpKind::RegMmxMem64),

		[0xAA] = entry_2(0xAA, ops::PFSUBR, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0xAE] = entry_2(0xAE, ops::PFACC, OpKind::RegMmx, OpKind::RegMmxMem64),

		[0xB0] = entry_2(0xB0, ops::PFCMPEQ, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0xB4] = entry_2(0xB4, ops::PFMUL, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0xB6] = entry_2(0xB6, ops::PFRCPIT2, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0xB7] = entry_2(0xB7, ops::PMULHRW, OpKind::RegMmx, OpKind::RegMmxMem64),

		[0xBB] = entry_2(0xBB, ops::PSWAPD, OpKind::RegMmx, OpKind::RegMmxMem64),
		[0xBF] = entry_2(0xBF, ops::PAVGUSB, OpKind::RegMmx, OpKind::RegMmxMem64),

		[0xFF] = entry_blank
	};
}
