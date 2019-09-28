// Copyright(c) 2013-2019, mCODE A/S
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and /or other materials provided with the distribution.
//  3. Neither the name of the copyright holders nor the
//     names of its contributors may be used to endorse or promote products
//     derived from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
#pragma once


#include "Exports.h"
#include "M3DCore/PlatformDef.h"
#include "M3DCore/List.h"
#include "M3DCore/Set.h"
#include "M3DCore/Map.h"
#include "M3DCore/Path.h"
#include "M3DCore/MString.h"
#include "Err.h"


#define M3D_TITLE "SnaX"
#define M3D_COMPANY "mCODE AS"

namespace m3d
{


class Application;
class Engine;
class Graphics;
class Document;
class DocumentManager;
class ClassManager;
class Class;
class DocumentLoader;
class DocumentSaver;
class ChipManager;
class Chip;
class FunctionSignatureManager;
class Function;
class DataBuffer;
class FunctionCall;


using ChipTypeIndex = unsigned;
const ChipTypeIndex InvalidChipTypeIndex = -1;

typedef unsigned int ChipID;
const ChipID InvalidChipID = 0;

typedef unsigned int DocumentID;
const DocumentID InvalidDocumentID = 0;

typedef unsigned int ClassID;
const ClassID InvalidClassID = 0;

typedef unsigned FunctionSignatureID;
const FunctionSignatureID InvalidFunctionSignatureID = 0;

typedef unsigned ClassInstanceID;
const ClassInstanceID InvalidClassInstanceID = 0;


enum MessageSeverity { DINFO, INFO, NOTICE, WARN, FATAL, ALWAYS };

// Utility function for easily setting debug messages.
extern void M3DENGINE_API msg(MessageSeverity severity, String message);
extern void M3DENGINE_API msg(MessageSeverity severity, String message, const Class *clazz);
extern void M3DENGINE_API msg(MessageSeverity severity, String message, const Chip *chip);

struct M3DENGINE_API Version
{
	union
	{
		unsigned version;
		unsigned char elements[4]; // Major,Minor,Build,Revision.
	};
	Version(unsigned v = 0x01000000u) : version(v) {}
	Version(unsigned char ma, unsigned char mi, unsigned char bu, unsigned char re) : version((unsigned(ma) << 24u | unsigned(mi) << 16u | unsigned(bu) << 8u | unsigned(re))) {}

	bool operator==(Version b) const { return version == b.version; }
	bool operator!=(Version b) const { return version != b.version; }
	bool operator<(Version b) const { return version < b.version; }
	bool operator<=(Version b) const { return version <= b.version; }

	operator unsigned() const { return version; }
};

#define VERSION1 m3d::Version(0x01000000u)


}

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif



