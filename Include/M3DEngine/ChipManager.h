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
#include "ChipDef.h"
#include "M3DCore/Map.h"
#include "M3DCore/List.h"
#include "M3DCore/Path.h"
#include "M3DCore/SlimRWLock.h"
#include <atomic>


namespace m3d
{

typedef const wchar_t* (*GETPACKETNAME)();
typedef unsigned (*GETCHIPCOUNT)();
typedef const ChipDesc &(*GETCHIPTYPE)(unsigned);
typedef bool (*ONPACKETQUERY)();
typedef bool (*ONPACKETLOAD)();
typedef void (*ONPACKETUNLOAD)();
typedef Chip *(*CHIPFACTORY)();
typedef void (*ADDDEPENDENCIES)(ProjectDependencies &deps);
typedef unsigned (*GETSUPPORTEDPLATFORMS)();


struct Packet;

struct ChipInfo
{
	Packet* packet;
	struct {
		String name;
		GUID type;
		GUID basetype;
		ChipDesc::Usage usage;
		Version version;
		StringA factoryFunc;
		String filters; // For importers only. File types supported by Importer-chip, eg "dae;xml;3ds"
	} chipDesc; // NOTE: Can not use ChipDesc here because of wchar_t* and char*.
	CHIPFACTORY chipFactoryFunc;
	ChipTypeIndex chipTypeIndex;

	List<GUID> baseguids;

	ChipInfo() : packet(nullptr), chipFactoryFunc(0), chipTypeIndex(0) {}
};

typedef Map<GUID, ChipInfo*> ChipInfoPtrByGUIDMap;
template class M3DENGINE_API Map<GUID, ChipInfo*>;

typedef Map<String, Packet*> PacketPtrByStringMap;
template class M3DENGINE_API Map<String, Packet*>;

typedef Map<ChipTypeIndex, ChipInfo*> ChipInfoPtrByGUIDIndexMap;
template class M3DENGINE_API Map<ChipTypeIndex, ChipInfo*>;

typedef Map<std::pair<GUID, String>, Chip*> GlobalChipMap;
template class M3DENGINE_API Map<std::pair<GUID, String>, Chip*>;


struct Packet
{
	Path filename;
	String name;
	unsigned chipCount;
	// Note: The supportedPlatforms-flag is mostly for use by the publisher! 
	// We don't do any testing on load! If the dll is present and loads for a given platform it is assumed to be supported!
	unsigned supportedPlatforms; 
	std::atomic<HMODULE> module; // The dll-module. Atomic because we load it on demand, possibly from any thread.
	ChipInfoPtrByGUIDMap chips; 
	ADDDEPENDENCIES addDependenciesFunc;

	Packet() : chipCount(0), supportedPlatforms(0), module(0), addDependenciesFunc(nullptr) {}
};



class M3DENGINE_API ChipManagerEventListener
{
public:
	virtual void ChipManagerInitiated() = 0;
	virtual void PacketLoaded(String packetName) = 0;
};

class M3DENGINE_API ChipManager
{
protected:
	ChipTypeIndex _guidIndices;
	ChipInfoPtrByGUIDMap _chipsByGUIDMap;
	ChipInfoPtrByGUIDIndexMap _chipsByGUIDIndexMap;
	PacketPtrByStringMap _packetMap;
	bool *_guidSupportMap;

	GlobalChipMap _globalChips;

	ChipManagerEventListener *_eventListener;

	Path _chipsDirectory;

	SlimRWLock _lock;
	SlimRWLock _globalChipsLock;

	void _clear();
	void _processDLL(Path fileName);

	bool &_isChipGuidSupported(ChipTypeIndex guidRequired, ChipTypeIndex guidToCheck) const { return _guidSupportMap[guidRequired * _guidIndices + guidToCheck]; }

public:
	ChipManager();
	virtual ~ChipManager();

	void SetEventListener(ChipManagerEventListener *eventListener) { _eventListener = eventListener; }

	bool FindChips(Path folder);

	Path GetChipsDirectory() const { return _chipsDirectory; }

	// THREAD SAFE!
	Chip *CreateChip(const GUID &chipType);
	// THREAD SAFE!
	Chip *CreateChip(ChipTypeIndex chipTypeIndex);

	const PacketPtrByStringMap &GetPacketMap() const { return _packetMap; }
	const ChipInfoPtrByGUIDMap &GetChipInfoPtrByGUIDMap() const { return _chipsByGUIDMap; }

	ChipTypeIndex GetChipTypeIndex(const GUID &chipType) const;
	const ChipInfo *GetChipInfo(ChipTypeIndex chipTypeIndex) const;
	const ChipInfo *GetChipInfo(const GUID &chipType) const { return GetChipInfo(GetChipTypeIndex(chipType)); }

	bool IsChipTypeSupported(const GUID &typeRequired, const GUID &typeToCheck) const;
	bool IsChipTypeSupported(ChipTypeIndex typeRequired, ChipTypeIndex typeToCheck) const;

	Chip *GetGlobalChip(const GUID &type, String name = L"", bool create = true);
	bool ReleaseGlobalChip(const GUID &type, String name = L"");
	void ClearGlobalChips();

	void DestroyDeviceObjects();

	void OnNewFrame();

	List<GUID> GetDerivedChips(const GUID &baseType) const;
};


}

