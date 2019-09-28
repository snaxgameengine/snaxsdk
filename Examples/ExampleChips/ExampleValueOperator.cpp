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


#include "ExampleValueOperator.h"
#include "M3DEngine\DocumentSaveLoadUtil.h"

using namespace example;

// This macro registrates the chip with the engine, sets a name for use 
// in the developer, and defines the run-time type hierarchy.
// It is important to set the correct baseGuid to match the class we derive from!
// There are many variations of this macro, but this is the most common.
CHIPDESCV1_DEF(ExampleValueOperator, L"Example Value Operator", EXAMPLEVALUEOPERATOR_GUID, m3d::VALUE_GUID);



ExampleValueOperator::ExampleValueOperator()
{
	// Initialize the operator type to NONE.
	_operatorType = OT_NONE;

	// Define two child connectors for this chip,
	// both accepting a Value chip.
	CREATE_CHILD(0, m3d::VALUE_GUID, false, UP, L"Operand 1");
	CREATE_CHILD(1, m3d::VALUE_GUID, false, UP, L"Operand 2");
}

ExampleValueOperator::~ExampleValueOperator()
{
}

bool ExampleValueOperator::CopyChip(m3d::Chip *chip)
{
	// This function is called when we need to make a copy of the supplied chip.
	// Cast to our type. The base class implementation will return false
	// if the type is incorrect.
	ExampleValueOperator *c = dynamic_cast<ExampleValueOperator*>(chip);
	// Call the case class implementation first!
	B_RETURN(Value::CopyChip(c));
	// Set the operator type.
	_operatorType = c->_operatorType;
	return true;
}

bool ExampleValueOperator::LoadChip(m3d::DocumentLoader &loader)
{
	// This function is called when loading (deserializing) the chip.
	// Call the base class implementation first.
	B_RETURN(Value::LoadChip(loader));
	// Load the operator type using the LOAD macro.
	LOAD("operatorType", _operatorType);
	return true;
}

bool ExampleValueOperator::SaveChip(m3d::DocumentSaver &saver) const
{
	// This function is called when saving (serializing) the chip.
	// Call the base class implementation first.
	B_RETURN(Value::SaveChip(saver));
	// Save the current operator type using the SAVE macro.
	SAVE("operatorType", _operatorType);
	return true;
}

void ExampleValueOperator::CallChip()
{
	// Nothing to do here. Call the base class' implementation.
	Value::CallChip();
}

m3d::value ExampleValueOperator::GetValue() 
{
	// Check if the chip needs to recalculate based on 
	// the Chip's current Refresh-mode setting.
	if (!Refresh)
		return _value;

	// Get the Chip connected to the first child connection.
	m3d::ChildPtr<m3d::Value> ch0 = GetChild(0);
	if (!ch0) {
		// If no chip is connected, add a message to the chip to alert the user.
		AddMessage(MissingChildException(0));
		// Return a value of 0.0 in this case!
		return _value = 0.0f;
	}

	// Get the Chip connected to the second child connection.
	m3d::ChildPtr<m3d::Value> ch1 = GetChild(1);
	if (!ch1) {
		// If no chip is connected, add a message to the chip to alert the user.
		AddMessage(MissingChildException(1));
		// Return a value of 0.0 in this case!
		return _value = 0.0f;
	}

	// Get the actual value (double) from the two connected chips.
	m3d::value operand0 = ch0->GetValue();
	m3d::value operand1 = ch1->GetValue();

	// Do an operation based on the currently set operator type.
	switch (_operatorType)
	{
	case OT_ADD:
		_value = operand0 + operand1;
		break;
	case OT_SUB:
		_value = operand0 - operand1;
		break;
	case OT_MUL:
		_value = operand0 * operand1;
		break;
	case OT_DIV:
		_value = operand0 / operand1;
		break;
	default:
		// No operator-type is set. Alert the user!
		AddMessage(UninitializedException(L"Please select the operator type!"));
		_value = 0.0;
		break;
	}
	// Return the resulting value of the operation!
	return _value;
}

void ExampleValueOperator::SetValue(m3d::value v) 
{
	// We do not need to do anything when a value is set.
	// Just call the base class' implementation.
	Value::SetValue(v);
}