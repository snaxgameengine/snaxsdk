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


class CHIPDIALOGS_EXPORT SimpleFormDialogPage : public SimpleDialogPage
{
public:
	typedef List<std::pair<String, RVariant>> ComboBoxInitList;
	typedef int Id;

	SimpleFormDialogPage(bool instantUpdateMode = true);
	~SimpleFormDialogPage();

	typedef std::function<void(Id, RVariant)> Callback;

	// Setup
	virtual RPtr AddCheckBox(Id id, String label, RCheckState value, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddCheckBox(String label, RCheckState value, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddCheckBox(Id id, String label, RCheckState value, Callback cb = nullptr) { return AddCheckBox(id, label, value, RVariant(), cb); }
	virtual RPtr AddCheckBox(String label, RCheckState value, Callback cb = nullptr) { return AddCheckBox(label, value, RVariant(), cb); }

	virtual RPtr AddSpinBox(Id id, String label, int value, int minimum, int maximum, int singleStep, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddSpinBox(String label, int value, int minimum, int maximum, int singleStep, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddSpinBox(Id id, String label, int value, int minimum, int maximum, int singleStep = 1, Callback cb = nullptr) { return AddSpinBox(id, label, value, minimum, maximum, singleStep, RVariant(), cb); }
	virtual RPtr AddSpinBox(String label, int value, int minimum, int maximum, int singleStep = 1, Callback cb = nullptr) { return AddSpinBox(label, value, minimum, maximum, singleStep, RVariant(), cb); }

	virtual RPtr AddDoubleSpinBox(Id id, String label, double value, double minimum, double maximum, double singleStep, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddDoubleSpinBox(String label, double value, double minimum, double maximum, double singleStep, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddDoubleSpinBox(Id id, String label, double value, double minimum, double maximum, double singleStep = 1.0, Callback cb = nullptr) { return AddDoubleSpinBox(id, label, value, minimum, maximum, singleStep, RVariant(), cb); }
	virtual RPtr AddDoubleSpinBox(String label, double value, double minimum, double maximum, double singleStep = 1.0, Callback cb = nullptr) { return AddDoubleSpinBox(label, value, minimum, maximum, singleStep, RVariant(), cb); }

	virtual RPtr AddPushButton(Id id, String label, Callback cb = nullptr);
	virtual RPtr AddPushButton(String label, Callback cb = nullptr);

	virtual RPtr AddComboBox(Id id, String label, ComboBoxInitList elements, RVariant value, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddComboBox(String label, ComboBoxInitList elements, RVariant value, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddComboBox(Id id, String label, ComboBoxInitList elements, RVariant value, Callback cb = nullptr) { return AddComboBox(id, label, elements, value, RVariant(), cb); }
	virtual RPtr AddComboBox(String label, ComboBoxInitList elements, RVariant value, Callback cb = nullptr) { return AddComboBox(label, elements, value, RVariant(), cb); }

	virtual RPtr AddLineEdit(Id id, String label, String value, bool signalImmediately, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddLineEdit(String label, String value, bool signalImmediately, RVariant defaultValue, Callback cb = nullptr);
	virtual RPtr AddLineEdit(Id id, String label, String value, bool signalImmediately, Callback cb = nullptr) { return AddLineEdit(id, label, value, signalImmediately, RVariant(), cb); }
	virtual RPtr AddLineEdit(String label, String value, bool signalImmediately, Callback cb = nullptr) { return AddLineEdit(label, value, signalImmediately, RVariant(), cb); }

	virtual RPtr AddReadOnlyLineEdit(Id id, String label, String value);
	virtual RPtr AddReadOnlyLineEdit(String label, String value);

	virtual RPtr AddLine();

	virtual void OverloadCheckBox(Id id, RPtr w, RCheckState value, Callback cb = nullptr);
	virtual void OverloadSpinBox(Id id, RPtr w, int value, Callback cb = nullptr);
	virtual void OverloadDoubleSpinBox(Id id, RPtr w, double value, Callback cb = nullptr);
	virtual void OverloadPushButton(Id id, RPtr w, Callback cb = nullptr);
	virtual void OverloadComboBox(Id id, RPtr w, RVariant value, Callback cb = nullptr);
	virtual void OverloadLineEdit(Id id, RPtr w, String value, Callback cb = nullptr);

	virtual RCheckState CheckBoxValue(Id id) const;
	virtual int SpinBoxValue(Id id) const;
	virtual double DoubleSpinBoxValue(Id id) const;
	virtual RVariant ComboBoxValue(Id id) const;
	virtual String LineEditValue(Id id) const;

	virtual void SetCheckBoxValue(Id id, RCheckState value);
	virtual void SetSpinBoxValue(Id id, int value);
	virtual void SetDoubleSpinBoxValue(Id id, double value);
	virtual void SetComboBoxValue(Id id, RVariant value);
	virtual void SetLineEditValue(Id id, String value);

	virtual void Activate(Id id);

	virtual bool IsUpdated() const;
	virtual bool IsUpdated(Id id) const;

	RVariant GetValueFromWidget(Id id) const;
	void SetWidgetEnabled(Id id, bool enabled);

	void OnOK() override;
	void OnCancel() override;
	void AfterApply() override;

protected:
	enum ElementType { CHECKBOX, SPINBOX, DOUBLESPINBOX, PUSHBUTTON, COMBOBOX, LINEEDIT };
	struct Element
	{
		ElementType type;
		RVariant initValue;
		RVariant value;
		RPtr widget;
		bool isEnabled;
		Callback cb;
		Element() {} // For map[]...
		Element(RPtr widget, ElementType type, RVariant init, Callback cb = nullptr) : type(type), initValue(init), value(init), widget(widget), isEnabled(true), cb(cb) {}
		Element(RPtr buttonWidget, Callback cb = nullptr) : type(PUSHBUTTON), widget(buttonWidget), isEnabled(true), cb(cb) {}
	};
	Map<Id, Element> _elements;
	Map<RPtr, Id> _actives;

	RPtr _layout;

	bool _instantUpdateMode;

	// callbacks
	virtual void CheckBoxUpdated(Id id, RCheckState value) { _doCallback(id, value); }
	virtual void SpinBoxUpdated(Id id, int value) { _doCallback(id, value); }
	virtual void DoubleSpinBoxUpdated(Id id, double value) { _doCallback(id, value); }
	virtual void PushButtonPressed(Id id) { _doCallback(id, RVariant()); }
	virtual void ComboBoxUpdated(Id id, RVariant value) { _doCallback(id, value); }
	virtual void LineEditUpdated(Id id, String value) { _doCallback(id, value); }

private:
	void _doCallback(Id id, RVariant value);

};


}
