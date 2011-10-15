// ixload.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "ixload.h"
#include <stdio.h>
#include <cctype>

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

inline std::string int_to_string_2(int num)
{
	char s[1024];
	sprintf(s, "%d", num);
	return std::string(s);
}

inline std::string make_uppercase(const std::string s)
{
	using namespace std;
	std::string t;
	for(std::string::const_iterator i = s.begin(); i != s.end(); ++i)
		t += toupper(*i);
	return t;
}

struct PACKED(coff_section_header_t)
{
	char name[8];					// 8-byte, null-padded, ascii string. no terminating null if length is exactly 8 bytes.
	U4 virtual_size;				// (or physical address). total size of section when loaded in memory. if > size of raw data, is 0 padded.
	U4 virtual_address;				// address of 1st byte of section, when loaded in memory, relative to image base.

	// Note. raw_data_size is the size on disk. it's rounded up to a multiple of file alignment.
	// So, if the actual data size is 513 bytes but file alignment is 512, this will be 1024. But virtual_size will still be 513!
	U4 raw_data_size;				// size of initialized data on disk. must be a multiple of file alignment.

	U4 raw_data_pointer;			// file pointer to raw data. 0 if section contains only uninitialized data.
	U4 relocation_pointer;			// zero
	U4 linenum_pointer;				// file pointer to line number entries for section, or zero if none.
	U2 num_relocations;				// zero
	U2 num_linenum_entries;

	// 0x00000020 - contains executable code
	// 0x00000040 - contains initialized data
	// 0x00000080 - contains uninitialized data
	// 0x20000000 - executable
	// 0x40000000 - readable
	// 0x80000000 - writable
	U4 characteristics;
};

struct PACKED(coff_data_dir_t)
{
	U4 rva;							// rva = address of the table, when loaded, relative to the base of the image
	U4 size;						// size in bytes
};

struct PACKED(edt_t)				// export directory table
{
	U4 characteristics;
	U4 time_date_stamp;
	U2 major_version;
	U2 minor_version;
	U4 real_name_rva;
	U4 ordinals_base;			// e.g. 1, if ordinal 1 is at index 0
	U4 number_of_functions;
	U4 number_of_names;
	U4 rva_functions;
	U4 rva_names;
	U4 rva_ordinals;
};

struct PACKED(import_table_t)
{
	// import_lookup_table: mask by 0x7fffffff. If bit 31 was 1,
	// import by ordinal. The ordinal is the value &'d by 0x7fffffff.
	// else, import by hint/name. rva of image hint/name, with this
	// format:
	//    U2 hint;
	//    char asciiz_name[<variable size>].
	//    [another null is appended to pad to an even address]
	U4 import_lookup_table_rva;		// U4's, terminated by 0.
	U4 time_date_stamp;
	U4 forwarder_chain;
	U4 name_rva;				// asciiz e.g. "kernel32.dll"
	U4 iat_rva;					// thunk table (these are overwritten)
};

struct PACKED(dos_header_t)
{
	U1 signature[2];	// {'M','Z'} or {'Z','M'}
	U2 size_mod_512;
	U2 size_div_512;	// includes any partial last page
	U2 num_relocs;
	U2 header_size;		// in paragraphs (1 paragraph = 16 bytes)
	U2 min_mem;			// in paragraphs
	U2 max_mem;			// in paragraphs
	U2 initial_ss;		// relative to start of executable
	U2 initial_sp;
	U2 checksum;
	U2 initial_ip;
	U2 initial_cs;		// relative to start of executable
	U2 reloc_offset;	// within header. >= 0x40 for new executables.
	U2 overlay_num;		// normally 0 = main program
	// New executables: 
	U4 unknown;			// ???
	U2 behavior;		// ?
	U1 behavior_reserved[26];	// reserved for additional behavior info
	U4 new_header_offset;	// within disk file
};

struct PACKED(coff32_t)
{
	U2 machine;						// 0x14c = i386+
	U2 number_of_sections;
	U4 time_date_stamp;
	U4 pointer_to_symbol_table;		// file offset
	U4 number_of_symbols;
	U2 size_of_optional_header;
	U2 characteristics;

