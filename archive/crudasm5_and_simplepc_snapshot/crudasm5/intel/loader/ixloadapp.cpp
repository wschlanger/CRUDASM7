// ixloadapp.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#define NO_INT_TYPES 1
#include "ixload.h"
#include <iostream>
#include <cstdio>
#include <cctype>

#include <Windows.h>
#include <Psapi.h>
#include <direct.h>

#ifndef PSAPI_VERSION
#error PSAPI_VERSION not defined
#endif

#if PSAPI_VERSION != 1
#error PSAPI_VERSION should be defined as 1
#endif

struct syminfo_t
{
	bool delay_load;
	void *addr;

	syminfo_t()
	{
		delay_load = false;
		addr = NULL;
	}

	syminfo_t(bool delayT)
	{
		delay_load = delayT;
		addr = NULL;
	}
};

struct CompleteLoader
{
	ImageLoader *loader;
	LoadContext *context;
	U1 *fd;
	long fs;

	CompleteLoader(std::string filename)
	{
		using namespace std;
		FILE *f = fopen(filename.c_str(), "rb");
		if(feof(f))
		{
			fclose(f);
			throw __LINE__;
		}
		fseek(f, -1, SEEK_END);
		fs = 1 + ftell(f);
		rewind(f);
		fd = new U1 [fs];
		if(fread(fd, fs, 1, f) != 1)
		{
			fclose(f);
			delete [] fd;
			throw __LINE__;
		}
		fclose(f);

		loader = new ImageLoader(fd, fs);
		bool is64on32 = false;
		if(!loader->load_coff(&is64on32))
		{
			delete loader;
			delete [] fd;
			throw __LINE__;
		}
		if(is64on32)
		{
			delete loader;
			delete [] fd;
			throw __LINE__;
		}

		context = new LoadContext(loader->getLoadedSize(), loader->getLoadedData());
		is64on32 = false;
		if(!context->load(&is64on32))
		{
			delete context;
			delete loader;
			delete [] fd;
			throw __LINE__;
		}
		if(is64on32)
		{
			delete context;
			delete loader;
			delete [] fd;
			throw __LINE__;
		}

		// Success.
		delete [] fd;
		fd = loader->getLoadedData();
		fs = loader->getLoadedSize();
	}

	~CompleteLoader()
	{
		delete context;
		delete loader;
	}
};

bool exists(std::string fn)
{
	using namespace std;
	FILE *fi = fopen(fn.c_str(), "rb");
	if(fi != NULL)
		fclose(fi);
	return fi != NULL;
}

std::string getPath(std::string fn)
{
	std::string::iterator i = fn.end();
	while(i != fn.begin())
	{
		--i;
		if(*i == '\\' || *i == '/')
			return std::string(fn.begin(), i);
	}
	return "";
}

std::string getFileName(std::string fn)
{
	std::string::iterator i = fn.end();
	while(i != fn.begin())
	{
		--i;
		if(*i == '\\' || *i == '/')
			return std::string(++i, fn.end());
	}
	return fn;
}

std::string convert_uppercase(std::string s)
{
	std::string t;
	for(std::string::iterator i = s.begin(); i != s.end(); ++i)
	{
		using namespace std;
		t += toupper(*i);
	}
	return t;
}

