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
#include "ChildPtr.h"
#include "FunctionStack.h"
#include "M3DCore/DestructionObserver.h"
#include "M3DCore/GUIDUtil.h"
#include "M3DCore/Path.h"



namespace m3d
{

static const GUID CHIP_GUID = { 0x11111111, 0x1111, 0x1111, { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 } };


class M3DENGINE_API Chip : public DestructionObservable
{
	template<typename T> friend void mmdelete(const T*);
public:
	// These are some common messages.
	CHIPMSG(NoInstanceException, WARN, L"No instance set!")
	CHIPMSGV(WrongInstanceException, FATAL, String(L"The provided instance of class \'%1\' is incompatible with the excepted type \'%2\'!").arg(given).arg(expected), String given, String expected)
	CHIPMSG(InfiniteLoopException, FATAL, L"Infinite loop detected. Execution truncated. If this is wrong, please increase the reference count limit.")
	CHIPMSG(StackOverflowException, FATAL, L"Stack overflow!")
	CHIPMSG(UninitializedException, WARN, L"This Chip is not initialized correctly!")
	CHIPMSGV(MissingChildException, WARN, String(L"A required child connection (%1) is empty!").arg(String::fromNum(index)), unsigned index)
	CHIPMSGV(NotImplementedException, WARN, String(L"This functionality (%1) is not implemented!").arg(name), String name)
	CHIPMSG(UnsupportedOperationException, WARN, L"This operation is not supported!")
	CHIPMSGV(ClassNotFoundException, WARN, String(L"Class (%1) not found!").arg(filename.IsFile() ? filename.GetName() : GuidToString(id)), GUID id, Path filename)
	CHIPMSGV(InstanceNotFoundException, WARN, String(L"Instance (%1) not found! Last seen in file \'%2\'.").arg(GuidToString(id)).arg(ownerFilename.GetName()), GUID id, Path ownerFilename)
	CHIPMSG(GetChildNotAllowedException, FATAL, L"A Chip is not allowed to ask for its children from operations performed in the dialog pages.")
	CHIPMSG(DeprecatedFeatureException, WARN, L"This feature is deprecated and should not be used!")
	CHIPMSG(ReinitPreventedException, WARN, L"This Chip is not initialized correctly, and initialization was prevented by refresh mode.")

		
private:
	// _id is unique for this chip instance in the current process.
	const ChipID _id;
	// _globalID is globally unique for this chip instance. It is used to identify the chip through save/load.
	GUID _globalID;
	// The class this chip belongs to.
	// The class does not neccesarily contain the chip. It may be stored in a chip that is located in the class.
	// _clazz should always be set, except for special cases like global singletons.
	Class *_clazz;
	// The owner of this chip. Normally it will be itself, but if it is contained in another chip, it will be the containing chip. Never nullptr!
	Chip *_owner;
	// Meta data about the chip. Used by the editor.
	ChipEditorData *_editorData;
	// Chip name
	String _name;
	// The ChipTypeIndex corresponding to the type given by GetChipType().
	mutable ChipTypeIndex _typeIndex;
	// Connected children. NOTE: If _childProvider != this, _children
	// will not be used when getting/setting children, because the ChildConnectionList
	// will be provided by another chip. It is not allowed to tweak (remove/add)
	// connections given by the provider, so such changes will still be performed on the
	// local list.
	ChildConnectionList _children;
	// Provider of the ChildConnectionList to be used when setting/getting children.
	// It's const because we're not allowed to tweak the connections.
	// It defaults to "this", but when the chip is used in eg a ClassInstance, it is
	// another chip (the InstanceData) that will provide the children we should use. 
	const Chip *_childProvider;
	// Timestamp of the last time our GetChip() was called. It is used to provide
	// visual feedback of chip activity in the editor.
	long _lastHit;
	// The function this chip is/has if any.
	Function *_function;
	// A unique value that a derived chip can set when it is updated. This way, it's easy to see when a chip is updated.
	UpdateStamp _updateStamp;
	// List of chips (ie ShellChip) supposed to have exact the same connections as us. Any connection-modifications are relied to these.
	ChipList _connectionClones;
	// Chip Messages. NOTE: Will probably contain only one or two (or very few!) messages. Preferring list over map for searching! (mutable to be able to add messages from const func.)
	ChipMessageList *_messages;
	
