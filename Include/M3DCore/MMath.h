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


//#define  XM_NO_OPERATOR_OVERLOADS
#include "Exports.h"
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include "List.h"

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace m3d
{

static bool flteql(float A, float B, float maxDiff = 1.0e-10f, float maxRelDiff = 1.0e-5f)
{
    float diff = fabs(A - B);
    return diff <= maxDiff || diff <= (((B = fabs(B)) > (A = fabs(A)) ? B : A) * maxRelDiff); 
}

const XMUBYTE4 BLACK = XMUBYTE4(0xFF000000);
const XMUBYTE4 GRAY25 = XMUBYTE4(0xFF404040);
const XMUBYTE4 GRAY = XMUBYTE4(0xFF808080);
const XMUBYTE4 GRAY75 = XMUBYTE4(0xFFC0C0C0);
const XMUBYTE4 WHITE = XMUBYTE4(0xFFFFFFFF);
const XMUBYTE4 RED = XMUBYTE4(0xFF0000FF);
const XMUBYTE4 GREEN = XMUBYTE4(0xFF00FF00);
const XMUBYTE4 BLUE = XMUBYTE4(0xFFFF0000);
const XMUBYTE4 YELLOW = XMUBYTE4(0xFF00FFFF);
const XMUBYTE4 MAGENTA = XMUBYTE4(0xFFFF00FF);
const XMUBYTE4 CYAN = XMUBYTE4(0xFFFFFF00);


template class M3DCORE_API List<XMFLOAT2>;
template class M3DCORE_API List<XMFLOAT3>;
template class M3DCORE_API List<XMFLOAT4>;
template class M3DCORE_API List<XMCOLOR>;
template class M3DCORE_API List<XMUBYTEN4>;
template class M3DCORE_API List<XMUSHORT4>;
template class M3DCORE_API List<XMFLOAT4X4>;

}
