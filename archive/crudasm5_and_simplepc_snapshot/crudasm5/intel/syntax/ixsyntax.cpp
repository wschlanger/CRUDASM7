// ixsyntax.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "ixsyntax.h"
#include "ixdisasm.h"

// B1 -> 1
// B2 -> 2
// ...
// other -> -1
int getBSize(std::string s)
{
	if(s.size() < 2)
		return -1;
	if(s[0] != 'B')
		return -1;
	int x = 0;
	for(std::string::iterator i = ++s.begin(); i != s.end(); ++i)
	{
		if(*i >= '0' && *i <= '9')
		{
			x = (x * 10) + (*i - '0');
		}
		else
			return -1;
	}
	return x;
}

std::string IntelSyntax::getSymbol(U4 num)
{
	if(num >= strTabSymbols.size())
		return "";	// bad input number
	return *strTabSymbols[num];
}

U4 IntelSyntax::getSymbol(std::string symbol)
{
	std::map<std::string, U4>::iterator i = strTabNums.find(symbol);
	if(i != strTabNums.end())
	{
		return i->second;
	}
	U4 count = strTabSymbols.size();
	strTabNums[symbol] = count;
	strTabSymbols.push_back(&strTabNums.find(symbol)->first);
	return count;
}

QString IntelSyntax::load(QDomElement &intel_xml)
{
	if(intel_xml.tagName() != "intel")
		return "Invalid XML: root tag is not 'intel'!";

	// This is version 1.0 of IntelSyntax. Anything else must be a newer version.
	if(intel_xml.attribute("minver") != "1.0")
		return QString("Minimum version required exceeds current version (1.0): ") + intel_xml.attribute("minver");

	QDomNode insns = intel_xml.firstChild();
	if(insns.isNull())
		return QString("Invalid XML: missing tag 'insns'!");
	QDomElement insnsE = insns.toElement();
	if(insnsE.isNull())
		return QString("Invalid XML: missing tag 'insns'!");
	if(insnsE.tagName() != "insns")
		return QString("Invalid XML: unexpected tag '") + insnsE.tagName() + QString("', wanted 'insns'!");

	QDomNode insn = insnsE.firstChild();
	while(!insn.isNull())
	{
		QDomElement insnE = insn.toElement();
		if(!insnE.isNull())
		{
			if(insnE.tagName() != "insn")
			{
				return QString("Invalid XML: unexpected tag '") + insnE.tagName() + QString("', wanted 'insn'!");
			}

			QString name = insnE.attribute("name");
			QString alias = insnE.attribute("alias");

			QString numArgsStr = insnE.attribute("numargs");
			bool ok = false;
			int numArgs = numArgsStr.toInt(&ok);
			if(!ok || numArgs < 0 || numArgs > 4)
			{
				return QString("Invalid XML: Instruction ") + name + QString(": bad number of arguments '") + numArgsStr + QString("'!");
			}

			U4 insnNameNum = getSymbol(qPrintable(name));
			if(insnNames.find(insnNameNum) != insnNames.end())
			{
				return QString("Invalid XML: Instruction ") + name + QString(": duplicate instruction!");
			}

			insnList.push_back(IntelSyntaxInsn());
			IntelSyntaxInsn *dest = &insnList.back();
			U4 insnNumber = insnsX.size();
			dest->insn_num = insnNumber;
			insnsX.push_back(dest);
			insnNames[insnNameNum] = insnNumber;
			dest->proto.name = getSymbol(qPrintable(name));
			dest->proto.alias = getSymbol(qPrintable(alias));
			dest->proto.numArgs = numArgs;

			// Get arg sizes.
			const char *argN[4] = {"arg0", "arg1", "arg2", "arg3"};
			std::map<std::string, S1> templNums;
			S1 nextTemplateNum = -1;
			for(int a = 0; a < 4; ++a)
			{
				QString argS = argN[a];
				QString size = insnE.attribute(argS);
				std::string sizeS = qPrintable(size);
				if(size == "void")
					dest->proto.argSizes[a] = 0;
				int sizeI = getBSize(sizeS);
				if(sizeI != -1)
				{
					if(sizeI > 0 && sizeI <= 127)
						dest->proto.argSizes[a] = sizeI;
					else
						return QString("Invalid XML: Instruction ") + name + QString(": unsupported argument size: ") + size;
				}
				else
				{
					if(templNums.find(sizeS) != templNums.end())
					{
						dest->proto.argSizes[a] = templNums[sizeS];
					}
					else
					{
						dest->proto.argSizes[a] = nextTemplateNum;
						templNums[sizeS] = nextTemplateNum;
						--nextTemplateNum;
					}
				}
			}

			// Get tags.
			int numAttrs = insnE.attributes().count();
			for(int i = 0; i < numAttrs; ++i)
			{
				QDomNode n = insnE.attributes().item(i);
				if(!n.isNull())
				{
					QDomAttr attr = n.toAttr();
					if(!attr.isNull())
					{
						if(attr.value() == "true")
						{
							std::string nameS = qPrintable(attr.name());
							dest->proto.tags.insert(getSymbol(nameS));
						}
					}
				}
			}

			bool foundEncodings = false;
			QDomNode m = insnE.firstChild();
			while(!m.isNull())
			{
				QDomElement mm = m.toElement();

				if(!mm.isNull() && mm.tagName() == "dis")
				{
					QString dis_type = mm.attribute("type");

					if(dest->disasm.find(getSymbol(qPrintable(dis_type))) != dest->disasm.end())
					{
						return QString("Invalid XML: Instruction ") + name + QString(": repeat disassembly type: ") + dis_type;
					}
					std::list<IntelSyntaxDisStmt> &ilst = dest->disasm[getSymbol(qPrintable(dis_type))];
					
					// Go through all child <dis>...</dis> tags.
					QDomNode n = mm.firstChild();
					while(!n.isNull())
					{
						QDomElement nn = n.toElement();

						if(!nn.isNull() && nn.tagName() == "dis_stmt")
						{
							QString dis_stmt_ident = nn.attribute("ident");

							//std::cout << qPrintable(name) << " : " << qPrintable(dis_type) << " : " << qPrintable(dis_stmt_ident) << std::endl;
							ilst.push_back(IntelSyntaxDisStmt());

							std::string ident = qPrintable(dis_stmt_ident);
							ilst.back().ident = getSymbol(ident);

							QDomNode q = nn.firstChild();
							int num_args = 0;
							while(!q.isNull())
							{
								QDomElement qq = q.toElement();

								if(qq.tagName() == "dis_arg")
								{
									++num_args;
									ilst.back().args.push_back(qPrintable(qq.attribute("value")));
									std::string hexval = ilst.back().args.back();
									int base = 16;

									// write_arg() has some arguments that can be booleans.
									if(hexval == "true")
										hexval = ilst.back().args.back() = "1";
									else
									if(hexval == "false")
										hexval = ilst.back().args.back() = "0";

									if(hexval.size() >= 2 && hexval[0] == '0' && hexval[1] == 'x')
									{
										std::string::iterator b = hexval.begin();
										++b;
										++b;
										hexval = std::string(b, hexval.end());
									}
									else
										base = 10;
									bool ok = false;
									QString tmp(hexval.c_str());
									int val = tmp.toInt(&ok, base);
									if(!ok)
									{
										return QString("Invalid XML: Instruction ") + name + QString(": bad disassembly numeric constant: ") + QString(ilst.back().args.back().c_str());
									}
								}
								else
								if(qq.tagName() == "dis_arg_str")
								{
									++num_args;
									ilst.back().args.push_back(std::string("\"") + qPrintable(qq.attribute("value")) + std::string("\""));
								}

								q = q.nextSibling();
							}

							if(!IntelDisassembler::isValidIdent(ident, num_args))
							{
								return QString("Invalid XML: Instruction ") + name + QString(": bad disassembly method call: ") + QString(ident.c_str());
							}
						}

						n = n.nextSibling();
					}
				}
				else
				if(!mm.isNull() && mm.tagName() == "encodings")
				{
					// Now go through all child <encoding.../> tags.
					QDomNode n = mm.firstChild();
					while(!n.isNull())
					{
						QDomElement nn = n.toElement();

						if(!nn.isNull() && nn.tagName() == "encoding")
						{
							foundEncodings = true;

							dest->encodings.push_back(IntelSyntaxEncoding());
							IntelSyntaxEncoding *enc = &dest->encodings.back();
							enc->encoding_num = encodings.size();
							encodings.push_back(enc);

							QString opcode1 = nn.attribute("opcode1");
							QString opcode2 = nn.attribute("opcode2");
							QString mod = nn.attribute("mod");
							QString regop = nn.attribute("regop");
							QString rm = nn.attribute("rm");
							QString op66 = nn.attribute("op66");
							QString fx = nn.attribute("fx");

							int tmp;
							bool ok;

							ok = false;
							tmp = opcode1.toInt(&ok, 16);
							if(!ok || tmp < 0 || tmp > 0x2ff)
								return QString("Invalid XML: Instruction ") + name + QString(": invalid opcode1: ") + opcode1;
							enc->opcode1 = tmp;

							if(opcode2 != "")
							{
								ok = false;
								tmp = opcode2.toInt(&ok, 16);
								if(!ok || tmp < 0 || tmp > 0xff)
									return QString("Invalid XML: Instruction ") + name + QString(": invalid opcode2: ") + opcode2;
								enc->opcode2 = tmp;
							}

							if(mod != "")
							{
								if(mod == "m")
									enc->mod = 4;
								else
								{
									ok = false;
									tmp = mod.toInt(&ok, 10);
									if(!ok || tmp < 0 || tmp > 3)
										return QString("Invalid XML: Instruction ") + name + QString(": invalid mod: ") + mod;
									enc->mod = tmp;
								}
							}

							if(regop != "")
							{
								if(regop == "r")
									enc->regop = 8;
								else
								{
									ok = false;
									tmp = regop.toInt(&ok, 10);
									if(!ok || tmp < 0 || tmp > 7)
										return QString("Invalid XML: Instruction ") + name + QString(": invalid regop: ") + regop;
									enc->regop = tmp;
								}
							}

							if(rm != "")
							{
								ok = false;
								tmp = rm.toInt(&ok, 10);
								if(!ok || tmp < 0 || tmp > 7)
									return QString("Invalid XML: Instruction ") + name + QString(": invalid rm: ") + rm;
								enc->rm = tmp;
							}

							enc->insn = insnNumber;

							if(op66 != "")
							{
								if(op66 == "no66")
									enc->op66 = 1;
								else
								if(op66 == "66")
									enc->op66 = 2;
								else
									return QString("Invalid XML: Instruction ") + name + QString(": invalid op66: ") + op66;
							}

							if(fx != "")
							{
								if(fx == "nofx")
									enc->fx = 0;
								else
								if(fx == "f2")
									enc->fx = 2;
								else
								if(fx == "f3")
									enc->fx = 3;
								else
									return QString("Invalid XML: Instruction ") + name + QString(": invalid fx: ") + fx;
							}

							// Get tags.
							int numAttrs = nn.attributes().count();
							for(int i = 0; i < numAttrs; ++i)
							{
								QDomNode n = nn.attributes().item(i);
								if(!n.isNull())
								{
									QDomAttr attr = n.toAttr();
									if(!attr.isNull())
									{
										if(attr.value() == "true")
										{
											std::string nameS = qPrintable(attr.name());
											enc->tags.insert(getSymbol(nameS));
										}
									}
								}
							}

							// Get arguments here (arg0..arg3).
							for(int i = 0; i < numArgs; ++i)
							{
								QString tsv = nn.attribute(argN[i]);
								if(tsv == "" || tsv == "void")
								{
									return QString("Invalid XML: Instruction ") + name + QString(": bad or missing argument type/size/value");
								}
								std::string tmp = qPrintable(tsv);
								std::string tsvT[3];
								int count = 0;
								tmp += ":";
								std::string tmp2;
								for(std::string::iterator ii = tmp.begin(); ii != tmp.end(); ++ii)
								{
									if(*ii == ':')
									{
										if(count >= 3 || tmp2 == "")
											return QString("Invalid XML: Instruction ") + name + QString(": bad or missing argument type/size/value");
										tsvT[count] = tmp2;
										++count;
										tmp2 = "";
									}
									else
										tmp2 += *ii;
								}
								if(count != 3)
									return QString("Invalid XML: Instruction ") + name + QString(": bad or missing argument type/size/value");
								// tsvT[0] = type, tsvT[1] = size, tsvT[2] = value.

								enc->argtype[i] = IntelArgTypes::getType(tsvT[0]);
								if(enc->argtype[i] == IntelArgTypes::Tvoid)
									return QString("Invalid XML: Instruction ") + name + QString(": unsupported argument type: ") + QString(tsvT[0].c_str());

								enc->argsize[i] = IntelArgSizes::getSize(tsvT[1]);
								if(enc->argsize[i] == IntelArgSizes::Tvoid)
									return QString("Invalid XML: Instruction ") + name + QString(": unsupported argument size: ") + QString(tsvT[1].c_str());

								QString argvalue = tsvT[2].c_str();
								bool ok = false;
								int argvalueNum = argvalue.toInt(&ok, 16);
								if(!ok || argvalueNum < 0 || argvalueNum > 0xff)
									return QString("Invalid XML: Instruction ") + name + QString(": unsupported argument value: ") + QString(tsvT[2].c_str());
								enc->argvalue[i] = argvalueNum;

								if(argvalueNum == 0xff && ((enc->argtype[i] & 0x80) == 0) && IntelArgTypes::is_reg(enc->argtype[i]))
								{
									// OK, we got a reg argument that is non-implict. It is not /mem.
									///std::cout << std::hex << qPrintable(opcode1) << " " << (unsigned)enc->argtype[i] << " " << (unsigned)argvalueNum << " " << qPrintable(regop) << std::dec << std::endl;
									if(regop != "r" && enc->tags.find(getSymbol("reg_rm")) == enc->tags.end() && enc->tags.find(getSymbol("reg_base")) == enc->tags.end())
										return QString("Invalid XML: Instruction ") + name + QString(": don\'t know where to get register (use reg_rm or reg_base, or /r)");
								}
							}
						}

						n = n.nextSibling();
					}
				}

				if(!foundEncodings)
					return QString("Invalid XML: Instruction ") + name + QString(": contains no encodings!");

				m = m.nextSibling();
			}
		}

		insn = insn.nextSibling();
	}

	return "";	// success
}
