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
#include "M3DEngine/Chip.h"
#include "ValueDef.h"

namespace m3d
{

static const GUID NUMERIC_GUID = { 0x9478e85d, 0x274a, 0x4793,{ 0xb9, 0xcf, 0x9d, 0xd6, 0x8f, 0x40, 0x22, 0x8d } };

class Value;
class Vector;
class MatrixChip;

class STDCHIPS_API Numeric : public Chip
{
	CHIPDESC_DECL;
	CHIPMSG(InvalidExpressionException, FATAL, L"The given expression is not valid!")
	CHIPMSG(UnexpectedDimensionException, FATAL, L"An operand is of an unexepted dimension!")
public:
	Numeric();
	~Numeric();

	virtual Value *AsValue() { return nullptr; }
	virtual Vector *AsVector() { return nullptr; }
	virtual MatrixChip *AsMatrix() { return nullptr; }
};



}