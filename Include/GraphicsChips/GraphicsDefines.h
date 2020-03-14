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


namespace m3d
{
	enum struct ShaderModel { UNDEFINED, SM40_91, SM40_93, SM40, SM41, SM50, SM51 };
	enum struct ShaderType { UNDEFINED = 0, VS = 0x01, HS = 0x02, DS = 0x04, GS = 0x08, PS = 0x10, CS = 0x20, ALL = VS | HS | DS | GS | PS };


	// These enums and structs are 1-1 with the D3D12 versions...

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


	enum M3D_FORMAT
	{
		M3D_FORMAT_UNKNOWN = 0,
		M3D_FORMAT_R32G32B32A32_TYPELESS = 1,
		M3D_FORMAT_R32G32B32A32_FLOAT = 2,
		M3D_FORMAT_R32G32B32A32_UINT = 3,
		M3D_FORMAT_R32G32B32A32_SINT = 4,
		M3D_FORMAT_R32G32B32_TYPELESS = 5,
		M3D_FORMAT_R32G32B32_FLOAT = 6,
		M3D_FORMAT_R32G32B32_UINT = 7,
		M3D_FORMAT_R32G32B32_SINT = 8,
		M3D_FORMAT_R16G16B16A16_TYPELESS = 9,
		M3D_FORMAT_R16G16B16A16_FLOAT = 10,
		M3D_FORMAT_R16G16B16A16_UNORM = 11,
		M3D_FORMAT_R16G16B16A16_UINT = 12,
		M3D_FORMAT_R16G16B16A16_SNORM = 13,
		M3D_FORMAT_R16G16B16A16_SINT = 14,
		M3D_FORMAT_R32G32_TYPELESS = 15,
		M3D_FORMAT_R32G32_FLOAT = 16,
		M3D_FORMAT_R32G32_UINT = 17,
		M3D_FORMAT_R32G32_SINT = 18,
		M3D_FORMAT_R32G8X24_TYPELESS = 19,
		M3D_FORMAT_D32_FLOAT_S8X24_UINT = 20,
		M3D_FORMAT_R32_FLOAT_X8X24_TYPELESS = 21,
		M3D_FORMAT_X32_TYPELESS_G8X24_UINT = 22,
		M3D_FORMAT_R10G10B10A2_TYPELESS = 23,
		M3D_FORMAT_R10G10B10A2_UNORM = 24,
		M3D_FORMAT_R10G10B10A2_UINT = 25,
		M3D_FORMAT_R11G11B10_FLOAT = 26,
		M3D_FORMAT_R8G8B8A8_TYPELESS = 27,
		M3D_FORMAT_R8G8B8A8_UNORM = 28,
		M3D_FORMAT_R8G8B8A8_UNORM_SRGB = 29,
		M3D_FORMAT_R8G8B8A8_UINT = 30,
		M3D_FORMAT_R8G8B8A8_SNORM = 31,
		M3D_FORMAT_R8G8B8A8_SINT = 32,
		M3D_FORMAT_R16G16_TYPELESS = 33,
		M3D_FORMAT_R16G16_FLOAT = 34,
		M3D_FORMAT_R16G16_UNORM = 35,
		M3D_FORMAT_R16G16_UINT = 36,
		M3D_FORMAT_R16G16_SNORM = 37,
		M3D_FORMAT_R16G16_SINT = 38,
		M3D_FORMAT_R32_TYPELESS = 39,
		M3D_FORMAT_D32_FLOAT = 40,
		M3D_FORMAT_R32_FLOAT = 41,
		M3D_FORMAT_R32_UINT = 42,
		M3D_FORMAT_R32_SINT = 43,
		M3D_FORMAT_R24G8_TYPELESS = 44,
		M3D_FORMAT_D24_UNORM_S8_UINT = 45,
		M3D_FORMAT_R24_UNORM_X8_TYPELESS = 46,
		M3D_FORMAT_X24_TYPELESS_G8_UINT = 47,
		M3D_FORMAT_R8G8_TYPELESS = 48,
		M3D_FORMAT_R8G8_UNORM = 49,
		M3D_FORMAT_R8G8_UINT = 50,
		M3D_FORMAT_R8G8_SNORM = 51,
		M3D_FORMAT_R8G8_SINT = 52,
		M3D_FORMAT_R16_TYPELESS = 53,
		M3D_FORMAT_R16_FLOAT = 54,
		M3D_FORMAT_D16_UNORM = 55,
		M3D_FORMAT_R16_UNORM = 56,
		M3D_FORMAT_R16_UINT = 57,
		M3D_FORMAT_R16_SNORM = 58,
		M3D_FORMAT_R16_SINT = 59,
		M3D_FORMAT_R8_TYPELESS = 60,
		M3D_FORMAT_R8_UNORM = 61,
		M3D_FORMAT_R8_UINT = 62,
		M3D_FORMAT_R8_SNORM = 63,
		M3D_FORMAT_R8_SINT = 64,
		M3D_FORMAT_A8_UNORM = 65,
		M3D_FORMAT_R1_UNORM = 66,
		M3D_FORMAT_R9G9B9E5_SHAREDEXP = 67,
		M3D_FORMAT_R8G8_B8G8_UNORM = 68,
		M3D_FORMAT_G8R8_G8B8_UNORM = 69,
		M3D_FORMAT_BC1_TYPELESS = 70,
		M3D_FORMAT_BC1_UNORM = 71,
		M3D_FORMAT_BC1_UNORM_SRGB = 72,
		M3D_FORMAT_BC2_TYPELESS = 73,
		M3D_FORMAT_BC2_UNORM = 74,
		M3D_FORMAT_BC2_UNORM_SRGB = 75,
		M3D_FORMAT_BC3_TYPELESS = 76,
		M3D_FORMAT_BC3_UNORM = 77,
		M3D_FORMAT_BC3_UNORM_SRGB = 78,
		M3D_FORMAT_BC4_TYPELESS = 79,
		M3D_FORMAT_BC4_UNORM = 80,
		M3D_FORMAT_BC4_SNORM = 81,
		M3D_FORMAT_BC5_TYPELESS = 82,
		M3D_FORMAT_BC5_UNORM = 83,
		M3D_FORMAT_BC5_SNORM = 84,
		M3D_FORMAT_B5G6R5_UNORM = 85,
		M3D_FORMAT_B5G5R5A1_UNORM = 86,
		M3D_FORMAT_B8G8R8A8_UNORM = 87,
		M3D_FORMAT_B8G8R8X8_UNORM = 88,
		M3D_FORMAT_R10G10B10_XR_BIAS_A2_UNORM = 89,
		M3D_FORMAT_B8G8R8A8_TYPELESS = 90,
		M3D_FORMAT_B8G8R8A8_UNORM_SRGB = 91,
		M3D_FORMAT_B8G8R8X8_TYPELESS = 92,
		M3D_FORMAT_B8G8R8X8_UNORM_SRGB = 93,
		M3D_FORMAT_BC6H_TYPELESS = 94,
		M3D_FORMAT_BC6H_UF16 = 95,
		M3D_FORMAT_BC6H_SF16 = 96,
		M3D_FORMAT_BC7_TYPELESS = 97,
		M3D_FORMAT_BC7_UNORM = 98,
		M3D_FORMAT_BC7_UNORM_SRGB = 99,
		M3D_FORMAT_AYUV = 100,
		M3D_FORMAT_Y410 = 101,
		M3D_FORMAT_Y416 = 102,
		M3D_FORMAT_NV12 = 103,
		M3D_FORMAT_P010 = 104,
		M3D_FORMAT_P016 = 105,
		M3D_FORMAT_420_OPAQUE = 106,
		M3D_FORMAT_YUY2 = 107,
		M3D_FORMAT_Y210 = 108,
		M3D_FORMAT_Y216 = 109,
		M3D_FORMAT_NV11 = 110,
		M3D_FORMAT_AI44 = 111,
		M3D_FORMAT_IA44 = 112,
		M3D_FORMAT_P8 = 113,
		M3D_FORMAT_A8P8 = 114,
		M3D_FORMAT_B4G4R4A4_UNORM = 115,

