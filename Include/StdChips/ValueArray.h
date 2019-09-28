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
#include "ArrayChip.h"
#include "ValueDef.h"
#include "M3DCore/CommonTemplates.h"

namespace m3d
{


static const GUID VALUEARRAY_GUID = { 0x18f99330, 0xdd34, 0x46f6, { 0xa2, 0x6e, 0x4a, 0x29, 0x78, 0xe9, 0xc0, 0x77 } };

class STDCHIPS_API ValueArray : public ArrayChip
{
	CHIPDESC_DECL;
public:
	typedef List<value> ArrayType;

	ValueArray();
	virtual ~ValueArray();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual unsigned GetContainerSize() override;
	virtual void SetContainerSize(unsigned size) override;
	virtual void ClearContainer() override;

	virtual const ArrayType &GetArray() const;
	virtual void SetArray(const ArrayType &a);
	virtual void SetArray(ArrayType &&a);

	virtual value GetValue(unsigned index) const;
	virtual void SetValue(unsigned index, value v);

	virtual void Sort(int dir = 1, unsigned start = 0, unsigned count = -1);

protected:
	ArrayType _array; // Using float array because this is the gpu-standard.
};



}