	void _removeChild(Chip *child);
	bool _setChild(Chip *child, unsigned index, unsigned subIndex, bool insert = false);
	void _clearConnections(unsigned fromIndex = 0);
	void _clearConnection(unsigned index);
	void _setConnection(unsigned index, const ChildConnectionDesc &connection, bool keepChildren);
	void _clearMessages();

protected:
	RefreshManager Refresh;

	// Resets the _typeIndex.
	void ResetTypeIndex() { _typeIndex = InvalidChipTypeIndex; }



	// Function called by GetChildren() to get the ChildConnectionList to use.
	// Normally this is our _children, but chips like the TemplateChip
	// overrides it to get the list from its template.
	virtual const ChildConnectionList &ProvideChildren() const { return _children; }

	virtual ~Chip();
	virtual void OnRelease() {}

public:
	Chip();

	// Call release when done with the chip.
	void Release() const;

	// To be overridden by all new chips!
//	virtual const ChipDesc &GetChipDesc() const { return CHIP_DESC; } 

	// These are defined for all chips using the CHIPDESC_DECL macro.
	static const ChipDesc DESC;
	virtual const ChipDesc &GetChipDesc() const { return DESC; }

	// To be overridden by special chips like the ProxyChip. 
	// Remember to call ResetTypeIndex() when changing this.
	// No not change more than once!
	virtual const GUID &GetChipType() const { return GetChipDesc().type; } 

	// Gets the ChipTypeIndex (type of fast runtime guid) for this chip.
	virtual ChipTypeIndex GetChipTypeIndex() const; 

	// Returns the runtime id of this chip.
	ChipID GetID() const { return _id; }

	// Returns the global id of this chip. It is unchanged through save/load.
	const GUID &GetGlobalID() const { return _globalID; }

	// Only to be used by loading functionality!
	void SetGlobalID(const GUID &globalID) { _globalID = globalID; }

	// Get/Set the class this chip belongs to.
	Class *GetClass() const { return _clazz; }
	virtual void SetClass(Class* clazz); // This can be overridden in classes that hold instances.

	// Get/Set the owner of this chip.
	Chip *GetOwner() const { return _owner; }
	void SetOwner(Chip *owner) { _owner = owner; }

	// Editor data used by the editor.
	ChipEditorData *GetChipEditorData() const { return _editorData; }
	void SetChipEditorData(ChipEditorData *editorData) { _editorData = editorData; }

	// Function methods.
	virtual bool CanCreateFunction() const;
	Function *CreateFunction();
	Function *GetFunction() const { return _function; }
	void RemoveFunction();

	// Fast casting functions for selected types.
	virtual ProxyChip *AsProxyChip() { return nullptr; }
	virtual Parameter *AsParameter() { return nullptr; }
	virtual FunctionCall *AsFunctionCall() { return nullptr; }
	virtual FunctionData *AsFunctionData() { return nullptr; }
	virtual InstanceData *AsInstanceData() { return nullptr; }
	virtual Shortcut *AsShortcut() { return nullptr; }

	// Get/Set chip name.
	virtual String GetName() const { return _name; }
	virtual void SetName(String name);

	// After a chip is created, ONE of InitChip(), CopyChip() OR LoadChip() 
	// should be called to finalize initialization of the chip!
	// Therefore, move heavy initialization code from the constructor to these methods!
	virtual bool InitChip();
	virtual bool CopyChip(Chip *chip);
	virtual bool LoadChip(DocumentLoader &loader) { return true; }
	virtual bool SaveChip(DocumentSaver &saver) const { return true; }

