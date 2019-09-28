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

#include "Exports.h"
#include "ChipDialogs/DialogPage.h"
#include <assert.h>

namespace
{
	const unsigned MAX_DIALOGS = 128;
	m3d::DialogType dialogDescs[MAX_DIALOGS];
	unsigned dialogCount = 0;
}

// Defined in DialogPage.h. 
const m3d::DialogType &m3d::RegisterDialogDesc(GUID chipType, GUID chipTypeNP, const char *factoryFunc)
{
	m3d::DialogType dd = { chipType, chipTypeNP, factoryFunc };
	assert(dialogCount < MAX_DIALOGS);
	return dialogDescs[dialogCount++] = dd;
}

extern "C"
{
	__declspec(dllexport) const m3d::DialogType& __cdecl GetDialogType(unsigned index) { return dialogDescs[index]; }
	__declspec(dllexport) unsigned __cdecl GetDialogCount() { return dialogCount; }
	// Memory allocated in this dll (eg chips created with the corresponding factory functions) should be freed in this dll as well.
	__declspec(dllexport) void __cdecl ReleaseMemory(void *v) { delete v; }
	__declspec(dllexport) bool __cdecl OnDlgPacketQuery() { return true; }
	__declspec(dllexport) bool __cdecl OnDlgPacketLoad() { return true; }
	__declspec(dllexport) void __cdecl OnDlgPacketUnload() { }
}
