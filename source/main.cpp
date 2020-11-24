// main.cpp
// Copyright (c) 2020, zhiayang
// Licensed under the Apache License Version 2.0.

#include <stdio.h>

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "zpr.h"
#include "buffer.h"
#include "x86/decode.h"

static std::string print_intel(const instrad::x86::Instruction& instr, uint64_t ip, const uint8_t* bytes, size_t len)
{
	auto print_operand = [&ip](const instrad::x86::Operand& op) -> std::string {
		if(op.isRegister())
		{
			return zpr::sprint("%s", op.reg().name());
		}
		else if(op.isImmediate())
		{
			int64_t value = op.imm();
			if(op.immediateSize() == 8)  value = (uint8_t) value;
			if(op.immediateSize() == 16) value = (uint16_t) value;
			if(op.immediateSize() == 32) value = (uint32_t) value;

			// printf("[%#lx]", op.imm());
			return zpr::sprint("%#lx", value);
		}
		else if(op.isRelativeOffset())
		{
			return zpr::sprint("%#lx", ip + op.ofs().offset());
		}
		else if(op.isMemory())
		{
			auto& mem = op.mem();
			auto& base = mem.base();
			auto& idx = mem.index();

			std::string size = "";
			switch(mem.bits())
			{
				case 8:     size = "BYTE PTR ";     break;
				case 16:    size = "WORD PTR ";     break;
				case 32:    size = "DWORD PTR ";    break;
				case 64:    size = "QWORD PTR ";    break;
				case 80:    size = "TWORD PTR ";    break;
				case 128:   size = "XMMWORD PTR ";  break;
				case 256:   size = "YMMWORD PTR ";  break;
				case 512:   size = "ZMMWORD PTR ";  break;
				default:    break;
			}


			std::string segment = "";
			if(mem.segment().present())
				segment = zpr::sprint("%s:", mem.segment().name());

			// you can't scale a displacement, so we're fine here.
			if(!base.present() && !idx.present())
				return size + segment + zpr::sprint("[%#x]", mem.displacement());

			std::string tmp = size + segment + "[";
			if(base.present())
			{
				tmp += zpr::sprint("%s", base.name());
				if(idx.present() || op.mem().displacement() != 0 || op.mem().scale() != 1)
					tmp += " + ";
			}

			if(idx.present())
				tmp += zpr::sprint("%s", idx.name());

			if(op.mem().scale() != 1)
				tmp += zpr::sprint("*%d", op.mem().scale());

			if(op.mem().displacement() != 0)
			{
				if(idx.present() || op.mem().scale() != 1)
					tmp += " + ";

				tmp += zpr::sprint("%#x", op.mem().displacement());
			}

			return tmp + "]";
		}

		return "<??>";
	};

	std::string margin = "    ";

	// print the bytes
	size_t col = 0;
	for(size_t i = 0; i < len; i++)
	{
		margin += zpr::sprint("%02x ", bytes[i]);
		col += 3;
	}

	while(col++ < 30)
		margin += zpr::sprint(" ");

	std::string prefix = "";
	if(instr.lockPrefix())  prefix = "lock ";
	if(instr.repPrefix())   prefix = "rep ";
	if(instr.repnzPrefix()) prefix = "repnz ";

	auto ret = zpr::sprint("%s%s%s ", margin, prefix, std::string(instr.op().mnemonic()));
	if(instr.operandCount() == 1)
	{
		ret += print_operand(instr.dst());
	}
	else if(instr.operandCount() == 2)
	{
		ret += print_operand(instr.dst()); ret += ", ";
		ret += print_operand(instr.src());
	}
	else if(instr.operandCount() == 3)
	{
		ret += print_operand(instr.dst()); ret += ", ";
		ret += print_operand(instr.src()); ret += ", ";
		ret += print_operand(instr.ext());
	}
	else
	{
		ret += print_operand(instr.dst()); ret += ", ";
		ret += print_operand(instr.src()); ret += ", ";
		ret += print_operand(instr.ext()); ret += ", ";
		ret += print_operand(instr.op4());
	}

	return ret;
}



