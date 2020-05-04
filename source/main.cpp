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
#include "x64/decode.h"

static std::string print_intel(const instrad::x64::Instruction& instr)
{
	auto print_operand = [](const instrad::x64::Operand& op) -> std::string {
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
	for(size_t i = 0; i < instr.numBytes(); i++)
	{
		margin += zpr::sprint("%02x ", instr.bytes()[i]);
		col += 3;
	}

	while(col++ < 40)
		margin += zpr::sprint(" ");

	std::string prefix = "";
	if(instr.lockPrefix())  prefix = "lock ";
	if(instr.repPrefix())   prefix = "rep ";
	if(instr.repnzPrefix()) prefix = "repnz ";

	if(instr.operandCount() == 0)
		return margin + prefix + std::string(instr.op().mnemonic());

	else if(instr.operandCount() == 1)
		return margin + prefix + zpr::sprint("%s %s", instr.op().mnemonic(), print_operand(instr.dst()));

	else
		return margin + prefix + zpr::sprint("%s %s, %s", instr.op().mnemonic(), print_operand(instr.dst()), print_operand(instr.src()));
}

constexpr uint8_t test_bytes[] = {
	0x48, 0x8d, 0x46, 0x10, 0x48, 0x89, 0x06, 0x48, 0x8b, 0x44, 0x24, 0x10, 0x48, 0x39, 0xd8,
	0x75, 0x86, 0x66, 0x0f, 0x6f, 0x4c, 0x24, 0x20, 0x0f, 0x11, 0x4e, 0x10, 0xeb, 0x86, 0x65, 0xf3, 0xa4

	// 0xb8, 0x04, 0x00, 0x00, 0x00, 0xbb, 0x01, 0x00, 0x00, 0x00, 0xb9, 0x24, 0x00, 0x00, 0x00,
	// 0xba, 0x0d, 0x00, 0x00, 0x00, 0x0f, 0x05, 0xb8, 0x01, 0x00, 0x00, 0x00, 0xbb, 0x00, 0x00,
	// 0x00, 0x00, 0x0f, 0x05

#if 0
   0:   b8 04 00 00 00          mov    eax,0x4
   5:   bb 01 00 00 00          mov    ebx,0x1
   a:   b9 24 00 00 00          mov    ecx,0x24
   f:   ba 0d 00 00 00          mov    edx,0xd
  14:   0f 05                   syscall
  16:   b8 01 00 00 00          mov    eax,0x1
  1b:   bb 00 00 00 00          mov    ebx,0x0
  20:   0f 05                   syscall
#endif
};

constexpr instrad::x64::Instruction test_fixed()
{
	auto buf = instrad::Buffer(test_bytes, sizeof(test_bytes));
	return instrad::x64::read(buf, instrad::x64::ExecMode::Compat);
}


int main(int argc, char** argv)
{
	constexpr auto foo = test_fixed();
	printf("%zu\n", foo.numBytes());

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

	if constexpr (false)
	{
		auto buf = instrad::Buffer(test_bytes, sizeof(test_bytes));

		while((ssize_t) buf.remaining() > 0)
		{
			auto instr = instrad::x64::read(buf, instrad::x64::ExecMode::Long);
			zpr::print("%s\n", print_intel(instr));
		}
	}
	else
	{
		auto buf = instrad::Buffer(bytes, length);

		while((ssize_t) buf.remaining() > 0)
		{
			auto instr = instrad::x64::read(buf, instrad::x64::ExecMode::Long);
			zpr::print("%s\n", print_intel(instr));
		}
	}

	delete[] bytes;
}







#if 0
std::string print_gnu(const instrad::x64::Instruction& instr)
{
	auto print_operand = [](const instrad::x64::Operand& op) -> std::string {
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
