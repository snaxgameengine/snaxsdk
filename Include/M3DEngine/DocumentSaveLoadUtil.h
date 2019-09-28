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

#include "DocumentSaver.h"
#include "DocumentLoader.h"
#include "M3DCore/MMath.h"
#include "M3DCore/Flags.h"


namespace m3d
{


template<typename T>
bool SerializeDocumentData(DocumentSaver &saver, const T *t, unsigned size) 
{
	for (unsigned i = 0; i < size; i++) {
		SAVE(StringA::format("d%i", i), t[i]);
	}
	return true;
}

template<typename T>
bool DeserializeDocumentData(DocumentLoader &loader, T *t, unsigned size) 
{
	for (unsigned i = 0; i < size; i++) {
		LOAD(StringA::format("d%i", i), t[i]);
	}
	return true;
}

template<typename T, typename S>
bool SerializeDocumentData(DocumentSaver &saver, const std::pair<T, S> &data)
{
	SAVE("first", data.first);
	SAVE("second", data.second);
	return true;
}

template<typename T, typename S>
bool DeserializeDocumentData(DocumentLoader &loader, std::pair<T, S> &data)
{
	LOAD("first", data.first);
	LOAD("second", data.second);
	return true;
}

template<typename T>
bool SerializeDocumentData(DocumentSaver &saver, const List<T> &data)
{
	SAVE("count", (unsigned)data.size());
	if (data.size())
		SAVEARRAY("array", &data.front(), (unsigned)data.size());
	return true;
}

template<typename T>
bool DeserializeDocumentData(DocumentLoader &loader, List<T> &data)
{
	unsigned size = 0;
	LOAD("count", size);
	data.resize(size);
	if (size)
		LOADARRAY("array", &data.front(), size);
	return true;
}


template<typename T, bool multiset, typename CMP>
bool SerializeDocumentData(DocumentSaver &saver, const Set<T, multiset, CMP> &data)
{
	SAVE("count", (unsigned)data.size());
	unsigned i = 0;
	for (Set<T, multiset, CMP>::cNode n = data.front(); n.valid(); n++) {
		SAVE(StringA::format("d%i", i), n.get());
		i++;
	}
	return true;
}

template<typename T, bool multiset, typename CMP>
bool DeserializeDocumentData(DocumentLoader &loader, Set<T, multiset, CMP> &data)
{
	data.clear();
	T p;
	unsigned count = 0;
	LOAD("count", count);
	for (unsigned i = 0; i < count; i++) {
		LOAD(StringA::format("d%i", i), p);
		B_RETURN(data.insert(p).second);
	}
	return true;
}

template<typename T, typename S, bool multimap, typename CMP>
bool SerializeDocumentData(DocumentSaver &saver, const Map<T, S, multimap, CMP> &data)
{
	SAVE("count", (unsigned)data.size());
	unsigned i = 0;
	for (Map<T, S, multimap, CMP>::cNode n = data.front(); n.valid(); n++) {
		SAVE(StringA::format("d%i", i), n.get());
		i++;
	}
	return true;
}

template<typename T, typename S, bool multimap, typename CMP>
bool DeserializeDocumentData(DocumentLoader &loader, Map<T, S, multimap, CMP> &data)
{
	data.clear();
	std::pair<T, S> p;
	unsigned count = 0;
	LOAD("count", count);
	for (unsigned i = 0; i < count; i++) {
		LOAD(StringA::format("d%i", i), p);
		B_RETURN(data.insert(p).second);
	}
	return true;
}


// Enum
template<class T, class = typename std::enable_if< std::is_enum<T>::value >::type>
bool SerializeDocumentData(DocumentSaver &saver, T t) {
	return saver.WriteData((unsigned)t);
}

// Enum
template<class T, class = typename std::enable_if< std::is_enum<T>::value >::type>
bool DeserializeDocumentData(DocumentLoader &loader, T &t) {
	return loader.ReadData((unsigned&)t);
}

static bool SerializeDocumentData(DocumentSaver &saver, Flags f) 
{
	if (saver.IsBinary())
		return saver.WriteData(f.flags);
	return saver.WriteData(String::format(L"0x%X", f.flags));
}

static bool DeserializeDocumentData(DocumentLoader &loader, Flags &f) 
{
	if (loader.IsBinary())
		return loader.ReadData(f.flags);
	String tmp;
	if (!loader.ReadData(tmp))
		return false;
	return tmp.toNum(f.flags, String::HEX);
}



static bool SerializeDocumentData(DocumentSaver &saver, const XMFLOAT2 &data) { return saver.WriteData((const float*)&data, 2); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMFLOAT2 &data) { return loader.ReadData((float*)&data, 2); }
static bool SerializeDocumentData(DocumentSaver &saver, const XMFLOAT2 *data, unsigned size) { return saver.WriteData((const float*)data, 2 * size); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMFLOAT2 *data, unsigned size) { return loader.ReadData((float*)data, 2 * size); }

static bool SerializeDocumentData(DocumentSaver &saver, const XMFLOAT3 &data) { return saver.WriteData((const float*)&data, 3); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMFLOAT3 &data) { return loader.ReadData((float*)&data, 3); }
static bool SerializeDocumentData(DocumentSaver &saver, const XMFLOAT3 *data, unsigned size) { return saver.WriteData((const float*)data, 3 * size); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMFLOAT3 *data, unsigned size) { return loader.ReadData((float*)data, 3 * size); }

static bool SerializeDocumentData(DocumentSaver &saver, const XMFLOAT4 &data) { return saver.WriteData((const float*)&data, 4); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMFLOAT4 &data) { return loader.ReadData((float*)&data, 4); }
static bool SerializeDocumentData(DocumentSaver &saver, const XMFLOAT4 *data, unsigned size) { return saver.WriteData((const float*)data, 4 * size); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMFLOAT4 *data, unsigned size) { return loader.ReadData((float*)data, 4 * size); }

static bool SerializeDocumentData(DocumentSaver &saver, const XMFLOAT4X4 &data) { return saver.WriteData((const float*)&data, 16); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMFLOAT4X4 &data) { return loader.ReadData((float*)&data, 16); }
static bool SerializeDocumentData(DocumentSaver &saver, const XMFLOAT4X4 *data, unsigned size) { return saver.WriteData((const float*)data, 16 * size); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMFLOAT4X4 *data, unsigned size) { return loader.ReadData((float*)data, 16 * size); }

static bool SerializeDocumentData(DocumentSaver &saver, const XMUSHORT4 &data) { return saver.WriteData((const unsigned short*)&data, 4); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMUSHORT4 &data) { return loader.ReadData((unsigned short*)&data, 4); }
static bool SerializeDocumentData(DocumentSaver &saver, const XMUSHORT4 *data, unsigned size) { return saver.WriteData((const unsigned short*)data, 4 * size); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMUSHORT4 *data, unsigned size) { return loader.ReadData((unsigned short*)data, 4 * size); }

static bool SerializeDocumentData(DocumentSaver &saver, const XMCOLOR &data) { return saver.WriteData((const unsigned char*)&data, 4); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMCOLOR &data) { return loader.ReadData((unsigned char*)&data, 4); }
static bool SerializeDocumentData(DocumentSaver &saver, const XMCOLOR *data, unsigned size) { return saver.WriteData((const unsigned char*)data, 4 * size); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMCOLOR *data, unsigned size) { return loader.ReadData((unsigned char*)data, 4 * size); }

static bool SerializeDocumentData(DocumentSaver &saver, const XMUBYTEN4 &data) { return saver.WriteData(data.v); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMUBYTEN4 &data) { return loader.ReadData(data.v); }
static bool SerializeDocumentData(DocumentSaver &saver, const XMUBYTEN4 *data, unsigned size) { return saver.WriteData((unsigned*)data, size); }
static bool DeserializeDocumentData(DocumentLoader &loader, XMUBYTEN4 *data, unsigned size) { return loader.ReadData((unsigned*)data, size); }

bool M3DENGINE_API SerializeDocumentData(DocumentSaver &saver, const GUID &data); 
bool M3DENGINE_API DeserializeDocumentData(DocumentLoader &loader, GUID &data);

bool M3DENGINE_API SerializeDocumentData(DocumentSaver &saver, const Chip *chip);
bool M3DENGINE_API DeserializeDocumentData(DocumentLoader &loader, Chip *&chip);

bool M3DENGINE_API SerializeDocumentData(DocumentSaver &saver, const DataBuffer &data);
bool M3DENGINE_API DeserializeDocumentData(DocumentLoader &loader, DataBuffer &data);

bool M3DENGINE_API SerializeDocumentData(DocumentSaver &saver, Function::Access data);
bool M3DENGINE_API DeserializeDocumentData(DocumentLoader &loader, Function::Access &data);
bool M3DENGINE_API SerializeDocumentData(DocumentSaver &saver, Function::Type data);
bool M3DENGINE_API DeserializeDocumentData(DocumentLoader &loader, Function::Type &data);
bool M3DENGINE_API SerializeDocumentData(DocumentSaver &saver, RefreshManager::RefreshMode data);
bool M3DENGINE_API DeserializeDocumentData(DocumentLoader &loader, RefreshManager::RefreshMode &data);

class AxisAlignedBox;

bool M3DENGINE_API SerializeDocumentData(DocumentSaver &saver, const AxisAlignedBox &data);
bool M3DENGINE_API DeserializeDocumentData(DocumentLoader &loader, AxisAlignedBox &data);

bool M3DENGINE_API SerializeDocumentData(DocumentSaver &saver, Version data); 
bool M3DENGINE_API DeserializeDocumentData(DocumentLoader &loader, Version &data);

bool M3DENGINE_API SerializeDocumentData(DocumentSaver &saver, Path data); 
bool M3DENGINE_API DeserializeDocumentData(DocumentLoader &loader, Path &data);


}