		M3D_FORMAT_P208 = 130,
		M3D_FORMAT_V208 = 131,
		M3D_FORMAT_V408 = 132,


		M3D_FORMAT_FORCE_UINT = 0xffffffff
	};


	enum M3D_SHADER_COMPONENT_MAPPING
	{
		M3D_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_0 = 0,
		M3D_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_1 = 1,
		M3D_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_2 = 2,
		M3D_SHADER_COMPONENT_MAPPING_FROM_MEMORY_COMPONENT_3 = 3,
		M3D_SHADER_COMPONENT_MAPPING_FORCE_VALUE_0 = 4,
		M3D_SHADER_COMPONENT_MAPPING_FORCE_VALUE_1 = 5
	};

#define M3D_SHADER_COMPONENT_MAPPING_MASK 0x7 
#define M3D_SHADER_COMPONENT_MAPPING_SHIFT 3 
#define M3D_SHADER_COMPONENT_MAPPING_ALWAYS_SET_BIT_AVOIDING_ZEROMEM_MISTAKES (1<<(M3D_SHADER_COMPONENT_MAPPING_SHIFT*4)) 
#define M3D_ENCODE_SHADER_4_COMPONENT_MAPPING(Src0,Src1,Src2,Src3) ((((Src0)&M3D_SHADER_COMPONENT_MAPPING_MASK)| \
                                                                (((Src1)&M3D_SHADER_COMPONENT_MAPPING_MASK)<<M3D_SHADER_COMPONENT_MAPPING_SHIFT)| \
                                                                (((Src2)&M3D_SHADER_COMPONENT_MAPPING_MASK)<<(M3D_SHADER_COMPONENT_MAPPING_SHIFT*2))| \
                                                                (((Src3)&M3D_SHADER_COMPONENT_MAPPING_MASK)<<(M3D_SHADER_COMPONENT_MAPPING_SHIFT*3))| \
                                                                M3D_SHADER_COMPONENT_MAPPING_ALWAYS_SET_BIT_AVOIDING_ZEROMEM_MISTAKES))
