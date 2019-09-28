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
#include "M3DCore/MString.h"
#include "M3DCore/GuidUtil.h"
#include "M3DCore/Map.h"
#include "M3DCore/Path.h"
#include "DocumentTags.h"
#include "Function.h"
#include "Class.h"
#include "M3DCore/CommonTemplates.h"

namespace m3d
{

class DataBuffer;

template<typename T>
struct SaveDataT
{
	static bool Serialize(DocumentSaver &saver, const T &t) { return SerializeDocumentData(saver, t); }
};	

template<typename T>
struct SaveDataT<T*>
{
	static bool Serialize(DocumentSaver &saver, const T *t) 
	{
		if (t)
			return saver.PushGroup(DocumentTags::Ptr) && SerializeDocumentData(saver, t) && saver.PopGroup(DocumentTags::Ptr);
		return true;
	}
	static bool Serialize(DocumentSaver &saver, const T *t, unsigned size) { return SerializeDocumentData(saver, t, size); }
};

// Saves the data. Can be used togehter with LOAD.
#define SAVE(name, value) if (!saver.SaveData(name, value)) return false;
// Saves the data (array). Can be used together with LOADARRAY.
#define SAVEARRAY(name, value, size) if (!saver.SaveData(name, value, size)) return false;
// Saves the data if not equal to dvalue. Can be used together with LOADDEF or LOADPREDEF.
#define SAVEDEF(name, value, dvalue) if (value != dvalue && !saver.SaveData(name, value)) return false;
// Saves the data IF saving editor data only. (Can be used for saving data only needed in the editor (eg shader source code.).  Can be used together with LOADDEF or LOADPREDEF.
#define SAVEOPT(name, value) if (saver.IsSaveEditorData() && !saver.SaveData(name, value)) return false;
// Saves the data IF saving editor data AND not equal to dvalue only. Can be used together with LOADDEF or LOADPREDEF.
#define SAVEDEFOPT(name, value, dvalue) if (saver.IsSaveEditorData() && value != dvalue && !saver.SaveData(name, value)) return false;

typedef Set<ChipTypeIndex> ChipTypeIndexSet;

class M3DENGINE_API DocumentSaver
{
private:
	// true if this is a 'chip'-saver (for parallel execution) of another master saver.
	const bool _chipSaver; 
	// true if we are to save editor data.
	bool _saveEditorData;
	// true if we should use multithreading.
	bool _useMultithreading;
	// The compression level to be used.
	DocumentCompressionLevel _compression;
	// This set contains all chip types encountered.
	ChipTypeIndexSet _chipTypes;
	// true if we currently are writing an attribute. (Pr thread variable)
	bool _writeAttr;
	// the attribute we are currently writing. (Pr thread variable)
	DocumentTags::Tag _attrTag;
	// The Chip currently being loaded. (Pr thread variable)
	Chip *_currentChip;

protected:
	DocumentSaver() : _chipSaver(false), _saveEditorData(true), _useMultithreading(true), _compression(DCL_NONE), _writeAttr(false), _attrTag((DocumentTags::Tag)0), _currentChip(nullptr) {}

	DocumentSaver(DocumentSaver *parent) : _chipSaver(true), _saveEditorData(parent->_saveEditorData), _useMultithreading(false), _compression(parent->_compression), _writeAttr(false), _attrTag((DocumentTags::Tag)0), _currentChip(nullptr) {}

	bool _finalize();

	inline bool IsWriteAttribute() const { return _writeAttr; }
	inline DocumentTags::Tag GetAttributeTag() const { return _attrTag; }
	inline ChipTypeIndexSet &GetChipTypes() { return _chipTypes; }

public:
	virtual ~DocumentSaver() { }

	// true if we are to save editor data like formatting, comments, shortcuts etc. 
	// Chips can use this flag to skip saving stuff not neccessary outside the editor (eg. shader source code)
	bool IsSaveEditorData() const { return _saveEditorData; }
	void SetSaveEditorData(bool doSave) { _saveEditorData = doSave; }

	// Get/Set if we should use multithreading to speed up things (default)!
	bool IsUsingMultithreading() const { return _useMultithreading; }
	void SetUseMultithreading(bool b) { _useMultithreading = b; }

	// Get/Set if we should apply compression to the saved document. For XML-docs, compression is applied to binary data.
	void SetCompressionLevel(DocumentCompressionLevel cl) { _compression = cl; }
	DocumentCompressionLevel GetCompressionLevel() const { return _compression; }

