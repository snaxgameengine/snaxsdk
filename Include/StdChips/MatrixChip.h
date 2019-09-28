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
#include "Numeric.h"


namespace m3d
{


static const GUID MATRIXCHIP_GUID = { 0x01466aab, 0x80e4, 0x447c, { 0x96, 0x62, 0x08, 0x9b, 0x79, 0x48, 0x52, 0x31 } };

class STDCHIPS_API MatrixChip : public Numeric
{
	CHIPDESC_DECL;
public:
	static const XMFLOAT4X4 IDENTITY;

	MatrixChip();
	virtual ~MatrixChip();

	virtual bool InitChip() override;
	virtual bool CopyChip(Chip *chip) override;
	virtual bool LoadChip(DocumentLoader &loader) override;
	virtual bool SaveChip(DocumentSaver &saver) const override;

	virtual void CallChip() override;

	MatrixChip *AsMatrix() override { return this; }

	virtual const XMFLOAT4X4 &GetMatrix();
	virtual void SetMatrix(const XMFLOAT4X4 &m);

	// These are utility functions that by default uses matrix from GetMatrix().
	// Some special Matrix-derivatives can overload these to do the calculation in a wiser manner. maybe... :/
	virtual bool GetInverseMatrix(XMFLOAT4X4 &invertedMatrix);
	virtual bool GetDecomposedMatrix(XMFLOAT3 &translation, XMFLOAT4 &rotation, XMFLOAT3 &scaling);

	virtual const XMFLOAT4X4 &GetChipMatrix() const { return _matrix; }

protected:
	XMFLOAT4X4 _matrix;
};



}