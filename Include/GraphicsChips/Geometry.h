// Copyright(c) 2013-2020, mCODE A/S
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
#include "GraphicsDefines.h"
#include "M3DEngine/Chip.h"
#include "M3DCore/AxisAlignedBox.h"
#include "M3DCore/List.h"

namespace m3d
{


static const GUID GEOMETRY_GUID = { 0xf75cc4d4, 0xbb15, 0x40de, { 0xb7, 0x18, 0x07, 0xc3, 0xbc, 0x40, 0x66, 0x1b } };

enum DrawApi { DRAW, DRAW_INDEXED };


struct GeometrySubset
{
	String name; // Name of the subset. does not have to be unique.
	AxisAlignedBox boundingBox; // Subset bounding box.
	M3D_PRIMITIVE_TOPOLOGY pt = M3D_PRIMITIVE_TOPOLOGY_UNDEFINED; // The primitive topology for this subset. They must belong to the same group as set for the Geometry (points, lines, triangles, patch)
	UINT count = 0u; // vertexCount if api is DRAW, else indexCount
	UINT startLocation = 0u; // startVertexLocation if api is DRAW, else startIndexLocation
	INT baseVertexLocation = 0; // Offset into the vertex array
	mutable unsigned _cullingPlane = 0u; // This MAY improve performance in some cases by testing the frustum plane we failed last frame first.. Works good in theory :)
};

typedef List<GeometrySubset> GeometrySubsetList;
template class GRAPHICSCHIPS_API List<GeometrySubset>;

class GRAPHICSCHIPS_API Geometry : public Chip
{
	CHIPMSG(MSG_VB_FAIL, FATAL, L"Failed to create vertex buffer!")
	CHIPMSG(MSG_IB_FAIL, FATAL, L"Failed to create index buffer!")
public:
	Geometry();
	~Geometry();

	bool CopyChip(Chip* chip) override;
	bool LoadChip(DocumentLoader& loader) override;
	bool SaveChip(DocumentSaver& saver) const override;

	void OnDestroyDevice() override { return DestroyDeviceObjects(); }

	// Prepares GPU before rendering. Will create device objects if they do not exist!
	virtual void Prepare() = 0;
	// Updates anything neccessary in the geometry!
	virtual void Update() {}
	// Create gpu objects. Most chips overrides this.
	virtual void CreateDeviceObjects() = 0;
	// Destroy gpu objects
	virtual void DestroyDeviceObjects() = 0;
	// true if gpu objects is created.
	virtual bool IsGPUInit() const = 0;

	// Clears all settings in the chip.
	virtual void Clear();
	// Get the draw api. (If we are to use index buffer or not) To be overridden!
	virtual DrawApi GetAPI() const { return DRAW; }
	// Sets the subsets.
	virtual void SetSubsets(const GeometrySubsetList& subsets) { _subsets = subsets; }
	// Adds a new subset to the end of the list.
	virtual void AddSubset(const GeometrySubset& subset) { _subsets.push_back(subset); }
	// Sets a subset at given index.
	virtual void SetSubset(const GeometrySubset& subset, unsigned index) { _subsets[index] = subset; }
	// Gets the list of subsets.
	virtual const GeometrySubsetList& GetSubsets() const { return _subsets; }

	// Set bounding box for given subset.
	virtual void SetBoundingBox(const AxisAlignedBox& aabb, unsigned subset) { _subsets[subset].boundingBox = aabb; }

protected:
	GeometrySubsetList _subsets;

	// These are generated!
	unsigned _cullingPlane; // This MAY improve performance in some cases by testing the frustum plane we failed last frame first.. Works good in theory :)

};




}