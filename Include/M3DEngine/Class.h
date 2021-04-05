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
#include "M3DCore/Set.h"
#include "M3DCore/Path.h"



namespace m3d
{

static const Version DocumentVersion = Version(1, 2, 5, 0); // Fixed bug where tag 'Description' was written as 'Publish'
//static const Version DocumentVersion = Version(1, 2, 4, 0); // Added multiconnections
//static const Version DocumentVersion = Version(1, 2, 3, 0); // Added class description
//static const Version DocumentVersion = Version(1,2,2,0); // Standardized 'Class' names
//static const Version DocumentVersion = Version(1,2,1,0); // Added autoload for some chips
//static const Version DocumentVersion = Version(1,2,0,0); // Added <PublishProfiles>
//static const Version DocumentVersion = Version(1,1,0,1);


class ClassInstance;
class RenderWindow;
class ClassChip;


typedef Map<ChipID, Chip*> ChipPtrByChipIDMap;
template class M3DENGINE_API Map<ChipID, Chip*>;

typedef Set<Parameter*> ParameterPtrSet;
template class M3DENGINE_API Set<Parameter*>;

typedef Set<Function*> FunctionPtrSet;
template class M3DENGINE_API Set<Function*>;

typedef Set<InstanceData*> InstanceDataPtrSet;
template class M3DENGINE_API Set<InstanceData*>;

typedef Map<FunctionSignatureID, Function*> FunctionPtrByFunctionSignatureIDMap;
template class M3DENGINE_API Map<FunctionSignatureID, Function*>;

typedef Set<Class*> ClassPtrSet;
template class M3DENGINE_API Set<Class*>;

typedef Set<ClassInstance*> ClassInstancePtrSet;
template class M3DENGINE_API Set<ClassInstance*>;

typedef Set<ClassChip*> ClassChipPtrSet;
template class M3DENGINE_API Set<ClassChip*>;



class M3DENGINE_API ClassFactory
{
public:
	virtual Class *Create(); // Create using mmnew!
};

extern ClassFactory M3DENGINE_API DefaultClassFactory;



class M3DENGINE_API ClassEventListener
{
public:
	virtual void OnFunctionCreate(Function *function) = 0;
	virtual void OnFunctionRemove(Function *function) = 0;
	virtual void OnFunctionChange(Function *function) = 0;
	virtual void OnInstanceRegistered(ClassInstance *instance) = 0;
	virtual void OnInstanceUnregistered(ClassInstance *instance) = 0;
	virtual void OnInstanceNameChanged(ClassInstance *instance) = 0;
	virtual void OnBaseClassAdded(Class *derived, Class *base) = 0;
	virtual void OnBaseClassRemoved(Class *derived, Class *base) = 0;
};

typedef Map<GUID, Path> GUIDByPathMap;
template class M3DENGINE_API Map<GUID, Path>;

typedef Map<GUID, ClassInstance*> ClassInstancePtrByGUIDMap;
template class M3DENGINE_API Map<GUID, ClassInstance*>;

struct M3DENGINE_API ClassLoadInfo
{
	// During loading, base classes are added to this map. When the clazz is added to the manager, the map is iterated
	// and the base classes are (loaded and) added to the class. Loading is a two stage process!
	GUIDByPathMap loadedBaseClasses;
	// These are instances found in a loaded class. They may be instances of other classes.
	ClassInstancePtrByGUIDMap instances;
};


class M3DENGINE_API Class
{
public:
	Class();
	virtual ~Class();
	// Erases all content in the class. Important: Call before destructor!
	virtual void Clear();
	// Returns the global id for this class.
	const GUID &GetGuid() const { return _guid; }
	// Sets the global id for this class.
	void SetGuid(const GUID &guid) { _guid = guid; }
	// The runtime id for this class.
	ClassID GetID() const { return _clazzid; }
	// Returns the name of this class.
	String GetName() const { return _name; }
	// Sets the name of this class. 
	void SetName(String name);
	// Returns the document this clazz belongs to.
	Document *GetDocument() const { return _doc; }
	// Sets the document this clazz belongs to.
	void SetDocument(Document *doc) { _doc = doc; }
	// Returns a map of all chips ordered by id.
	const ChipPtrByChipIDMap &GetChips() const { return _chips; }
	// Returns all parameters.
	const ParameterPtrSet &GetParameters() const { return _parameters; }
	// Returns all instance data.
	const InstanceDataPtrSet &GetInstanceData() const { return _instanceData; }
	// Adds a new chip to the class. initChip is true if Chip::InitChip() is to be called.
	virtual Chip *AddChip(ChipTypeIndex chipGuid, bool initChip = true);
	// Removes the given chip from the class. The chip is released (deleted).
	virtual bool RemoveChip(Chip *ch);
	// Gets a chip by id.
	Chip *GetChip(ChipID id);

	ClassEventListener *GetEventListener() const { return _eventListener; }
	void SetEventListener(ClassEventListener *eventListener) { _eventListener = eventListener; }


	virtual bool IsDirty() const { return false; }
	virtual void SetDirty(bool dirty = true) {}

	void OnFunctionCreate(Function *function);
	void OnFunctionRemove(Function *function);
	void OnFunctionChange(Function *function, FunctionSignatureID oldSignature);
	Function *GetFunction(const String &name, ChipTypeIndex typeRequired = InvalidChipTypeIndex);
	const FunctionPtrSet &GetFunctions() const { return _functions; }
	const FunctionPtrByFunctionSignatureIDMap &GetVirtualFunctions() const { return _vFunctions; }

