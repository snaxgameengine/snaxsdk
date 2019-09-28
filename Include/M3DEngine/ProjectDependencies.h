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
#include "M3DCore/Path.h"
#include "M3DCore/Set.h"
#include "M3DCore/SlimRWLock.h"


namespace m3d
{

typedef Set<Path> PathSet;
template class M3DENGINE_API Set<Path>;

class M3DENGINE_API ProjectDependencies
{
public:
	enum Platform { WD_X64, WD_X86, WSA_X64, WSA_X86, WSA_ARM, WP8_X86, WP8_ARM }; // duplicate of that in publisher...

	ProjectDependencies();
	ProjectDependencies(Path baseDirectory, unsigned platform);
	~ProjectDependencies();

	bool AddDependency(String file);

	const PathSet &GetDependencies() const { return _deps; }
	Path GetBaseDirectory() const { return _baseDirectory; }

	unsigned GetPlatform() const { return _platform; }

private:
	// base directory for all files. They myst be in this directory or subdirectories.
	Path _baseDirectory;
	PathSet _deps;
	SlimRWLock _lock;
	unsigned _platform;
};

}