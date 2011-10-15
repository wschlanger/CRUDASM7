// ixload.h
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#ifndef l_ixload_h__ceres_included
#define l_ixload_h__ceres_included

#include <iostream>
#include <cstdio>
#include <string>
#include <list>
#include <string.h>
#include <stddef.h>
#include <set>
#include <map>

//#include "../syntax/ixdisasm.h"
//#include "../syntax/ixsyntax.h"
#include "../syntax/ixbasic.h"
#include "../../common/include/types.h"
//#include "../syntax/ixdecoder.h"

// All this class does is load an image and provide its origin, entrypoint, and number of bits.
// It does not process imports or exports.
class ImageLoader
{
	bool raw_mode;
	U1 *raw_data;
	long raw_size;
	U1 *data;
	long size;
	U1 bits;
	U8 entrypoint;			// offset in image of entrypoint
	U8 origin;				// image load offset
	bool is_library;
	U8 start_offset;

public:
	ImageLoader(U1 *fdT, long fsT)
	{
		raw_data = fdT;
		raw_size = fsT;
		raw_mode = true;
		data = raw_data;
		size = raw_size;
		bits = 0;			// 16 bit mode (or unsupported image type)
		entrypoint = 0;
		origin = 0;
		is_library = false;
		bool is64on32 = false;
		start_offset = 0;
		load_coff(&is64on32);
		if(is64on32)
			bits = 3;	// 64 bits but not supported (in 32 bit mode)
	}

	~ImageLoader()
	{
		if(!raw_mode)
			delete [] data;
	}

	bool isLibrary() const { return is_library; }

	// This returns the 0 for 16 bit mode (i.e. image is not 32bit or 64bit in a recognized format;
	// 1 for 32 bit mode, 2 for 64 bit mode. If this returns 0, then you do not want to use
	// LoadContext::load() on the image as that will simply fail.
	// This returns 3 if the image is 64bits but the current program was compiled in 32 bit mode.
	U1 getBits() const { return bits; }

	U8 getEntrypoint() const { return entrypoint; }

	long getLoadedSize() const { return size; }

	U1 *getLoadedData() const { return data; }

	U8 getStartOffset() const { return start_offset; }

	bool load_coff(bool *is64on32 = NULL);
};

struct CoffModuleSymbol
{
	std::string module;
	std::string symbol;
};

class LoadContext
{
	U8 image_size;
	U1 *image_data;
	U1 bits;
	U8 entrypoint;			// offset in image of entrypoint
	U8 origin;				// image load offset (if DLL, this is expected load offset)

	// Contains a log of error messages encountered during load.
	std::list<std::string> load_log;

	std::set<std::string> import_forwarder_modules;
	std::map<std::string, CoffModuleSymbol> fwd_export_symbols;
	std::map<std::string, U8> regular_export_symbols;
	std::map<U8, const std::string *> regular_export_offsets;

	std::map<std::string, U8> imports1;				// module.dll!FcnName or !module.dll!FcnName (latter is delayed import)
	std::map<U8, const std::string *> imports2;

public:
	LoadContext(U8 image_sizeT, U1 *image_dataT)
	{
		image_size = image_sizeT;
		image_data = image_dataT;
	}

	std::map<U8, const std::string *>::const_iterator imports_begin() { return imports2.begin(); }
	std::map<U8, const std::string *>::const_iterator imports_end() { return imports2.end(); }

	// Returns true on success, false on error.
	bool load(bool *is64on32 = NULL);

	std::list<std::string>::const_iterator load_log_begin() { return load_log.begin(); }
	std::list<std::string>::const_iterator load_log_end() { return load_log.end(); }
	U8 get_origin() const { return origin; }
	std::map<U8, const std::string *>::const_iterator exports_begin() { return regular_export_offsets.begin(); }
	std::map<U8, const std::string *>::const_iterator exports_end() { return regular_export_offsets.end(); }
	const std::map<U8, const std::string *> &get_exports() { return regular_export_offsets; }
};

#endif	// l_ixload_h__ceres_included
