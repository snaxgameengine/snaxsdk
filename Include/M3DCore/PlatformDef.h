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

#include <winapifamily.h>

namespace m3d
{


#define PLATFORM_undefined			0x0000
#define PLATFORM_WINDESKTOP_X64		0x0001 
#define PLATFORM_WINDESKTOP_X86		0x0002 
#define PLATFORM_WINDESKTOP			0x0003 // FAMILY
#define PLATFORM_WINSTOREAPP_X64	0x0004 
#define PLATFORM_WINSTOREAPP_X86	0x0008 
#define PLATFORM_WINSTOREAPP_ARM	0x0010
#define PLATFORM_WINSTOREAPP		0x001C // FAMILY
#define PLATFORM_WINPHONE8_WIN32	0x0020
#define PLATFORM_WINPHONE8_ARM		0x0040
#define PLATFORM_WINPHONE8			0x0060 // FAMILY
#define PLATFORM_all_platforms		0x007F

/*
enum AppPlatform 
{ 
	PLATFORM_undefined =		0x0000,
	PLATFORM_WINDESKTOP_X64 =	0x0001, 
	PLATFORM_WINDESKTOP_X86 =	0x0002, 
	PLATFORM_WINDESKTOP =		0x0003, // FAMILY
	PLATFORM_WINSTOREAPP_X64 =	0x0004, 
	PLATFORM_WINSTOREAPP_X86 =	0x0008, 
	PLATFORM_WINSTOREAPP_ARM =	0x0010,
	PLATFORM_WINSTOREAPP =		0x001C, // FAMILY
	PLATFORM_WINPHONE8_WIN32 =	0x0020,
	PLATFORM_WINPHONE8_ARM =	0x0040,
	PLATFORM_WINPHONE8 =		0x0060, // FAMILY
	PLATFORM_all_platforms =	0x007F
};
*/
/*
enum _PlatformFamily
{
	PLATFORM_FAMILY_undefined =		0x0000,
	PLATFORM_FAMILY_WINDESKTOP =	PLATFORM_WINDESKTOP_X64|PLATFORM_WINDESKTOP_X86,
	PLATFORM_FAMILY_WINSTOREAPP =	PLATFORM_WINSTOREAPP_X64|PLATFORM_WINSTOREAPP_X86|PLATFORM_WINSTOREAPP_ARM,
	PLATFORM_FAMILY_WINPHONE8 =		PLATFORM_WINPHONE8_WIN32|PLATFORM_WINPHONE8_ARM
};
*/
}



// PLATFORM_FAMILY will always be defined for current platform family.
// PLATFORM will be defined for the current platform.
// WINDESKTOP will be defined for desktop applications.
// WINPHONE8 will be defined for WP8 apps.
// WINSTOREAPP will be defined for store apps.


#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
	#define PLATFORM_FAMILY PLATFORM_WINDESKTOP
	#define WINDESKTOP
	#ifdef _WIN64
		#define PLATFORM PLATFORM_WINDESKTOP_X64
	#else
		#define PLATFORM PLATFORM_WINDESKTOP_X86
	#endif
#elif defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	#define PLATFORM_FAMILY PLATFORM_WINPHONE8
	#define WINPHONE8
	#ifdef _M_ARM
		#define PLATFORM PLATFORM_WINPHONE8_ARM
	#else 
		#define PLATFORM PLATFORM_WINPHONE8_WIN32
	#endif
#else
	#define PLATFORM_FAMILY PLATFORM_WINSTOREAPP
	#define WINSTOREAPP
	#if _M_ARM
		#define PLATFORM PLATFORM_WINSTOREAPP_ARM
	#elif _M_X64
		#define PLATFORM PLATFORM_WINSTOREAPP_X64
	#else
		#define PLATFORM PLATFORM_WINSTOREAPP_X86
	#endif
#endif