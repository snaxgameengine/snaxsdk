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
#include "MMath.h"


namespace m3d
{

class Sphere;


class M3DCORE_API AxisAlignedBox
{
	XMFLOAT3 _minP;
	XMFLOAT3 _maxP;
	bool _isInfinite;

public:
	AxisAlignedBox();
	AxisAlignedBox(const XMFLOAT3 &minP, const XMFLOAT3 &maxP);
	AxisAlignedBox(const AxisAlignedBox &rhs);
	AxisAlignedBox(const Sphere &rhs);
	~AxisAlignedBox();

	AxisAlignedBox &operator=(const AxisAlignedBox &rhs);

	inline bool IsInfinite() const { return _isInfinite; }
	inline const XMFLOAT3 &GetMin() const { return _minP; }
	inline const XMFLOAT3 &GetMax() const { return _maxP; }

	void SetInfinite();
	void SetNull();
	void Set(const XMFLOAT3 &minP, const XMFLOAT3 &maxP);
	void Set(unsigned count, const XMFLOAT3* vertices, unsigned vStride);
	void Set(unsigned count, const XMFLOAT3* vertices, unsigned vStride, const unsigned short *indices);
	void Set(unsigned count, const XMFLOAT3* vertices, unsigned vStride, const unsigned *indices);

	AxisAlignedBox &Merge(const XMFLOAT3 &p);
	AxisAlignedBox &Merge(const AxisAlignedBox &bb);
	AxisAlignedBox &Merge(const Sphere &s);

	AxisAlignedBox &Transform(CXMMATRIX m);

	AxisAlignedBox Intersect(const AxisAlignedBox &aab) const;

	inline AxisAlignedBox &operator+=(const AxisAlignedBox &rhs) { return Merge(rhs); }
	inline AxisAlignedBox &operator+=(const Sphere &rhs) { return Merge(rhs); }
	inline AxisAlignedBox &operator*=(CXMMATRIX m) { return Transform(m); }
	inline AxisAlignedBox operator+(const AxisAlignedBox &rhs) const  { return AxisAlignedBox(*this).Merge(rhs); }
	inline AxisAlignedBox operator+(const Sphere &rhs) const  { return AxisAlignedBox(*this).Merge(rhs); }
	inline AxisAlignedBox operator*(CXMMATRIX m) const { return AxisAlignedBox(*this).Transform(m); }

};




}