	// Message system.
	virtual bool HasMessages() const { return _messages != nullptr; }
	virtual const ChipMessageList *GetMessages() const { return _messages; }
	virtual unsigned GetMessageHitCount(const ChipMessage &msg) const;
	virtual void AddMessage(const ChipMessage &msg);
	virtual void RemoveMessage(const ChipMessage &msg);
	virtual void ClearMessages();
	// This will add the message to the chip given in the exception. If none, it will add it to us!
	virtual void AddException(const ChipException &exp);

	// Get/Set the update-stamp.
	UpdateStamp GetUpdateStamp() const { return _updateStamp; }
	UpdateStamp SetUpdateStamp() { return _updateStamp = GenerateUpdateStamp(); }

	virtual void OnDestroyDevice() {}
	virtual void OnReleasingBackBuffer(RenderWindow *rw) {}


	virtual const ChildConnectionList &GetChildren() const { return _childProvider->ProvideChildren(); }
	inline const Chip *GetChildProvider() const { return _childProvider; }
	inline void SetChildProvider(const Chip *chip) { _childProvider = chip; }
	inline const Chip *ReplaceChildProvider(const Chip *chip) { const Chip *c = _childProvider; _childProvider = chip; return c; }

	// Removes all child connections in our _children list.
	// It does NOT use the ChildConnectionList returned by our _childProvider!
	// The fromIndex parameters lets you clear exessive connections.
	virtual void ClearConnections(unsigned fromIndex = 0);

	// Sets a child connection in our _children list.
	// It does NOT use the ChildConnectionList returned by our _childProvider!
	virtual void SetConnection(unsigned index, const ChildConnectionDesc &connection, bool keepChildren = false);

	virtual bool SetChild(Chip *child, unsigned index, unsigned subIndex);
	virtual bool InsertChild(Chip *child, unsigned index, unsigned subIndex);
	virtual bool MoveChild(unsigned index, unsigned fromSubIndex, unsigned toSubIndex);
	virtual void RemoveChild(Chip *child);
	virtual ChipChildPtr GetChild(unsigned index, unsigned subIndex = 0) const;
	virtual Chip *GetRawChild(unsigned index, unsigned subIndex = 0) const;
	virtual unsigned GetConnectionCount() const;
	virtual unsigned GetSubConnectionCount(unsigned index) const;
	virtual void RemoveEmptyConnections();

	void AddConnectionClone(Chip *chip);
	void RemoveConnectionClone(Chip *chip);

	virtual String GetValueAsString() const { return String(); }

	virtual void CallChip() {}
	virtual ChipChildPtr GetChip();

	virtual void Run();

	virtual Chip *FindChip(ChipID chipID) { return _id == chipID ? this : nullptr; }

	// Updates _lastHit.
	void Touch();

	// Returns the Refresh Manager used to limit chip updates.
	virtual RefreshManager &GetRefreshManager() { return Refresh; }
	virtual const RefreshManager &GetRefreshManager() const { return Refresh; }

	virtual long GetLastHitTime() const { return _lastHit; }

	virtual void RestoreChip() {}

	virtual void AddDependencies(ProjectDependencies &deps) {}

	// Hidden chips (aka global chips) will be called at the start of each frame. 
	// This is a single event that later may be part of a more advance event system.
	virtual void OnNewFrame() {}

	virtual List<Shortcut*> GetShortcuts() const;

	// Returns the platforms that are supported by this chip.
	// This function is to be used by the publishing process.
	// The publishing will fail or give a warning (and excluding the chip) if the
	// chip is not supported by the target platform.
	// It is implemented as a virtual function instead of being defined in the ChipDesc
	// for the chip to be able to select supported platform based on its configuration.
	// This also means that if the chip is a container or the creator of global chips,
	// is must call GetSupportedPlatforms() on them as well and merge the result!
	// Note: There is a similar function defined at packet level.
	virtual unsigned GetSupportedPlatforms() const { return PLATFORM_all_platforms; }
};

#ifdef DEVCHECKS
#define CHECK_CHILD(c, idx) if (c == nullptr) throw MissingChildException(idx);
#else
#define CHECK_CHILD(c, idx)
#endif

}