int convert_string_to_int(std::string s)
{
	int x = 0;
	for(std::string::iterator i = s.begin(); i != s.end(); ++i)
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

struct module_t
{
	HMODULE module;
	
	U8 module_address;		// This is (U8)(module) & ~TULL(3).
	U8 module_size;
	CompleteLoader *ldr;
	bool is_target;
	bool is_imported;

	module_t()
	{
		module = NULL;
		ldr = NULL;
		is_target = false;
		is_imported = false;
	}

	~module_t()
	{
		if(ldr != NULL)
		{
			delete ldr;
		}
	}

	void load(std::string fn)
	{
		if(ldr == NULL)
		{
			ldr = new CompleteLoader(fn);
			module_address = (U8)(module) & ~TULL(3);
			module_size = ldr->fs;
		}
	}
};

int main(int argc, char **argv)
{
	HANDLE curProc = GetCurrentProcess();

	DWORD neededSize = 1;
	char c = '\0';
	EnumProcessModules(curProc, (HMODULE *)(&c), 1, &neededSize);

	if(neededSize == 1)
	{
		std::cout << "Error enumerating modules! [case 1]" << std::endl;
		return 1;
	}

	HMODULE *modules = new HMODULE [(neededSize / sizeof(HMODULE) + 1) * 2];
	DWORD sizeAvail = ((neededSize / sizeof(HMODULE) + 1) * 2) * sizeof(HMODULE);

	if(EnumProcessModules(curProc, modules, sizeAvail, &neededSize) == 0)
	{
		delete [] modules;
		std::cout << "Error enumerating modules! [case 2]" << std::endl;
		return 1;
	}

	std::set<std::string> beforeModules;

	DWORD numEntries = neededSize / sizeof(HMODULE);
	char fname[16385];
	for(DWORD i = 0; i < numEntries; ++i)
	{
		fname[0] = '\0';
		if(GetModuleFileNameA(modules[i], fname, 16384) == 0)
		{
			delete [] modules;
			std::cout << "Error enumerating modules! [case 3]" << std::endl;
			return 1;
		}
		beforeModules.insert(fname);
		//std::cout << (i + 1) << ".\t" << fname << std::endl;
	}

	// Now load the target, ourselves. This will tell us whether it's a library or an executable.
	std::string target = "c:\\windows\\system32\\gdi32.dll";	// 32bit or 64bit gdi32.dll module
	std::string search_dir_2 = "";
if(argc == 2)  target = argv[1];

	try
	{
		if(!exists(target))
		{
			std::cout << "File does not exist: " << target << std::endl;
			throw __LINE__;
		}

		CompleteLoader ldrTarget(target);

		HMODULE targetLib = NULL;
		if(ldrTarget.loader->isLibrary())
		{
			if((targetLib = LoadLibraryA(target.c_str())) == NULL)
			{
				std::cout << "Unable to directly load target library: " << target << std::endl;
				throw __LINE__;
			}
		}

		// Now call LoadLibrary() on all dependencies. Do not fail if a delay-load library can't be loaded.
		std::map<std::string, std::map<std::string, syminfo_t> > imports;

		for(std::map<U8, const std::string *>::const_iterator i = ldrTarget.context->imports_begin(); i != ldrTarget.context->imports_end(); ++i)
		{
			std::string s = *i->second;
			if(s.empty())
				continue;
			bool delay_load = false;
			if(s[0] == '!')
			{
				s = std::string(++s.begin(), s.end());
				delay_load = true;
			}
			std::string::iterator j = s.end();
			while(j != s.begin())
			{
				--j;
				if(*j == '!')
					break;
			}
			std::string module = std::string(s.begin(), j);
			++j;
			std::string symbol = std::string(j, s.end());

			imports[module][symbol] = syminfo_t(delay_load);
		}

		GetSystemDirectoryA(fname, 16384);
		std::string search_dir_1 = fname;
		_chdir(fname);

		std::string path = getPath(target);
		if(!path.empty())
		{
			search_dir_1 = path;
		}

		for(std::map<std::string, std::map<std::string, syminfo_t> >::iterator i = imports.begin(); i != imports.end(); ++i)
		{
			_chdir(search_dir_1.c_str());
			HMODULE hmod = NULL;
			if((hmod = LoadLibraryExA(i->first.c_str(), NULL, 0)) == NULL)
			{
				bool ok = false;
				if(!search_dir_2.empty())
				{
					_chdir(search_dir_2.c_str());
					if((hmod = LoadLibraryExA(i->first.c_str(), NULL, 0)) != NULL)
						ok = true;
				}

				if(!ok)
				{
					std::cout << "WARNING--error loading library: " << i->first << std::endl;
					std::cout << std::hex << GetLastError() << std::dec << std::endl;
					continue;
				}
			}
			
			for(std::map<std::string, syminfo_t>::iterator j = i->second.begin(); j != i->second.end(); ++j)
			{
				std::string proc = j->first;
				if(proc.empty())
					continue;
				if(proc[0] == '#')
				{
					proc = std::string(++proc.begin(), proc.end());
					int val = convert_string_to_int(proc);
					j->second.addr = GetProcAddress(hmod, (LPCSTR)(val));
				}
				else
					j->second.addr = GetProcAddress(hmod, proc.c_str());
				if(j->second.addr == NULL)
				{
					std::cout << "WARNING: unable to locate symbol " << i->first << "!" << j->first;
					if(j->second.delay_load)
						std::cout << " (delay loaded)";
					std::cout << std::endl;
				}
			}
		}

		delete [] modules;
		modules = NULL;
		neededSize = 1;
		EnumProcessModules(curProc, (HMODULE *)(&c), 1, &neededSize);

		modules = new HMODULE [(neededSize / sizeof(HMODULE) + 1) * 2];
		sizeAvail = ((neededSize / sizeof(HMODULE) + 1) * 2) * sizeof(HMODULE);

		if(EnumProcessModules(curProc, modules, sizeAvail, &neededSize) == 0)
		{
			throw __LINE__;
		}

		std::map<std::string, module_t> afterModules;

		numEntries = neededSize / sizeof(HMODULE);
		int result = 1;
		for(DWORD i = 0; i < numEntries; ++i)
		{
			fname[0] = '\0';
			if(GetModuleFileNameA(modules[i], fname, 16384) == 0)
			{
				std::cout << "Error enumerating modules! [case 3a]" << std::endl;
				throw 1;
			}
			std::string tmpname = convert_uppercase(getFileName(fname));
			if(tmpname == "IXLOADAPP.EXE" || tmpname == "IXLOAD32.EXE" || tmpname == "IXLOAD64.EXE")
			{
				continue;	// don't process ourself!
			}
			afterModules[fname].module = modules[i];
			if(modules[i] != NULL && modules[i] == targetLib)
			{
				afterModules[fname].is_target = true;
			}

			afterModules[fname].load(fname);

//std::cout << (result++) << ".\t" << convert_uppercase(fname) << std::endl;
		}

		// For each module, determine its address & maximum possible length.
		// This way, we can look at the imports and we'll know to which module
		// (and symbol!) they refer. We will do a manual load of all modules
		// refered to by an import of the target, which will provide us with
		// the imported symbol's TRUE name.
		for(std::map<std::string, module_t>::iterator i = afterModules.begin(); i != afterModules.end(); ++i)
		{
			for(std::map<std::string, module_t>::iterator j = afterModules.begin(); j != afterModules.end(); ++j)
			{
				if(i->first == j->first)
					continue;
				if(i->second.module_address < j->second.module_address)
				{
					// module i comes before module j in memory.
					if(i->second.module_address + i->second.module_size >= j->second.module_address)
					{
						i->second.module_size = j->second.module_address - i->second.module_address;
					}
				}
			}
		}

		for(std::map<std::string, std::map<std::string, syminfo_t> >::iterator i = imports.begin(); i != imports.end(); ++i)
		{
			for(std::map<std::string, syminfo_t>::iterator j = i->second.begin(); j != i->second.end(); ++j)
			{
				void *addr = j->second.addr;
				bool found = false;
				if(addr != NULL)
				{
					U8 ofs = (U8)(addr);
					for(std::map<std::string, module_t>::iterator k = afterModules.begin(); k != afterModules.end(); ++k)
					{
						if(ofs >= k->second.module_address && ofs < k->second.module_address + k->second.module_size)
						{
							found = true;
							std::string mod = convert_uppercase(getFileName(k->first));

							const std::map<U8, const std::string *> &exports = k->second.ldr->context->get_exports();
							std::map<U8, const std::string *>::const_iterator h = exports.find(ofs - k->second.module_address);

							std::string symbol_name;
							if(h != exports.end())
							{
								symbol_name = *h->second;
							}

							k->second.is_imported = true;

							/*if(i->first != mod || j->first != symbol_name)
								std::cout << i->first << "!" << j->first << " -> " << mod << "!" << symbol_name << std::endl;*/
///std::cout << i->first << "!" << j->first << " -> " << mod << "!" << symbol_name << std::endl;
						}
					}
				}
				if(!found && addr != NULL)
				{
					std::cout << "WARNING: Symbol " << i->first << "!" << j->first << " : not found!" << std::endl;
				}
			}
		}

		std::string targetMod = convert_uppercase(getFileName(target));
		std::cout << targetMod << std::endl;
		for(std::map<std::string, module_t>::iterator k = afterModules.begin(); k != afterModules.end(); ++k)
		{
			if(k->second.is_imported && !k->second.is_target)
			{
				std::string mod = convert_uppercase(getFileName(k->first));
				std::cout << mod << std::endl;
			}
		}
	}
	catch(int x)
	{
		std::cout << "Error loading modules! Code " << x << std::endl;
		if(modules != NULL)
		{
			delete [] modules;
		}
		return 1;
	}

	delete [] modules;

	return 0;
}
