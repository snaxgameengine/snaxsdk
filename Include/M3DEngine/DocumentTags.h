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

// For XML only. When storing large arrays (eg float*) this is the size of the array before we switch to base64 to help performance!
#define ARRAY_LIMIT_BEFORE_BASE64 (unsigned)1000


namespace m3d
{

enum DocumentEncryptionLevel {ENCLEVEL0 = 0x10, ENCLEVEL1 = 0x20, ENCLEVEL2 = 0x40}; 
enum DocumentCompressionLevel { DCL_NONE, DCL_LOW, DCL_NORMAL, DCL_HIGH };

namespace DocumentTags
{
	enum Tag 
	{
		Document,
		Class,
		Chips,
		Chip,
		Connections,
		Connection,
		ChipData,
		Data,
		Inheritance,
		ClassDiagram,
		Ptr,
		Editor,
		ClassVisual,
		Folder,
		SelectionCopy,
		Function,
		Parameter,
		Comments,
		Comment,
		Instances,
		Instance,
		Description_old,
		Description,
		Rect,
		Image,
		Text,
		PublishProfiles,
		Profile,
		name = 32, // Attributes from 32-63
		startclassid,
		startchipid,
		version,
		classid,
		chipid,
		id,
		conn,
		subconn,
		x,
		y,
		inx,
		iny,
		type,
		access,
		refreshmode,
		filename,
		extent,
		multiconnection,
		sx = multiconnection + 6, // Skipping elementes that were removed...
		sy,
		color,
		textSize,
		_content = 255 // Internal
	};

	static const char *TagStr[] = 
	{
		"Document", 
		"Class",
		"Chips", 
		"Chip",
		"Connections",
		"Connection",
		"ChipData",
		"Data",
		"Inheritance",
		"ClassDiagram",
		"Ptr",
		"Editor",
		"ClassVisual",
		"Folder",
		"SelectionCopy",
		"Function",
		"Parameter",
		"Comments",
		"Comment",
		"Instances",
		"Instance",
		"Publish",
		"Description",
		"Rect",
		"Image",
		"Text",
		"PublishProfiles",
		"Profile",
		"","","","",
		"name",
		"startclassid",
		"startchipid",
		"version",
		"classid",
		"chipid",
		"id",
		"conn",
		"subconn",
		"x", 
		"y", 
		"inx",
		"iny",
		"type",
		"access",
		"refreshmode",
		"filename",
		"extent",
		"multiconnection",
		"target",
		"filters",
		"compression",
		"copyproject",
		"includeall",
		"sx",
		"sy",
		"color",
		"textSize"
	};

	
}

}


 