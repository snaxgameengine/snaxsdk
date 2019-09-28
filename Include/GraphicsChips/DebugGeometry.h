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
#include "M3DCore/MMath.h"


namespace m3d
{


static const GUID DEBUGGEOMETRY_GUID = { 0xf4b394ae, 0x6b87, 0x4fd1, { 0x88, 0xcd, 0xd6, 0xa, 0x2f, 0x67, 0x40, 0xc1 } };


struct DebugVertex
{
	XMFLOAT3 position;
	FLOAT index; // 0-4. for points without gs.
	XMUBYTE4 color;
	DebugVertex(const XMFLOAT3& position, const XMUBYTE4& color) : position(position), index(0.0f), color(color) {}
	DebugVertex(const DebugVertex& dv, FLOAT index) : position(dv.position), index(index), color(dv.color) {}
};

typedef List<DebugVertex> DebugVertexList;
template class GRAPHICSCHIPS_API List<DebugVertex>;


class GRAPHICSCHIPS_API DebugGeometry : public Chip
{
public:
	DebugGeometry();
	~DebugGeometry();

	// Called by engine every frame for hidden chips.
	void OnNewFrame() override;

	// Clears current geometry.
	virtual void ClearGeometry();

	// Adds geometry.
	virtual void AddPoint(const DebugVertex& p);
	virtual void AddLineSegment(const DebugVertex& a, const DebugVertex& b);
	virtual void AddTriangle(const DebugVertex& a, const DebugVertex& b, const DebugVertex& c);
	virtual void AddXZGrid(float scale = 10.0f);
	virtual void AddFrustum(CXMMATRIX vp, XMUBYTE4 color);
	virtual void AddBox(CXMMATRIX m, const XMFLOAT3& a, const XMFLOAT3& b, XMUBYTE4 color);

protected:
	struct CB
	{
		XMFLOAT4X4 vp;
		XMFLOAT2 gsScale;
		BYTE __padding[184]; // cb must be multiple of 256 bytes!
	};

	DebugVertexList _points;
	DebugVertexList _lineSegments;
	DebugVertexList _triangles;
};


}