#define M3D_DECODE_SHADER_4_COMPONENT_MAPPING(ComponentToExtract,Mapping) ((M3D_SHADER_COMPONENT_MAPPING)(Mapping >> (M3D_SHADER_COMPONENT_MAPPING_SHIFT*ComponentToExtract) & M3D_SHADER_COMPONENT_MAPPING_MASK))
#define M3D_DEFAULT_SHADER_4_COMPONENT_MAPPING M3D_ENCODE_SHADER_4_COMPONENT_MAPPING(0,1,2,3) 


	enum M3D_DSV_FLAGS
	{
		M3D_DSV_FLAG_NONE = 0,
		M3D_DSV_FLAG_READ_ONLY_DEPTH = 0x1,
		M3D_DSV_FLAG_READ_ONLY_STENCIL = 0x2
	};
	DEFINE_ENUM_FLAG_OPERATORS(M3D_DSV_FLAGS);


	enum M3D_BUFFER_UAV_FLAGS
	{
		M3D_BUFFER_UAV_FLAG_NONE = 0,
		M3D_BUFFER_UAV_FLAG_RAW = 0x1
	};
	DEFINE_ENUM_FLAG_OPERATORS(M3D_BUFFER_UAV_FLAGS);


	struct M3D_SAMPLE_DESC
	{
		UINT Count;
		UINT Quality;
	};


	enum M3D_RESOURCE_FLAGS
	{
		M3D_RESOURCE_FLAG_NONE = 0,
		M3D_RESOURCE_FLAG_ALLOW_RENDER_TARGET = 0x1,
		M3D_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL = 0x2,
		M3D_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS = 0x4,
		M3D_RESOURCE_FLAG_DENY_SHADER_RESOURCE = 0x8,
		M3D_RESOURCE_FLAG_ALLOW_CROSS_ADAPTER = 0x10,
		M3D_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS = 0x20,
		M3D_RESOURCE_FLAG_VIDEO_DECODE_REFERENCE_ONLY = 0x40
	};
	DEFINE_ENUM_FLAG_OPERATORS(M3D_RESOURCE_FLAGS);

	
	enum M3D_FILTER
	{
		M3D_FILTER_MIN_MAG_MIP_POINT = 0,
		M3D_FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
		M3D_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
		M3D_FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
		M3D_FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
		M3D_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
		M3D_FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
		M3D_FILTER_MIN_MAG_MIP_LINEAR = 0x15,
		M3D_FILTER_ANISOTROPIC = 0x55,
		M3D_FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
		M3D_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
		M3D_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
		M3D_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
		M3D_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
		M3D_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
		M3D_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
		M3D_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
		M3D_FILTER_COMPARISON_ANISOTROPIC = 0xd5,
		M3D_FILTER_MINIMUM_MIN_MAG_MIP_POINT = 0x100,
		M3D_FILTER_MINIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x101,
		M3D_FILTER_MINIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x104,
		M3D_FILTER_MINIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x105,
		M3D_FILTER_MINIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x110,
		M3D_FILTER_MINIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x111,
		M3D_FILTER_MINIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x114,
		M3D_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR = 0x115,
		M3D_FILTER_MINIMUM_ANISOTROPIC = 0x155,
		M3D_FILTER_MAXIMUM_MIN_MAG_MIP_POINT = 0x180,
		M3D_FILTER_MAXIMUM_MIN_MAG_POINT_MIP_LINEAR = 0x181,
		M3D_FILTER_MAXIMUM_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x184,
		M3D_FILTER_MAXIMUM_MIN_POINT_MAG_MIP_LINEAR = 0x185,
		M3D_FILTER_MAXIMUM_MIN_LINEAR_MAG_MIP_POINT = 0x190,
		M3D_FILTER_MAXIMUM_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x191,
		M3D_FILTER_MAXIMUM_MIN_MAG_LINEAR_MIP_POINT = 0x194,
		M3D_FILTER_MAXIMUM_MIN_MAG_MIP_LINEAR = 0x195,
		M3D_FILTER_MAXIMUM_ANISOTROPIC = 0x1d5
	};


	enum M3D_TEXTURE_ADDRESS_MODE
	{
		M3D_TEXTURE_ADDRESS_MODE_WRAP = 1,
		M3D_TEXTURE_ADDRESS_MODE_MIRROR = 2,
		M3D_TEXTURE_ADDRESS_MODE_CLAMP = 3,
		M3D_TEXTURE_ADDRESS_MODE_BORDER = 4,
		M3D_TEXTURE_ADDRESS_MODE_MIRROR_ONCE = 5
	};


	enum M3D_COMPARISON_FUNC
	{
		M3D_COMPARISON_FUNC_NEVER = 1,
		M3D_COMPARISON_FUNC_LESS = 2,
		M3D_COMPARISON_FUNC_EQUAL = 3,
		M3D_COMPARISON_FUNC_LESS_EQUAL = 4,
		M3D_COMPARISON_FUNC_GREATER = 5,
		M3D_COMPARISON_FUNC_NOT_EQUAL = 6,
		M3D_COMPARISON_FUNC_GREATER_EQUAL = 7,
		M3D_COMPARISON_FUNC_ALWAYS = 8
	};


	enum M3D_STATIC_BORDER_COLOR
	{
		M3D_STATIC_BORDER_COLOR_TRANSPARENT_BLACK = 0,
		M3D_STATIC_BORDER_COLOR_OPAQUE_BLACK = (M3D_STATIC_BORDER_COLOR_TRANSPARENT_BLACK + 1),
		M3D_STATIC_BORDER_COLOR_OPAQUE_WHITE = (M3D_STATIC_BORDER_COLOR_OPAQUE_BLACK + 1)
	};


	enum M3D_SHADER_VISIBILITY
	{
		M3D_SHADER_VISIBILITY_ALL = 0,
		M3D_SHADER_VISIBILITY_VERTEX = 1,
		M3D_SHADER_VISIBILITY_HULL = 2,
		M3D_SHADER_VISIBILITY_DOMAIN = 3,
		M3D_SHADER_VISIBILITY_GEOMETRY = 4,
		M3D_SHADER_VISIBILITY_PIXEL = 5
	};


	struct M3D_SAMPLER_DESC
	{
		M3D_FILTER Filter = M3D_FILTER_ANISOTROPIC;
		M3D_TEXTURE_ADDRESS_MODE AddressU = M3D_TEXTURE_ADDRESS_MODE_WRAP;
		M3D_TEXTURE_ADDRESS_MODE AddressV = M3D_TEXTURE_ADDRESS_MODE_WRAP;
		M3D_TEXTURE_ADDRESS_MODE AddressW = M3D_TEXTURE_ADDRESS_MODE_WRAP;
		FLOAT MipLODBias = 0;
		UINT MaxAnisotropy = 16;
		M3D_COMPARISON_FUNC ComparisonFunc = M3D_COMPARISON_FUNC_LESS_EQUAL;
		FLOAT BorderColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f};
		FLOAT MinLOD = -FLT_MAX;
		FLOAT MaxLOD = FLT_MAX;
	};


	struct M3D_STATIC_SAMPLER_DESC
	{
		M3D_FILTER Filter;
		M3D_TEXTURE_ADDRESS_MODE AddressU;
		M3D_TEXTURE_ADDRESS_MODE AddressV;
		M3D_TEXTURE_ADDRESS_MODE AddressW;
		FLOAT MipLODBias;
		UINT MaxAnisotropy;
		M3D_COMPARISON_FUNC ComparisonFunc;
		M3D_STATIC_BORDER_COLOR BorderColor;
		FLOAT MinLOD;
		FLOAT MaxLOD;
		UINT ShaderRegister;
		UINT RegisterSpace;
		M3D_SHADER_VISIBILITY ShaderVisibility;
	};


	enum M3D_FILL_MODE
	{
		M3D_FILL_MODE_WIREFRAME = 2,
		M3D_FILL_MODE_SOLID = 3
	};


	enum M3D_CULL_MODE
	{
		M3D_CULL_MODE_NONE = 1,
		M3D_CULL_MODE_FRONT = 2,
		M3D_CULL_MODE_BACK = 3
	};


	enum M3D_CONSERVATIVE_RASTERIZATION_MODE
	{
		M3D_CONSERVATIVE_RASTERIZATION_MODE_OFF = 0,
		M3D_CONSERVATIVE_RASTERIZATION_MODE_ON = 1
	};


	enum M3D_DEPTH_WRITE_MASK
	{
		M3D_DEPTH_WRITE_MASK_ZERO = 0,
		M3D_DEPTH_WRITE_MASK_ALL = 1
	};


	enum M3D_STENCIL_OP
	{
		M3D_STENCIL_OP_KEEP = 1,
		M3D_STENCIL_OP_ZERO = 2,
		M3D_STENCIL_OP_REPLACE = 3,
		M3D_STENCIL_OP_INCR_SAT = 4,
		M3D_STENCIL_OP_DECR_SAT = 5,
		M3D_STENCIL_OP_INVERT = 6,
		M3D_STENCIL_OP_INCR = 7,
		M3D_STENCIL_OP_DECR = 8
	};


	enum M3D_BLEND
	{
		M3D_BLEND_ZERO = 1,
		M3D_BLEND_ONE = 2,
		M3D_BLEND_SRC_COLOR = 3,
		M3D_BLEND_INV_SRC_COLOR = 4,
		M3D_BLEND_SRC_ALPHA = 5,
		M3D_BLEND_INV_SRC_ALPHA = 6,
		M3D_BLEND_DEST_ALPHA = 7,
		M3D_BLEND_INV_DEST_ALPHA = 8,
		M3D_BLEND_DEST_COLOR = 9,
		M3D_BLEND_INV_DEST_COLOR = 10,
		M3D_BLEND_SRC_ALPHA_SAT = 11,
		M3D_BLEND_BLEND_FACTOR = 14,
		M3D_BLEND_INV_BLEND_FACTOR = 15,
		M3D_BLEND_SRC1_COLOR = 16,
		M3D_BLEND_INV_SRC1_COLOR = 17,
		M3D_BLEND_SRC1_ALPHA = 18,
		M3D_BLEND_INV_SRC1_ALPHA = 19
	};


	enum M3D_BLEND_OP
	{
		M3D_BLEND_OP_ADD = 1,
		M3D_BLEND_OP_SUBTRACT = 2,
		M3D_BLEND_OP_REV_SUBTRACT = 3,
		M3D_BLEND_OP_MIN = 4,
		M3D_BLEND_OP_MAX = 5
	};


	enum M3D_LOGIC_OP
	{
		M3D_LOGIC_OP_CLEAR = 0,
		M3D_LOGIC_OP_SET = (M3D_LOGIC_OP_CLEAR + 1),
		M3D_LOGIC_OP_COPY = (M3D_LOGIC_OP_SET + 1),
		M3D_LOGIC_OP_COPY_INVERTED = (M3D_LOGIC_OP_COPY + 1),
		M3D_LOGIC_OP_NOOP = (M3D_LOGIC_OP_COPY_INVERTED + 1),
		M3D_LOGIC_OP_INVERT = (M3D_LOGIC_OP_NOOP + 1),
		M3D_LOGIC_OP_AND = (M3D_LOGIC_OP_INVERT + 1),
		M3D_LOGIC_OP_NAND = (M3D_LOGIC_OP_AND + 1),
		M3D_LOGIC_OP_OR = (M3D_LOGIC_OP_NAND + 1),
		M3D_LOGIC_OP_NOR = (M3D_LOGIC_OP_OR + 1),
		M3D_LOGIC_OP_XOR = (M3D_LOGIC_OP_NOR + 1),
		M3D_LOGIC_OP_EQUIV = (M3D_LOGIC_OP_XOR + 1),
		M3D_LOGIC_OP_AND_REVERSE = (M3D_LOGIC_OP_EQUIV + 1),
		M3D_LOGIC_OP_AND_INVERTED = (M3D_LOGIC_OP_AND_REVERSE + 1),
		M3D_LOGIC_OP_OR_REVERSE = (M3D_LOGIC_OP_AND_INVERTED + 1),
		M3D_LOGIC_OP_OR_INVERTED = (M3D_LOGIC_OP_OR_REVERSE + 1)
	};

	enum M3D_COLOR_WRITE_ENABLE
	{
		M3D_COLOR_WRITE_ENABLE_RED = 1,
		M3D_COLOR_WRITE_ENABLE_GREEN = 2,
		M3D_COLOR_WRITE_ENABLE_BLUE = 4,
		M3D_COLOR_WRITE_ENABLE_ALPHA = 8,
		M3D_COLOR_WRITE_ENABLE_ALL = (((M3D_COLOR_WRITE_ENABLE_RED | M3D_COLOR_WRITE_ENABLE_GREEN) | M3D_COLOR_WRITE_ENABLE_BLUE) | M3D_COLOR_WRITE_ENABLE_ALPHA)
	};
	DEFINE_ENUM_FLAG_OPERATORS(M3D_COLOR_WRITE_ENABLE);

}