static std::string print_att(const instrad::x86::Instruction& instr, uint64_t ip, const uint8_t* bytes, size_t len)
{
	std::string instr_suffix = "";

	auto print_operand = [&ip, &instr_suffix](const instrad::x86::Operand& op) -> std::string {
		if(op.isRegister())
		{
			return zpr::sprint("%%%s", op.reg().name());
		}
		else if(op.isImmediate())
		{
			int64_t value = op.imm();
			if(op.immediateSize() == 8)  value = (uint8_t) value;   instr_suffix = "b";
			if(op.immediateSize() == 16) value = (uint16_t) value;  instr_suffix = "w";
			if(op.immediateSize() == 32) value = (uint32_t) value;  instr_suffix = "l";
			if(op.immediateSize() == 64) value = (uint64_t) value;  instr_suffix = "absq";

			// printf("[%#lx]", op.imm());
			return zpr::sprint("$%#lx", value);
		}
		else if(op.isRelativeOffset())
		{
			return zpr::sprint("%#lx", ip + op.ofs().offset());
		}
		else if(op.isMemory())
		{
			auto& mem = op.mem();
			auto& base = mem.base();
			auto& idx = mem.index();

			switch(mem.bits())
			{
				case 8:     instr_suffix = "b"; break;
				case 16:    instr_suffix = "w"; break;
				case 32:    instr_suffix = "l"; break;
				case 64:    instr_suffix = "q"; break;

				// there should be no ambiguity in these cases,
				// so in theory we should not need a suffix.
				// (either way, idk what the suffixes would be,
				// and there don't seem to be any defined)
				default:
					break;
			}

			if(mem.isDisplacement64Bits())
				instr_suffix = "abs" + instr_suffix;

			std::string segment = mem.segment().present()
				? zpr::sprint("%%%s:", mem.segment().name())
				: "";

			// you can't scale a displacement, so we're fine here.
			if(!base.present() && !idx.present())
				return segment + zpr::sprint("%#x", mem.displacement());

			std::string tmp = segment;
			if(op.mem().displacement() != 0)
				tmp += zpr::sprint("%#lx", op.mem().displacement());

			tmp += "(";

			if(base.present())
				tmp += zpr::sprint("%%%s", base.name());

			if(idx.present())
				tmp += zpr::sprint(", %%%s", idx.name());

			if(op.mem().scale() != 1)
				tmp += zpr::sprint(", %d", op.mem().scale());

			tmp += ")";
			return tmp;
		}

		return "??";
	};

	std::string margin = "    ";

	// print the bytes
	size_t col = 0;
	for(size_t i = 0; i < len; i++)
	{
		margin += zpr::sprint("%02x ", bytes[i]);
		col += 3;
	}

	while(col++ < 30)
		margin += zpr::sprint(" ");

	std::string prefix = "";
	if(instr.lockPrefix())  prefix = "lock ";
	if(instr.repPrefix())   prefix = "rep ";
	if(instr.repnzPrefix()) prefix = "repnz ";


	// only print the instruction last, because we need to parse the operand to know the suffix.

	std::string operands;
	if(instr.operandCount() == 1)
	{
		operands += print_operand(instr.dst());
	}
	else if(instr.operandCount() == 2)
	{
		operands += print_operand(instr.src()); operands += ", ";
		operands += print_operand(instr.dst());
	}
	else if(instr.operandCount() == 3)
	{
		operands += print_operand(instr.ext()); operands += ", ";
		operands += print_operand(instr.src()); operands += ", ";
		operands += print_operand(instr.dst());
	}
	else
	{
		operands += print_operand(instr.op4()); operands += ", ";
		operands += print_operand(instr.ext()); operands += ", ";
		operands += print_operand(instr.src()); operands += ", ";
		operands += print_operand(instr.dst());
	}

	return zpr::sprint("%s%s%s%s %s", margin, prefix, std::string(instr.op().mnemonic()), instr_suffix, operands);
}