	U2 magic;						// 0x10b (PE32) or 0x20b (PE32+)
	U1 linker_version_major;
	U1 linker_version_minor;
	U4 code_size;					// total size of all text sections
	U4 initialized_data_size;		// total size of all data sections
	U4 uninitialized_data_size;		// total size of all bss sections
	U4 entrypoint_address;			// rel. to image base when executable is loaded into memory
	U4 base_of_code;				// rel. to image base when loaded into memory; beginning of code section
	U4 base_of_data;				// (absent in PE32+); beginning of data section

	U4 image_base;					// must be a multiple of 64kb. preferred address of 1st byte of image when loaded into memory.
	U4 section_alignment;			// >= file_alignment. typically 4096. sections are aligned to this when loaded into memory.
	U4 file_alignment;				// sections in the file are aligned to this many bytes. typically 512.
	U2 major_os_version;
	U2 minor_os_version;
	U2 major_image_version;
	U2 minor_image_version;
	U2 major_subsystem_version;
	U2 minor_subsystem_version;
	U4 reserved1;					// win32_version_value
	U4 size_of_image;				// size of image in bytes, including all headers. a multiple of section_alignment.
	U4 size_of_headers;				// DOS stub, PE header, and section headers rounded up to a multiple of file alignment
	U4 check_sum;
	U2 subsystem;					// 2=gui, 3=console
	U2 dll_characteristics;
	U4 stack_reserve;
	U4 stack_commit;
	U4 heap_reserve;
	U4 heap_commit;
	U4 loader_flags;				// obsolete
	U4 number_data_dirs;			// typically 16

	coff_data_dir_t data_dirs[1];	// variable size
};

struct PACKED(coff64_t)
{
	U2 machine;						// 0x14c = i386+
	U2 number_of_sections;
	U4 time_date_stamp;
	U4 pointer_to_symbol_table;		// file offset
	U4 number_of_symbols;
	U2 size_of_optional_header;
	U2 characteristics;

	U2 magic;						// 0x10b (PE32) or 0x20b (PE32+)
	U1 linker_version_major;
	U1 linker_version_minor;
	U4 code_size;					// total size of all text sections
	U4 initialized_data_size;		// total size of all data sections
	U4 uninitialized_data_size;		// total size of all bss sections
	U4 entrypoint_address;			// rel. to image base when executable is loaded into memory
	U4 base_of_code;				// rel. to image base when loaded into memory; beginning of code section

	U8 image_base;
	U4 section_alignmnet;
	U4 file_alignment;

	U2 major_os_version;
	U2 minor_os_version;
	U2 major_image_version;
	U2 minor_image_version;
	U2 major_subsystem_version;
	U2 minor_subsystem_version;
	U4 reserved1;					// win32_version_value
	U4 size_of_image;				// size of image in bytes, including all headers. a multiple of section_alignment.
	U4 size_of_headers;				// DOS stub, PE header, and section headers rounded up to a multiple of file alignment
	U4 check_sum;
	U2 subsystem;					// 2=gui, 3=console
	U2 dll_characteristics;
	U8 stack_reserve;
	U8 stack_commit;
	U8 heap_reserve;
	U8 heap_commit;
	U4 loader_flags;				// obsolete
	U4 number_data_dirs;			// typically 16

	coff_data_dir_t data_dirs[1];	// variable size
};

struct PACKED(delay_imp_t)
{
	U4 x1;
	U4 dll_name;
	U4 hmod;
	U4 iat;
	U4 xint;
	U4 biat;
	U4 uint;
	U4 date;
};

#ifdef _MSC_VER
#pragma pack(pop)
#endif

