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
#include "M3DCore/AxisAlignedBox.h"
#include "M3DCore/List.h"

namespace m3d
{


static const GUID GEOMETRY_GUID = { 0xf75cc4d4, 0xbb15, 0x40de, { 0xb7, 0x18, 0x07, 0xc3, 0xbc, 0x40, 0x66, 0x1b } };

enum DrawApi { DRAW, DRAW_INDEXED };

// 1-1 with D3D12
enum M3D_PRIMITIVE_TOPOLOGY
{
	M3D_PRIMITIVE_TOPOLOGY_UNDEFINED = 0,
	M3D_PRIMITIVE_TOPOLOGY_POINTLIST = 1,
	M3D_PRIMITIVE_TOPOLOGY_LINELIST = 2,
	M3D_PRIMITIVE_TOPOLOGY_LINESTRIP = 3,
	M3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST = 4,
	M3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP = 5,
	M3D_PRIMITIVE_TOPOLOGY_LINELIST_ADJ = 10,
	M3D_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ = 11,
	M3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ = 12,
	M3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ = 13,
	M3D_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST = 33,
	M3D_PRIMITIVE_TOPOLOGY_2_CONTROL_POINT_PATCHLIST = 34,
	M3D_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST = 35,
	M3D_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST = 36,
	M3D_PRIMITIVE_TOPOLOGY_5_CONTROL_POINT_PATCHLIST = 37,
	M3D_PRIMITIVE_TOPOLOGY_6_CONTROL_POINT_PATCHLIST = 38,
	M3D_PRIMITIVE_TOPOLOGY_7_CONTROL_POINT_PATCHLIST = 39,
	M3D_PRIMITIVE_TOPOLOGY_8_CONTROL_POINT_PATCHLIST = 40,
	M3D_PRIMITIVE_TOPOLOGY_9_CONTROL_POINT_PATCHLIST = 41,
	M3D_PRIMITIVE_TOPOLOGY_10_CONTROL_POINT_PATCHLIST = 42,
	M3D_PRIMITIVE_TOPOLOGY_11_CONTROL_POINT_PATCHLIST = 43,
	M3D_PRIMITIVE_TOPOLOGY_12_CONTROL_POINT_PATCHLIST = 44,
	M3D_PRIMITIVE_TOPOLOGY_13_CONTROL_POINT_PATCHLIST = 45,
	M3D_PRIMITIVE_TOPOLOGY_14_CONTROL_POINT_PATCHLIST = 46,
	M3D_PRIMITIVE_TOPOLOGY_15_CONTROL_POINT_PATCHLIST = 47,
	M3D_PRIMITIVE_TOPOLOGY_16_CONTROL_POINT_PATCHLIST = 48,
	M3D_PRIMITIVE_TOPOLOGY_17_CONTROL_POINT_PATCHLIST = 49,
	M3D_PRIMITIVE_TOPOLOGY_18_CONTROL_POINT_PATCHLIST = 50,
	M3D_PRIMITIVE_TOPOLOGY_19_CONTROL_POINT_PATCHLIST = 51,
	M3D_PRIMITIVE_TOPOLOGY_20_CONTROL_POINT_PATCHLIST = 52,
	M3D_PRIMITIVE_TOPOLOGY_21_CONTROL_POINT_PATCHLIST = 53,
	M3D_PRIMITIVE_TOPOLOGY_22_CONTROL_POINT_PATCHLIST = 54,
	M3D_PRIMITIVE_TOPOLOGY_23_CONTROL_POINT_PATCHLIST = 55,
	M3D_PRIMITIVE_TOPOLOGY_24_CONTROL_POINT_PATCHLIST = 56,
	M3D_PRIMITIVE_TOPOLOGY_25_CONTROL_POINT_PATCHLIST = 57,
	M3D_PRIMITIVE_TOPOLOGY_26_CONTROL_POINT_PATCHLIST = 58,
	M3D_PRIMITIVE_TOPOLOGY_27_CONTROL_POINT_PATCHLIST = 59,
	M3D_PRIMITIVE_TOPOLOGY_28_CONTROL_POINT_PATCHLIST = 60,
	M3D_PRIMITIVE_TOPOLOGY_29_CONTROL_POINT_PATCHLIST = 61,
	M3D_PRIMITIVE_TOPOLOGY_30_CONTROL_POINT_PATCHLIST = 62,
	M3D_PRIMITIVE_TOPOLOGY_31_CONTROL_POINT_PATCHLIST = 63,
	M3D_PRIMITIVE_TOPOLOGY_32_CONTROL_POINT_PATCHLIST = 64,
};

struct GeometrySubset
{
	String name; // Name of the subset. does not have to be unique.
	AxisAlignedBox boundingBox; // Subset bounding box.
	M3D_PRIMITIVE_TOPOLOGY pt; // The primitive topology for this subset. They must belong to the same group as set for the Geometry (points, lines, triangles, patch)
	UINT count; // vertexCount if api is DRAW, else indexCount
	UINT startLocation; // startVertexLocation if api is DRAW, else startIndexLocation
	INT baseVertexLocation; // Offset into the vertex array
	mutable unsigned _cullingPlane; // This MAY improve performance in some cases by testing the frustum plane we failed last frame first.. Works good in theory :)
	GeometrySubset() : pt(M3D_PRIMITIVE_TOPOLOGY_UNDEFINED), count(0), startLocation(0), baseVertexLocation(0), _cullingPlane(0) {}
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