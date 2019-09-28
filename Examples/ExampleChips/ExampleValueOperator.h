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
#include "StdChips/Value.h"


namespace example
{

// Define a unique global identifier for this chip. Use a guid-generator to make unique ids!
static const GUID EXAMPLEVALUEOPERATOR_GUID = { 0x11da1ffd, 0xe51, 0x4166,{ 0xb8, 0xc, 0x5f, 0x12, 0x2d, 0x34, 0xd8, 0x72 } };

// Create a chip deriving from the m3d::Value chip.
class EXAMPLECHIPS_API ExampleValueOperator : public m3d::Value
{
	// This macro is needed for all new chips!
	CHIPDESC_DECL;
public:
	ExampleValueOperator();
	~ExampleValueOperator();

	// This function is called when copying chips.
	bool CopyChip(m3d::Chip *chip) override;
	// This function is called when loading/deserializing the chip from file.
	bool LoadChip(m3d::DocumentLoader &loader) override;
	// This function is called when saving/serializing the chip to file.
	bool SaveChip(m3d::DocumentSaver &saver) const override;
	// This is the common entry chip for all chips. 
	void CallChip() override;

	// This is the function called when getting a value (double) from the chip.
	m3d::value GetValue() override;
	// This is the function called when setting a value to the chip.
	void SetValue(m3d::value v) override;

	// Define our operator types.
	enum OperatorType 
	{
		OT_NONE,
		OT_ADD,
		OT_SUB,
		OT_MUL,
		OT_DIV
	};

	// Get our current operator type.
	OperatorType GetOperatorType() const { return _operatorType; }
	// Set out operator type.
	void SetOperatorType(OperatorType operatorType) { _operatorType = operatorType; }

private:
	// Out current operator type.
	OperatorType _operatorType;
};

}