	// true if this is a temporary saver run in a worker thread for saving one single chip only.
	bool IsChipSaver() const { return _chipSaver; }

	bool SaveClasss(Document *doc);
	bool SaveClass(Class *clazz);
	// Save the chips in the given map. See also comment for SaveChip().
	// NOTE: This function is virtual so that derived savers can create their own parallel implementation!
	virtual bool SaveChips(const ChipPtrByChipIDMap&chips);
	// Saves the given chip. Does ONLY try to save the children IF chips is not null. Saves a child IF included in the chips-map.
	bool SaveChip(Chip *chip, const ChipPtrByChipIDMap*chips);

	bool AddInstance(const GUID &instanceID);


//	Class *GetCurrentClass() const { return _currentClazz; }
	Chip *GetCurrentChip() const { return _currentChip; } 

	// This template allows for saving any type user defined data type.
	template<typename T>
	bool SaveData(StringA id, const T &data)
	{
		bool ok = true;
		ok = ok && PushGroup(DocumentTags::Data);
		ok = ok && SetAttribute(DocumentTags::id, id);
		ok = ok && WriteData(data); 
		ok = ok && PopGroup(DocumentTags::Data);
		return ok;
	}

	// This template allows for saving of array of any user defined type.
	template<typename T>
	bool SaveData(StringA id, const T *data, unsigned size)
	{
		bool ok = true;
		ok = ok && PushGroup(DocumentTags::Data);
		ok = ok && SetAttribute(DocumentTags::id, id);
		ok = ok && WriteData(data, size);
		ok = ok && PopGroup(DocumentTags::Data);
		return ok;
	}

	template<typename T>
	bool SetAttribute(DocumentTags::Tag tag, const T &data)
	{
		_attrTag = tag; 
		_writeAttr = true;
		bool ok = WriteData(data);
		_writeAttr = false;
		return ok;
	}

	template<typename T>
	bool SetAttribute(DocumentTags::Tag tag, const T *data, unsigned size)
	{
		_attrTag = tag; 
		_writeAttr = true;
		bool ok = WriteData(data, size);
		_writeAttr = false;
		return ok;
	}

	// Returns true if the saver is binary. This allows for certain optimizations when saving user defined types.
	virtual bool IsBinary() const = 0;

	virtual bool Initialize() = 0;

	virtual bool SaveToFile(Path fileName, DocumentEncryptionLevel encryptionLevel = ENCLEVEL0, GUID signature = NullGUID) = 0;
	virtual bool SaveToMemory(DataBuffer &databuffer, DocumentEncryptionLevel encryptionLevel = ENCLEVEL0, GUID signature = NullGUID) = 0;

	virtual bool PushGroup(DocumentTags::Tag group) = 0;
	virtual bool PopGroup(DocumentTags::Tag group) = 0;

	// Primitive data types supported
	virtual bool WriteData(StringA data) = 0;
	virtual bool WriteData(StringW data) = 0;
	virtual bool WriteData(bool data) = 0;
	virtual bool WriteData(float data) = 0;
	virtual bool WriteData(double data) = 0;
	virtual bool WriteData(char data) = 0;
	virtual bool WriteData(unsigned char data) = 0;
	virtual bool WriteData(short data) = 0;
	virtual bool WriteData(unsigned short data) = 0;
	virtual bool WriteData(int data) = 0;
	virtual bool WriteData(unsigned int data) = 0;
	virtual bool WriteData(long long data) = 0;
	virtual bool WriteData(unsigned long long data) = 0;
	virtual bool WriteData(const void *data, unsigned sizeInBytes) = 0;
	virtual bool WriteData(const float *data, unsigned numberOfEntries) = 0;
	virtual bool WriteData(const double *data, unsigned numberOfEntries) = 0;
	virtual bool WriteData(const char *data, unsigned numberOfEntries) = 0;
	virtual bool WriteData(const unsigned char *data, unsigned numberOfEntries) = 0;
	virtual bool WriteData(const short *data, unsigned numberOfEntries) = 0;
	virtual bool WriteData(const unsigned short *data, unsigned numberOfEntries) = 0;
	virtual bool WriteData(const int *data, unsigned numberOfEntries) = 0;
	virtual bool WriteData(const unsigned *data, unsigned numberOfEntries) = 0;

	template<typename T>
	bool WriteData(const T &t) { return SaveDataT<T>::Serialize(*this, t); }

	template<typename T>
	bool WriteData(const T *t, unsigned size) { return SaveDataT<T*>::Serialize(*this, t, size); } 
 
};


}