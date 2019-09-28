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
#include "M3DCore/MMath.h"
#include "M3DEngine/Chip.h"
#include "ArrayChip.h"
#include "MatrixChip.h"

namespace m3d
{


static const GUID MATRIXARRAY_GUID = { 0xff95e39a, 0xd1c8, 0x4cfc, { 0x8a, 0x39, 0x95, 0x46, 0xe4, 0x15, 0x48, 0xee } };

class STDCHIPS_API MatrixArray : public ArrayChip
{
	CHIPDESC_DECL;
public:
	typedef List<XMFLOAT4X4> ArrayType;

	MatrixArray();
	virtual ~MatrixArray();

	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual unsigned GetContainerSize() override;
	virtual void SetContainerSize(unsigned size) override;
	virtual void ClearContainer() override;

	virtual const List<XMFLOAT4X4> &GetArray() const { return _array; }
	virtual List<XMFLOAT4X4> &GetArray() { return _array; }

	virtual void SetArray(const List<XMFLOAT4X4> &a);
	virtual void SetArray(List<XMFLOAT4X4> &&a);

	virtual const XMFLOAT4X4 &GetMatrix(unsigned index);
	virtual void SetMatrix(unsigned index, const XMFLOAT4X4 &m);

protected:
	List<XMFLOAT4X4> _array;

};



}