// Returns true on success, false on error.
bool ImageLoader::load_coff(bool *is64on32 /*= NULL*/)
{
	start_offset = 0;
	if(is64on32 != NULL)
		*is64on32 = false;

	U1 *image = raw_data;

	if(raw_size < 2)
		return false;

	dos_header_t *doshdr = (dos_header_t *)(image);
	if((doshdr->signature[0] == 'M' && doshdr->signature[1] == 'Z') ||
		(doshdr->signature[0] == 'Z' && doshdr->signature[1] == 'M')
		)
		;
	else
		return false;

	if(doshdr->new_header_offset + 3 >= raw_size)
		return false;

	U1 *x;
	x = image + doshdr->new_header_offset;
	if(x[0] != 'P' || x[1] != 'E' || x[2] != '\0' || x[3] != '\0')
		return false;

	U1 temp_bits = 0;
	coff32_t *coff32 = (coff32_t *)(image + doshdr->new_header_offset + 4);
	if(coff32->machine == 0x8664 && coff32->magic == 0x20b)
		temp_bits = 2;	// 64 bit mode
	else
	if(coff32->machine == 0x14c && coff32->magic == 0x10b)
		temp_bits = 1;	// 32 bit mode
	else
		return false;

	if(temp_bits == 2 && sizeof(void *) < 8)
	{
		if(is64on32 != NULL)
			*is64on32 = true;
		return false;
	}

	coff64_t *coff64 = (coff64_t *)(image + doshdr->new_header_offset + 4);

	U8 temp_image_size = (temp_bits == 2) ? coff64->size_of_image : coff32->size_of_image;
	U8 temp_image_size_2 = (temp_image_size >= raw_size) ? temp_image_size : raw_size;
	U1 *temp_data = new U1 [temp_image_size_2];
	memset(temp_data, 0, temp_image_size_2);
	memcpy(temp_data, raw_data, raw_size);

	image = temp_data;
	doshdr = (dos_header_t *)(temp_data);
	coff32 = (coff32_t *)(image + doshdr->new_header_offset + 4);
	coff64 = (coff64_t *)(image + doshdr->new_header_offset + 4);

	coff_section_header_t *sections;
	if(temp_bits == 2)
	{
		sections = (coff_section_header_t *)((U1 *)(coff64) +
			+ sizeof(coff64_t) - sizeof(coff_data_dir_t) +
			sizeof(coff_data_dir_t) * coff64->number_data_dirs
			);
	}
	else
	{
		sections = (coff_section_header_t *)((U1 *)(coff32) +
			+ sizeof(coff32_t) - sizeof(coff_data_dir_t) +
			sizeof(coff_data_dir_t) * coff32->number_data_dirs
			);
	}
	U2 number_of_sections = (temp_bits == 2) ? coff64->number_of_sections : coff32->number_of_sections;
	U8 temp_start_offset = TULL(0xffffffffffffffff);
	for(size_t i = 0; i < number_of_sections; ++i)
	{
		if(sections[i].raw_data_pointer == 0)
			continue;	// does this ever happen?
		U4 copy_size = (sections[i].raw_data_size < sections[i].virtual_size) ?
			sections[i].raw_data_size :
			sections[i].virtual_size
		;
		U1 *src = raw_data + sections[i].raw_data_pointer;
		U1 *dest = temp_data + sections[i].virtual_address;
		memset(dest, 0, sections[i].virtual_size);
		memcpy(dest, src, copy_size);
		if(sections[i].virtual_address < temp_start_offset)
			temp_start_offset = sections[i].virtual_address;
	}

	// Process exports.
	edt_t *exports = NULL;
	U4 *functions;
	U4 *names;
	U2 *ordinals;
	U4 num_export_fcns;

	if(temp_bits == 2)
	{
		if(coff64->data_dirs[0].rva != 0 && coff64->data_dirs[0].size != 0)
		{
			exports = (edt_t *)(image + coff64->data_dirs[0].rva);
			functions = (U4 *)(image + exports->rva_functions);
			names = (U4 *)(image + exports->rva_names);
			ordinals = (U2 *)(image + exports->rva_ordinals);
			num_export_fcns = exports->number_of_functions;
		}
	}
	else
	{
		if(coff32->data_dirs[0].rva != 0 && coff32->data_dirs[0].size != 0)
		{
			exports = (edt_t *)(image + coff32->data_dirs[0].rva);
			functions = (U4 *)(image + exports->rva_functions);
			names = (U4 *)(image + exports->rva_names);
			ordinals = (U2 *)(image + exports->rva_ordinals);
			num_export_fcns = exports->number_of_functions;
		}
	}

	is_library = false;
	if(exports != NULL && num_export_fcns != 0)
	{
		is_library = true;
	}

	// Success.
	entrypoint = (temp_bits == 2) ? coff64->entrypoint_address : coff32->entrypoint_address;
	origin = (temp_bits == 2) ? coff64->image_base : coff32->image_base;
	size = temp_image_size;
	bits = temp_bits;
	data = temp_data;
	start_offset = (temp_start_offset == TULL(0xffffffffffffffff)) ? 0 : temp_start_offset;
	return true;
}

