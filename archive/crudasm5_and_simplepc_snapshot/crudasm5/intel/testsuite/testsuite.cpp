// testsuite.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "../syntax/ixdisasm.h"
#include "../syntax/ixsyntax.h"
#include "../syntax/ixbasic.h"
#include "../../common/include/types.h"
#include "../syntax/ixdecoder.h"
#include <QtCore/QCoreApplication>
#include <QtXml>
#include <iostream>
#include <QFile>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <cstdio>
#include <fstream>
#include <cctype>

// Returns -1 on decode error. Prints differences between NDISASM and our disassembler to STDOUT, upon detection.
long compare_code(std::string opcode, std::string operands, IntelSyntax &syntax, IntelDecoder &decoder, U1 *fd1, long pos, long fs1, U1 bits, std::ostream &xx, long pos2)
{
	size_t numInsns = syntax.getInsnCount();
	long len = 0;

	ix_decoder_state s;
	IntelMCDecoder dec(&s, &decoder);
	ix_icode_t icode;
	if(!dec.decode(&icode, fs1 - pos, bits, fd1 + pos))
	{
		std::cout << "FATAL ERROR DECODING OFFSET " << std::hex << pos << std::dec << " (" << (16 << bits) << " bits)!" << std::endl;
		std::cout << "Instruction was: " << opcode << " " << operands << std::endl;
		std::cout << "Error was: " << dec.explain_error() << std::endl;
		return -1;
	}

	len = s.insn_size;
	std::string internal_insn_name = syntax.getSymbol(syntax.getInsn(icode.insn)->proto.alias);

	///if(internal_insn_name != opcode)
	{
		IntelDisassembler disassembler(&decoder);
		char dest[1024];
		dest[0] = '\0';
		disassembler.disasm(dest, NULL, NULL, pos2, &s, &icode, bits);
		if(!operands.empty())
			operands = std::string(" ") + operands;
		std::string nasmcode = opcode + operands;
		if(nasmcode != std::string(dest))
		{
			std::cout << nasmcode << " : " << dest << std::endl;
		}
		xx << dest << std::endl;
		///std::cout << std::hex << pos << std::dec << " " << opcode << " " << internal_insn_name << std::endl;
	}

	return len;
}

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	if(argc != 2 && argc != 3)
	{
		std::cout << "Usage: testsuite intel.xml [--pass2]" << std::endl;
		return 1;
	}

	if(argc == 2)
	{
		std::cout << "Copyright (C) 2011 Willow Schlanger. All rights reserved." << std::endl;
	}

	QFile file(argv[1]);
	if(!file.open(QIODevice::ReadOnly))
	{
		std::cout << "Error: unable to open file: " << argv[1] << std::endl;
		return 1;
	}
	QDomDocument doc("intel");
	if(!doc.setContent(&file))
	{
		file.close();
		std::cout << "Error: parse error reading file: " << argv[1] << std::endl;
		return 1;
	}
	file.close();

	QDomElement root = doc.documentElement();
	if(root.tagName() != "intel")
	{
		std::cout << "Error: invalid file: " << argv[1] << std::endl;
		std::cout << "root tag name is: " << qPrintable(root.tagName()) << std::endl;
		return 1;
	}

	IntelSyntax syntax;
	IntelDecoder decoder(&syntax);
	std::cout << "Loading... " << std::flush;
	QString result = syntax.load(root);
	QString result2;
	if(result == "")
		result2 = decoder.init();
	std::cout << "done" << std::endl;
	if(result != "")
	{
		std::cout << "Error parsing Intel XML for syntax: " << qPrintable(result) << std::endl;
		return 1;
	}
	if(result2 != "")
	{
		std::cout << "Error initializing Intel decoder table: " << qPrintable(result2) << std::endl;
		return 1;
	}

	size_t numInsns = syntax.getInsnCount();
	if(numInsns == 0)
	{
		std::cout << "Error: nothing to do - no Intel instructions!" << std::endl;
		return 1;
	}

	const char *fnames[] = {"test16", "test32", "test64"};

	if(argc == 3 && std::string(argv[2]) != std::string("--pass2"))
	{
		std::string fname1 = std::string(argv[2]);
		FILE *fi1 = fopen(fname1.c_str(), "rb");
		if(fi1 == NULL || feof(fi1))
		{
			if(fi1 != NULL)
				fclose(fi1);
			std::cout << "Error: " << fname1 << " does not exist or is empty." << std::endl;
			return 1;
		}
		fseek(fi1, -1, SEEK_END);
		long fs1 = ftell(fi1) + 1;
		rewind(fi1);
		U1 *fd1 = new U1 [fs1];
		fread(fd1, fs1, 1, fi1);
		fclose(fi1);

		long pos = 0;
		int bits = 2;	// 64 bits
		while(pos < fs1)
		{
			ix_decoder_state s;
			IntelMCDecoder dec(&s, &decoder);
			ix_icode_t icode;
			if(!dec.decode(&icode, fs1 - pos, bits, fd1 + pos))
			{
				std::cout << "FATAL ERROR DECODING OFFSET " << std::hex << pos << std::dec << " (" << (16 << bits) << " bits)!" << std::endl;
				std::cout << "Error was: " << dec.explain_error() << std::endl;
				delete [] fd1;
				return -1;
			}

			unsigned long len = s.insn_size;
			std::string internal_insn_name = syntax.getSymbol(syntax.getInsn(icode.insn)->proto.alias);

			IntelDisassembler disassembler(&decoder);
			char dest[1024];
			dest[0] = '\0';
			disassembler.disasm(dest, NULL, NULL, pos, &s, &icode, bits);
			std::cout << dest << std::endl;

			pos += len;
		}

		delete [] fd1;
		return 0;
	}

	if(argc == 3 && std::string(argv[2]) == std::string("--pass2"))
	{
		bool fail = false;

		std::ofstream xx("testxx.asm");
		xx << "org 0" << std::endl;
		long pos2 = 0;
		for(int bits = 0; bits < 3; ++bits)
		{
			std::string fname1 = std::string(fnames[bits]) + ".bin";
			std::string fname2 = std::string(fnames[bits]) + ".txt";
			std::cout << "Reading " << fname1 << "..." << std::endl;
			FILE *fi1 = fopen(fname1.c_str(), "rb");
			if(fi1 == NULL || feof(fi1))
			{
				if(fi1 != NULL)
					fclose(fi1);
				std::cout << "Error: " << fname1 << " does not exist or is empty." << std::endl;
				return 1;
			}
			fseek(fi1, -1, SEEK_END);
			long fs1 = ftell(fi1) + 1;
			rewind(fi1);
			U1 *fd1 = new U1 [fs1];
			fread(fd1, fs1, 1, fi1);
			fclose(fi1);

			std::ifstream fi2(fname2.c_str());
			xx << "bits " << (16 << bits) << std::endl;

			if(fi2)
			{
				std::string ln;
				long pos = 0;
				while(std::getline(fi2, ln))
				{
					using namespace std;
					if(ln.empty())
						continue;
					if(ln[0] == '-' || isspace(ln[0]))
						continue;

					std::string::iterator e = ln.end();
					std::string::iterator b = ln.begin();

					while(b != e)
					{
						if(isspace(*b))
							break;
						++b;
					}
					while(isspace(*b))
						++b;
					while(b != e)
					{
						if(isspace(*b))
							break;
						++b;
					}
					while(isspace(*b))
						++b;

					ln = std::string(b, e);

					std::string opcode, operands;
					bool gotspace = false;
					for(b = ln.begin(); b != ln.end(); ++b)
					{
						if(isspace(*b))
						{
							gotspace = true;
							break;
						}
					}

					if(!gotspace)
						opcode = ln;
					else
					{
						opcode = std::string(ln.begin(), b);
						while(isspace(*b))
							++b;
						operands = std::string(b, ln.end());
					}

					long x = compare_code(opcode, operands, syntax, decoder, fd1, pos, fs1, bits, xx, pos2);
					if(x < 0)
					{
						std::cout << "\nGiving up!" << std::endl;
						fail = true;
						break;
					}
					pos += x;
					pos2 += x;
					//std::cout << "(" << opcode << ")(" << operands << ")" << std::endl;
				}
			}
			else
			{
				std::cerr << "WARNING: File " << fname2 << " not found!" << std::endl;
			}

			delete [] fd1;
			if(fail)
				return 1;
		}

		return 0;
	}

	for(int bits = 0; bits < 3; ++bits)
	{
		std::string ofname = std::string(fnames[bits]) + ".bin";
		std::cout << "Writing " << ofname << "..." << std::endl;
		using namespace std;
		FILE *of = fopen(ofname.c_str(), "wb");

		for(size_t insn = 0; insn < numInsns; ++insn)
		{
			IntelSyntaxInsn *insnT = syntax.getInsn(insn);
			for(std::list<IntelSyntaxEncoding>::iterator enc = insnT->encodings.begin(); enc != insnT->encodings.end(); ++enc)
			{
				IntelSyntaxEncoding *encT = &*enc;

				if(encT->tags.find(syntax.getSymbol("asm_skip")) != encT->tags.end())
					continue;
				if(encT->tags.find(syntax.getSymbol("no64")) != encT->tags.end() && bits == 2)
					continue;
				if(encT->tags.find(syntax.getSymbol("like_arpl")) != encT->tags.end() && bits == 2)
					continue;
				if(encT->tags.find(syntax.getSymbol("like_movsxd")) != encT->tags.end() && bits != 2)
					continue;

				if(encT->fx == 2)
					fputc(0xf2, of);
				else
				if(encT->fx == 3)
					fputc(0xf3, of);

				bool got66 = false;
				if(encT->op66 == 2)
				{
					got66 = true;
					fputc(0x66, of);
				}

				// if any arguments have a size of osz_min32 then emit 0x66 in 16 bit mode.
				if(bits == 0)
				{
					for(int i = 0; i < 4; ++i)
					{
						if(encT->argtype[i] == IntelArgTypes::Tvoid)
							break;
						if(encT->argsize[i] == IntelArgSizes::Tosz_min32)
						{
							if(!got66)
								fputc(0x66, of);
							got66 = true;
							break;
						}
					}
				}

				if(encT->tags.find(syntax.getSymbol("like_movsxd")) != encT->tags.end() && bits == 2)
				{
					fputc(0x48, of);
				}

				// Emit opcode.
				if(encT->opcode1 < 0x100)
				{
					fputc(encT->opcode1, of);
					if(encT->opcode2 != 0x100)
						fputc(encT->opcode2, of);
				}
				else
				if(encT->opcode1 < 0x200)
				{
					fputc(0x0f, of);
					fputc(encT->opcode1 & 0xff, of);
					if(encT->opcode2 != 0x100)
						fputc(encT->opcode2, of);
				}
				else
				{
					fputc(0x0f, of);
					fputc(0x0f, of);
					fputc(encT->opcode1 & 0xff, of);
				}

				// modr/m.
				if(encT->regop != 0xf)
				{
					U1 modrm = 0;
					if(encT->mod < 4)
						modrm |= (U1)(encT->mod) << 6;
					if(encT->rm < 8)
						modrm |= (U1)(encT->rm);
					if(encT->regop < 8)
						modrm |= (U1)(encT->regop) << 3;

					fputc(modrm, of);
				}

				// 3DNow! opcode byte.
				if(encT->opcode1 >= 0x200)
					fputc(encT->opcode2 & 0xff, of);

				// Immediates & displacement (mutually exclusive).
				for(size_t j = 0; j < 4; ++j)
				{
					if(encT->argtype[j] == IntelArgTypes::Tvoid)
						break;
					if(encT->argtype[j] == IntelArgTypes::Tmem_fulldisp)
					{
						for(int k = 0; k < (2 << bits); ++k)
							fputc(0x55, of);
					}
					else
					if(IntelArgTypes::is_imm(encT->argtype[j]) && encT->argtype[j] != IntelArgTypes::Timm_implict)
					{
						S1 imm_size = encT->argsize[j];

						if(imm_size == IntelArgSizes::Tasz)
							imm_size = 2 << bits;	// this will never happen
						else
						if(imm_size < 0)
						{
							// size is some variation of osz.
							if(encT->tags.find(syntax.getSymbol("sx_byte")) != encT->tags.end())
								imm_size = 1;
							else
							{
								imm_size = 2 << bits;

								if(encT->tags.find(syntax.getSymbol("imm64_sx32")) == encT->tags.end())
								{
									if(encT->tags.find(syntax.getSymbol("imm64_disp")) != encT->tags.end())
									{
										if(imm_size == 8)
											imm_size = 4;		// no rex.w used here
									}
									else
									{
										if(bits == 2)		// 64 bit mode
											std::cerr << "WARNING: test case generation expects imm64_sx32 or imm64_disp for " << syntax.getSymbol(insnT->proto.name) << std::endl;
									}
								}
								else
								{
									///std::cerr << "2" << std::endl;
									if(imm_size == 8)
										imm_size = 4;		// we don't use rex.w
								}
							}
						}

						for(int k = 0; k < imm_size; ++k)
							fputc(0xaa, of);
					}
				}
			}
		}

		fclose(of);
	}

	return 0;	// app.exec();
}