constexpr uint8_t test_bytes[] = {
	0xC5, 0xFB, 0x10, 0x07, 0xC5, 0xFB, 0x10, 0xC1, 0xC5, 0xF8, 0xAE, 0x10, 0xC4, 0xE2, 0x79, 0x18, 0x00, 0xC4, 0xE2, 0x79, 0x18, 0xC0,

	0x48, 0x8d, 0x46, 0x10, 0x48, 0x89, 0x06, 0x48, 0x8b, 0x44, 0x24, 0x10, 0x48, 0x39, 0xd8,
	0x75, 0x86, 0x66, 0x0f, 0x6f, 0x4c, 0x24, 0x20, 0x0f, 0x11, 0x4e, 0x10, 0xeb, 0x86, 0x65, 0xf3, 0xa4,

	0x49, 0x8b, 0x7c, 0x24, 0x30,

	0x48, 0xa1, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa
};

constexpr instrad::x86::Instruction test_fixed()
{
	auto buf = instrad::Buffer(test_bytes, sizeof(test_bytes));
	return instrad::x86::read(buf, instrad::x86::ExecMode::Compat);
}


int main(int argc, char** argv)
{
	// constexpr auto foo = test_fixed();
	// printf("%zu\n", foo.numBytes());

	if(argc < 2)
	{
		zpr::println("give filename");
		return 1;
	}

	auto file = std::ifstream(argv[1], std::ios::binary);
	if(!file)
	{
		perror("failed to open file");
		return 1;
	}

	file.seekg(0, std::ios::end);
	size_t length = file.tellg();
	file.seekg(0, std::ios::beg);

	length = std::min(length, (size_t) 1024);

	auto bytes = new uint8_t[length];
	file.read((char*) &bytes[0], length);

	if constexpr (true)
		memcpy(bytes, test_bytes, sizeof(test_bytes));

	uint64_t ip = 0;
	auto buf = instrad::Buffer(bytes, length);

	while((ssize_t) buf.remaining() > 0)
	{
		auto pos = buf.position();
		auto instr = instrad::x86::read(buf, instrad::x86::ExecMode::Long);
		auto len = (pos - buf.position());

		zpr::print("%4x:  %s\n", ip, print_intel(instr, ip, bytes, len));

		ip += len;
	}

	delete[] bytes;
}







#if 0
std::string print_gnu(const instrad::x86::Instruction& instr)
{
	auto print_operand = [](const instrad::x86::Operand& op) -> std::string {
		if(op.isRegister())
		{
			return zpr::sprint("%%%s", op.reg().name());
		}
		else if(op.isImmediate())
		{
			return zpr::sprint("$%#x", op.imm());
		}
		else if(op.isMemory())
		{
			auto& mem = op.mem();
			auto& base = mem.base();
			auto& idx = mem.index();

			// you can't scale a displacement, so we're fine here.
			if(!base.present() && !idx.present())
				return zpr::sprint("%#x", mem.displacement());

			std::string tmp;
			if(op.mem().displacement() != 0)
				tmp = zpr::sprint("%#x", op.mem().displacement());

			tmp += "(";
			if(base.present())
				tmp += zpr::sprint("%%%s", base.name());

			if(idx.present() || op.mem().scale() != 1)
			{
				tmp += ",";
				if(idx.present())
					tmp += zpr::sprint("%%%s", idx.name());

				if(op.mem().scale() != 1)
					tmp += zpr::sprint(",%d", op.mem().scale());
			}

			return tmp + ")";
		}

		return "<??>";
	};

	if(instr.operandCount() == 0)
		return std::string(instr.op().mnemonic());

	else if(instr.operandCount() == 1)
		return zpr::sprint("%s %s", instr.op().mnemonic(), print_operand(instr.dst()));

	else
		return zpr::sprint("%s %s, %s", instr.op().mnemonic(), print_operand(instr.src()), print_operand(instr.dst()));
}
#endif
