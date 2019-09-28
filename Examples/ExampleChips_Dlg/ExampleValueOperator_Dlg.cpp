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

#include "ExampleValueOperator_Dlg.h"

using namespace example;

// This macro registeres the class with the dialog manager
// for the given type of chip (EXAMPLEVALUEOPERATOR_GUID).
DIALOGDESC_DEF(ExampleValueOperator_Dlg, EXAMPLEVALUEOPERATOR_GUID);


ExampleValueOperator_Dlg::ExampleValueOperator_Dlg()
{
}

ExampleValueOperator_Dlg::~ExampleValueOperator_Dlg()
{
}

void ExampleValueOperator_Dlg::Init() 
{
	// Add items to the combo box.
	AddItem(L"Add", ExampleValueOperator::OT_ADD);
	AddItem(L"Substract", ExampleValueOperator::OT_SUB);
	AddItem(L"Multiply", ExampleValueOperator::OT_MUL);
	AddItem(L"Divide", ExampleValueOperator::OT_DIV);
	// Set the current item.
	SetInit(GetChip()->GetOperatorType(), ExampleValueOperator::OT_NONE);
}

bool ExampleValueOperator_Dlg::onSelectionChanged(m3d::RData data) 
{
	// Get the selected operator type from the combo box.
	ExampleValueOperator::OperatorType type = (ExampleValueOperator::OperatorType)data;	
	if (type == GetChip()->GetOperatorType())
		return false; // Return if nothing changed.
	// Set the operator type to the chip.
	GetChip()->SetOperatorType(type);
	// Return true to confirm that we updated the chip.
	return true;
}