bool LoadContext::load(bool *is64on32 /*= NULL*/)
{
	if(is64on32 != NULL)
		*is64on32 = false;
	bits = 0;
	import_forwarder_modules.clear();
	fwd_export_symbols.clear();
	regular_export_symbols.clear();
	regular_export_offsets.clear();
	load_log.clear();

	if(image_size < 2)
	{
		load_log.push_back("Image size too small");
		return false;
	}

	dos_header_t *doshdr = (dos_header_t *)(image_data);
	if((doshdr->signature[0] == 'M' && doshdr->signature[1] == 'Z') ||
		(doshdr->signature[0] == 'Z' && doshdr->signature[1] == 'M')
		)
		;
	else
	{
		load_log.push_back("Image does not begin wtih MZ or ZM.");
		return false;
	}

	if(doshdr->new_header_offset + 3 >= image_size)
	{
		load_log.push_back("Image is not in a recognized format.");
		return false;
	}

	U1 *x;
	x = image_data + doshdr->new_header_offset;
	if(x[0] != 'P' || x[1] != 'E' || x[2] != '\0' || x[3] != '\0')
	{
		load_log.push_back("Image is not in a recognized format.");
		return false;
	}

	coff32_t *coff32 = (coff32_t *)(image_data + doshdr->new_header_offset + 4);
	if(coff32->machine == 0x8664 && coff32->magic == 0x20b)
		bits = 2;	// 64 bit mode
	else
	if(coff32->machine == 0x14c && coff32->magic == 0x10b)
		bits = 1;	// 32 bit mode
	else
		return false;

	if(bits == 2 && sizeof(void *) < 8)
	{
		if(is64on32 != NULL)
			*is64on32 = true;
		return false;
	}

	coff64_t *coff64 = (coff64_t *)(image_data + doshdr->new_header_offset + 4);

	entrypoint = (bits == 2) ? coff64->entrypoint_address : coff32->entrypoint_address;
	origin = (bits == 2) ? coff64->image_base : coff32->image_base;
	U1 *image = image_data;

	// Process exports.
	edt_t *exports = NULL;
	U4 *functions;
	U4 *names;
	U2 *ordinals;
	U4 num_export_fcns;
	U4 rva0 = 0;
	U4 size0 = 0;
	U4 rva1 = 0;
	U4 size1 = 0;
	U4 rva13 = 0;
	U4 size13 = 0;

	if(bits == 2)
	{
		// 64bit mode.
		////if(coff64->data_dirs[0].rva != 0 && coff64->data_dirs[0].size != 0)
		{
			exports = (edt_t *)(image + coff64->data_dirs[0].rva);
			functions = (U4 *)(image + exports->rva_functions);
			names = (U4 *)(image + exports->rva_names);
			ordinals = (U2 *)(image + exports->rva_ordinals);
			num_export_fcns = exports->number_of_functions;
			if(coff64->number_data_dirs < 1)
				rva0 = size0 = 0;
			else
			{
				rva0 = coff64->data_dirs[0].rva;
				size0 = coff64->data_dirs[0].size;
			}
			if(coff64->number_data_dirs < 2)
				rva1 = size1 = 0;
			else
			{
				rva1 = coff64->data_dirs[1].rva;
				size1 = coff64->data_dirs[1].size;
			}
			if(coff64->number_data_dirs < 14)
				rva13 = size13 = 0;
			else
			{
				rva13 = coff64->data_dirs[13].rva;
				size13 = coff64->data_dirs[13].size;
			}
		}
	}
	else
	{
		// 32bit mode.
		////if(coff32->data_dirs[0].rva != 0 && coff32->data_dirs[0].size != 0)
		{
			exports = (edt_t *)(image + coff32->data_dirs[0].rva);
			functions = (U4 *)(image + exports->rva_functions);
			names = (U4 *)(image + exports->rva_names);
			ordinals = (U2 *)(image + exports->rva_ordinals);
			num_export_fcns = exports->number_of_functions;
			if(coff32->number_data_dirs < 1)
				rva0 = size0 = 0;
			else
			{
				rva0 = coff32->data_dirs[0].rva;
				size0 = coff32->data_dirs[0].size;
			}
			if(coff32->number_data_dirs < 2)
				rva1 = size1 = 0;
			else
			{
				rva1 = coff32->data_dirs[1].rva;
				size1 = coff32->data_dirs[1].size;
			}
			if(coff32->number_data_dirs < 14)
				rva13 = size13 = 0;
			else
			{
				rva13 = coff32->data_dirs[13].rva;
				size13 = coff32->data_dirs[13].size;
			}
		}
	}

	// Process exports.
	if(exports != NULL && num_export_fcns != 0)
	{
		std::map<U4, std::string> xexports;

		for(U4 x = 0; x < exports->number_of_functions; ++x)
		{
			U4 y = x + exports->ordinals_base;
			xexports[y] = std::string("#") + int_to_string_2(y);
		}
		for(U4 x = 0; x < exports->number_of_names; ++x)
		{
			xexports[ordinals[x] + exports->ordinals_base] = std::string((char *)(image + names[x]));
		}

		std::string newname;
		for(std::map<U4, std::string>::iterator i = xexports.begin(); i != xexports.end(); ++i)
		{
			U8 offset = functions[i->first - exports->ordinals_base];
					
			// note--offset may be 0 at this point. ignore that case.
		
			// check for forwarders.
			if(offset >= rva0 && offset < rva0 + size0)
			{
				// is a forwarder.
				std::string t = std::string((const char *)(&image[offset]));
						
				// This will fail if the module filename contains a dot ('.') character in it.
				std::string::iterator ix = t.begin();
				while(ix != t.end())
				{
					if(*ix == '.')
						break;
					++ix;
				}
				if(ix == t.end())
				{
					load_log.push_back(std::string("Unrecognized forwarder: ") + t);
					continue;
				}
				std::string fwd_module = make_uppercase(std::string(t.begin(), ix));
				import_forwarder_modules.insert(fwd_module);
				++ix;	// skip the dot

				fwd_export_symbols[i->second].module = fwd_module;
				fwd_export_symbols[i->second].symbol = std::string(ix, t.end());
			}
			else
			if(offset != 0)
			{
				regular_export_symbols[i->second] = offset;
				regular_export_offsets[offset] = &regular_export_symbols.find(i->second)->first;
			}
		}
	}	// process exports

	// Process imports.
	if(rva1 != 0 && size1 != 0)
	{
		import_table_t *imps;
		if(bits == 2)
			imps = (import_table_t *)(image + coff64->data_dirs[1].rva);
		else
			imps = (import_table_t *)(image + coff32->data_dirs[1].rva);

		while(imps->name_rva != 0 && imps->import_lookup_table_rva != 0)
		{
			std::string module_name((const char *)(image + imps->name_rva));
			module_name = make_uppercase(module_name);
					
			///import_modules.insert(module_name);
			///CoffLoaderImport &tmpImport = imports[module_name] = CoffLoaderImport();
			
			if(bits == 2)
			{
				U8 *x = (U8 *)(image + imps->import_lookup_table_rva);
				U8 y = 0;
				size_t count = 0;
				while(*x != 0)
				{
					std::string name;
					if(*x < TULL(0x8000000000000000))
					{
						name = std::string((char *)(image) + *x + 2);
					}
					else
					{
						U4 z = *x & 0xffff;
						name = std::string("#") + int_to_string_2(z);
					}
					U4 procaddr = imps->iat_rva + y;
						
					///U8 addr = image_address + (U8)(procaddr);
					///tmpImport.symbol = name;
					///tmpImport.address = addr;

					///std::cout << name << std::endl;
					// Do something with the imports, here.
					std::string fullName = module_name + std::string("!") + name;
					imports1[fullName] = procaddr;
					imports2[procaddr] = &imports1.find(fullName)->first;

					++x;
					y += 8;
					++count;
				}
				//std::cout << module_name << " (" << count << ")" << std::endl;
			}
			else
			{
				U4 *x = (U4 *)(image + imps->import_lookup_table_rva);
				U8 y = 0;
				size_t count = 0;
				while(*x != 0)
				{
					std::string name;
					if(*x < 0x80000000)
					{
						name = std::string((char *)(image) + *x + 2);
					}
					else
					{
						U4 z = *x & 0xffff;
						name = std::string("#") + int_to_string_2(z);
					}
					U4 procaddr = imps->iat_rva + y;
						
					///U8 addr = image_address + (U8)(procaddr);
					///tmpImport.symbol = name;
					///tmpImport.address = addr;

					//std::cout << name << std::endl;
					// Do something with the imports, here.
					std::string fullName = module_name + std::string("!") + name;
					imports1[fullName] = procaddr;
					imports2[procaddr] = &imports1.find(fullName)->first;

					++x;
					y += 4;
					++count;
				}
				//std::cout << module_name << " (" << count << ")" << std::endl;
			}
					
			++imps;
		}
	}	// process imports

	// Process delayed imports.
	if(rva13 != 0 && size13 != 0)
	{
		delay_imp_t *delay_imp = (delay_imp_t *)(image + rva13);
		U8 target_base = ((delay_imp->x1 & 1) == 0) ? origin : 0;

		while(delay_imp->dll_name != 0)
		{
			std::string dll_name = make_uppercase(std::string((const char *)(image + (U8)(delay_imp->dll_name) - target_base)));
			U8 yy = 0;
			for(;;)
			{
				std::string procname;
				U4 procaddr;

				if(bits == 1)
				{
					U4 *x = (U4 *)(image + delay_imp->xint - (U8)(target_base) + yy * (U8)(4));
					if(*x == 0)
						break;

					if(*x < 0x80000000)
					{
						procname = std::string((char *)(image) + (U8)(*x) + (U8)(2) - target_base);
					}
					else
					{
						U4 z = *x & 0xffff;
						procname = std::string("#") + int_to_string_2(z);
					}

					procaddr = (U8)(delay_imp->iat) + yy * 4 - target_base;
				}
				else
				{
					U8 *x = (U8 *)(image + delay_imp->xint - (U8)(target_base) + yy * (U8)(8));
					if(*x == 0)
						break;

					if(*x < TULL(0x8000000000000000))
					{
						procname = std::string((char *)(image) + (U8)(*x) + (U8)(2) - target_base);
					}
					else
					{
						U4 z = *x & 0xffff;
						procname = std::string("#") + int_to_string_2(z);
					}

					procaddr = (U8)(delay_imp->iat) + yy * 8 - target_base;
				}

				if(procname.empty())
					;	// ignore bogus delay-load entries, this should never happen
				else
				if(procaddr >= image_size)
				{
					load_log.push_back(std::string("address of delay-load symbol is invalid: ") + procname);
				}
				else
				{
					// Do something with the delayed import, here.
					std::string fullName = std::string("!") + dll_name + std::string("!") + procname;
					imports1[fullName] = procaddr;
					imports2[procaddr] = &imports1.find(fullName)->first;
				}

				++yy;
			}

			++delay_imp;
		}
	}	// process delayed imports

	return true;	// success
}
