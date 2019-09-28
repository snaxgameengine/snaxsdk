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
#include "SimpleDialogPage.h"

namespace m3d
{


class CHIPDIALOGS_EXPORT SimpleComboBoxDialogPage : public SimpleDialogPage
{
public:
	SimpleComboBoxDialogPage(bool instantUpdateMode = true);
	~SimpleComboBoxDialogPage();

	void AddItem(String text, RData data);
	void SetInit(RData data, RData defaultData);
	RData GetCurrent();

	void sort(RSortOrder order = RSortOrder::AscendingOrder);

	void OnOK() override;
	void OnCancel() override;
	void AfterApply() override;

protected:
	virtual bool onSelectionChanged(RData data) { return false; }
	void SetSelectionChangedCallback(std::function<bool(RData)> cb) { _onSelectedChangedCallback = cb; }

private:
	void _onSelectionChanged();

	RPtr _comboBox;
	std::function<bool(RData)> _onSelectedChangedCallback;

	RData _initData;
	RData _defaultData;

	bool _instantUpdateMode;
	bool _isInit;
};


}