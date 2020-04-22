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

namespace m3d
{

	// mode 0: Only message
	// mode 1: Message box is shown
	// mode 2: DebugBreak() is always called.
	HRESULT M3DENGINE_API Trace(const char *file, unsigned long line, HRESULT hr, const wchar_t *msg, int mode = 0); // Returns hr
	bool M3DENGINE_API Trace(const char *file, unsigned long line, bool b, const wchar_t *msg, int mode = 0); // returns b

}




#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = x; if( FAILED(hr) ) { m3d::Trace( __FILE__, (unsigned long)__LINE__, hr, L#x, 1 ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return m3d::Trace( __FILE__, (unsigned long)__LINE__, hr, L#x, 1 ); } }
#ifndef B_RETURN
#define B_RETURN(x)	   { if ( !(x) ) { return m3d::Trace( __FILE__, (unsigned long)__LINE__, false, L#x, 1 ); } }
#endif
#endif
#else
#ifndef V
#define V(x)           { hr = x; if( FAILED(hr) ) { m3d::Trace( __FILE__, (unsigned long)__LINE__, hr, nullptr, 0 ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = x; if( FAILED(hr) ) { return m3d::Trace( __FILE__, (unsigned long)__LINE__, hr, nullptr, 0 ); } }
#ifndef B_RETURN
#define B_RETURN(x)	   { if ( !(x) ) { return m3d::Trace( __FILE__, (unsigned long)__LINE__, false, nullptr, 0 ); } }
#endif
#endif
#endif