	const ClassPtrSet &GetBaseClasses() const { return _baseClasses; }
	const ClassPtrSet &GetSubClasses() const { return _subClasses; }

	virtual bool AddBaseClass(Class *base);
	virtual bool RemoveBaseClass(Class *base);
	bool IsBaseClass(const Class *base) const;
	unsigned IsBaseClassN(const Class *base) const;
	bool IsDirectBaseClass(const Class *base) const;
	unsigned GetBaseCount() const;

	void OnParameterTypeSet(Parameter *parameter);
	void OnInstanceDataTypeSet(InstanceData *data);

	unsigned GetNewFunctionDataID() { return _functionDataIDs++; }

	// Searches for a chip.
	Chip *FindChip(ChipID chipID) const;

	// Called by clazz-manager when we are added.
	virtual void OnAddedToClassManager();
	// Called by clazz-manager when we are removed.
	virtual void OnRemovedFromClassManager();
	// Returns a set with all instances of this clazz. These are instances of us!
	const ClassInstancePtrSet &GetInstances() const { return _instances; }
	// Finds an instance using the global id.
	ClassInstance *FindInstance(const GUID &instanceID);
	// Searches all chips for dependencies when publishing.
	void AddDependencies(ProjectDependencies &packets, ProjectDependencies &third);
	// Calls RestoreChip() on all chips. 
	void RestoreChips();
	// Calls CallChip() on the start-chip.
	void Run();
	// Returns the start chip in this clazz.
	Chip *GetStartChip() const { return _chStart; }
	// Sets the start chip in this clazz.
	bool SetStartChip(Chip *chip);
	// Saves editor data. Overridden in ClassExt.
	virtual bool SaveEditorData(DocumentSaver &saver) const { return true; }
	// Loads editor data. Overridden in ClassExt.
	virtual bool LoadEditorData(DocumentLoader &loader) { return true; }
	// Called when the d3d-device is destroyed. Relayed to all chips.
	void OnDestroyDevice();
	// Called when the back buffer is released. Relayed to all chips.
	void OnReleasingBackBuffer(RenderWindow *rw);

	const ClassChipPtrSet &GetClassChips() const { return _clazzChips; }
	void RegisterClassChip(ClassChip *ch) { _clazzChips.insert(ch); }
	void UnregisterClassChip(ClassChip *ch) { _clazzChips.erase(ch); }
	// Called by our Shortcuts when its type is set.
	virtual void OnShortcutSet(Shortcut *sc) {}

	// Should only be called by the instance itself.
	bool RegisterInstance(ClassInstance *instance);
	// Should only be called by the instance itself.
	void UnregisterInstance(ClassInstance *instance);
	// Should only be called by the instance itself.
	void OnInstanceNameChanged(ClassInstance *instance);

	ClassLoadInfo *GetLoadInfo(bool create = true);
	void DeleteLoadInfo();

protected:
	void RemoveChipAsChildOrParameter(Chip *ch);
	virtual ClassLoadInfo *CreateLoadInfo() const;
	virtual void DeleteLoadInfo(ClassLoadInfo *nfo) const;

private:
	// Listener for class events.
	ClassEventListener *_eventListener;
	// This is the global id of this class used to identify it during save/load.
	GUID _guid;
	// This is the runtime id.
	const ClassID _clazzid;
	// The name of the class.
	String _name;
	// The document this class belongs to. In general, all classes should belong to a document!
	Document *_doc;
	// All chips in the clazz ordered by ChipID.
	ChipPtrByChipIDMap _chips;
	// This is the start chip for this clazz.
	Chip *_chStart;
	// All parameters in the clazz.
	ParameterPtrSet _parameters;
	// All instance data in the clazz.
	InstanceDataPtrSet _instanceData;
	// All functions is the clazz.
	FunctionPtrSet _functions;
	// All virtual functions ordered by the signature.
	FunctionPtrByFunctionSignatureIDMap _vFunctions;
	// This set contains ALL base classes (also indirect) for this class.
	ClassPtrSet _allBaseClasses;
	// This set contains all direct base classes.
	ClassPtrSet _baseClasses;
	// This set contains all direct sub classes.
	ClassPtrSet _subClasses;
	// This contains all instances of us.
	ClassInstancePtrSet _instances;
	// This contains all instances of us ordered by their global id.
	ClassInstancePtrByGUIDMap _instancesByGlobalID;
	// A set of ClassChips pointing to us.
	ClassChipPtrSet _clazzChips;
	// Each function data in a clazz get its own id>=0. This is used for fast lookup in FunctionStack.
	unsigned _functionDataIDs;
	// Some data aquired during loading. These are used when the clazz is added to manager. Then deleted!
	ClassLoadInfo *_loadInfo;

	void _updateVirtualFunction(FunctionSignatureID signature);
	void _onBaseClassAdded(Class *base, Class *directBase);
	void _onBaseClassRemoved(Class *base, Class *directBase);
	void _findInstanceData(Class *base, Set<InstanceData*> &instanceData, unsigned nBaseOccurences) const;
	
	unsigned _findBaseCount(Class *base) const;

	void _onInstanceDataAdded(InstanceData *data);
	void _onInstanceDataRemoved(InstanceData *data);

	virtual void _onInstanceRegistered(ClassInstance *instance) {}
	virtual void _onInstanceUnregistered(ClassInstance *instance) {}

};


}


