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
#include "M3DEngine/GlobalDef.h"


class QWidget;

namespace m3d
{

class ChipDialog;
class Chip;
class ChipDialogManager;

typedef void (*RELEASEMEMORY)(void *mem);

struct DialogType
{
	GUID chipType; // The chip type this dialog page represent.
	GUID chipTypeNP; // Chip type for next page. Can be equal to chipType. Set to NullGUID to skip all other pages!
	const char *factoryFunc; // Factory function for the dialog page. Set to nullptr for no page. 
};

// Each dialog packet implements this function in its exports.cpp file.
extern const DialogType &RegisterDialogDesc(GUID chipType, GUID chipTypeNP, const char *factoryFunc);

// DIALOGDESC_DECL is placed in all new dialogs.
#define DIALOGDESC_DECL public: static const m3d::DialogType DESC; virtual const m3d::DialogType &GetDialogDesc() const override { return DESC; } 
// DIALOGDESC_DEF or DIALOGDESC_DEF_ADV is placed in the cpp file of the dialog, or in the exports.cpp of the packet.
// TODO: If placed in cpp file of dialog, confirm that RegisterDialogDesc(...) is always called on library loading, and that this technique is safe!
#define DIALOGDESC_DEF(clazz, chipType) const m3d::DialogType clazz::DESC = m3d::RegisterDialogDesc(chipType, chipType, #clazz"_FACTORY"); extern "C" __declspec( dllexport ) m3d::DialogPage* __cdecl clazz##_FACTORY() throw(...) { return new clazz(); }
#define DIALOGDESC_DEF_ADV(clazz, chipType, chipTypeNP) const m3d::DialogType clazz::DESC = m3d::RegisterDialogDesc(chipType, chipTypeNP, #clazz"_FACTORY"); extern "C" __declspec( dllexport ) m3d::DialogPage* __cdecl clazz##_FACTORY() throw(...) { return new clazz(); }


class CHIPDIALOGS_EXPORT DialogPage
{
private:
	ChipDialog *_chipDialog;
	ChipTypeIndex _chipType;

	RELEASEMEMORY _dealloc;
	bool _edited;
public:
	DialogPage();
	virtual ~DialogPage();

	// Overridden by all pages.
	virtual const DialogType &GetDialogDesc() const = 0;
	// Interal!
	void SetChipDialog(ChipDialog *cd) { _chipDialog = cd; }
	// Returns the containing chip dialog.
	ChipDialog *GetChipDialog() const { return _chipDialog; }
	// Interal!
	void SetChipTypeIndex(ChipTypeIndex chipType) { _chipType = chipType; }
	// Returns the chip type this page represent.
	ChipTypeIndex GetChipTypeIndex() const { return _chipType; }
	// Returns our chip.
	Chip *GetChip() const;
	// Returns the dialog manager.
	ChipDialogManager *GetDialogManager() const;
	// Returns the qt widget assosiated with this page.
	virtual QWidget *GetWidget() = 0; // It is up to inherited class to make sure it is freed when the returned QWidget is freed!
	// Called by the dialog just before the dialog is shown.
	virtual void Init() {}
	// Called once each frame.
	virtual void Update() {}
	// Called regulary at a lower frequency (eg 0.5 sec) at the page currently active. Gives user a chance to update the UI at a reasonable frequency.
	virtual void Refresh() {}
	// Called just before the page is shown. Eighter when dialog is opened or user select this page in the dialog.
	virtual void OnEnter() { Refresh(); }
	// Called when the page is hidden. Eigther because the dialog is closed or because the user selects another page.
	virtual void OnLeave() {}
	// Called when the user presses ok. By default it is also called when the user presses Apply. NOTE: The function is called in order from base chip first!
	virtual void OnOK() {}
	// Called when the user pressed cancel. NOTE: The function is called in order from base chip first!
	virtual void OnCancel() {}
	// Called when the user presses apply. NOTE: The function is called in order from base chip first!
	virtual void OnApply() { return OnOK(); }
	// Called after OnApply() has been called on all pages. NOTE: The function is called in order from base chip first!
	virtual void AfterApply() {}
	// Called when direct3D device is destroyed.
	virtual void DestroyDeviceObject() {}
	// Returns true if this page should be maximized when opened embedded!
	virtual bool IsEmbedMaximized() const { return false; }
	// Call this function to mark the class dirty when a change is made! Leave nullptr do mark our class dirty.
	void SetDirty(Class *clazz = nullptr);
	// Call this method to mark the dialog page edited when there are no changes to the chip itself!
	void SetEdited() { _edited = true; }
	// Returns true if SetDirty()/SetEdited() has been called.
	bool IsEdited() const { return _edited; }
	// Clears the edit flag. Cleared on apply.
	void ResetEditFlag() { _edited = false; }
	// Internal!
	void SetDealloc(RELEASEMEMORY dealloc) { _dealloc = dealloc; }
	// Internal!
	RELEASEMEMORY GetDealloc() const { return _dealloc; }

};


}