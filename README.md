# SnaX Game Engine SDK
Copyright (c) 2013-2019, mCODE A/S
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

## Introduction
SnaX ([snaxgameengine.com](https://snaxgameengine.com/)) is a fast and lightweight development tool
for 3D-graphics applications based on a real-time, visual programming concept. _Real-time_ because
every update you make to your program can be viewed instantly as you develop - _Visual_ because 
programming happens in a graphical environment, where writing code is replaced by linking together 
small, precompiled blocks of functionality called _Chips_. This SDK enables _you_ to create your own 
Chips, extending SnaX's functionality with whatever you need. It could be physics engines, sound or 
network libraries, physical input devices, complex algorithms and more.

As an example of what you can implement using this SDK, have a look at the 
[PhysXForSnaX](https://github.com/snaxgameengine/PhysXForSnaX) repository, 
which implements the nVidia PhysX 4 library for SnaX.

## Getting started
- Get the Windows SDK. It can be installed using Visual Studio Installer.
- Clone the Git-repo: `git clone https://github.com/snaxgameengine/snaxsdk.git`
- Create two new C++ DLL-projects in __Visual Studio (2015-2019)__; one for the chips, and one optional for the dialogs. 
- Add the SnaXSDK\\Include\\ and SnaXSDK\\Lib\\ paths to the additional include and library paths.
- Add M3DCore.lib, M3DEngine.lib and StdChips.lib as additional library dependencies. 
- For the optional dialog-project, add ChipDialogs.lib and the chips-project as well!
- Create the Exports.h/cpp-files as done in the Example-project. The important part is to set a uniqe preprocessor directive for each project, eg. MYCHIPS_EXPORTS, and update the Exports.h file:

**`Exports.h`**
```cpp 
#ifdef MYCHIPS_EXPORTS
#define MYCHIPS_API __declspec(dllexport)
#else
#define MYCHIPS_API __declspec(dllimport)
#endif

#define PACKET_NAME L"My Chips"
```
- Next is to create a new chip, eg MyChip, by adding a pair of MyChip.h/MyChip.cpp files. Each _Packet_ can contain any number of chips. As a bare minimum you will need the following for each chip:

**`MyChip.h`**
```cpp 
#pragma once

#include "Exports.h"
#include "M3DEngine/Chip.h"

// Define a unique global identifier for this chip. Use a guid-generator to make unique ids!
static const GUID MYCHIP_GUID = { 0xbb7c969, 0x94db, 0x40ce, { 0x86, 0x46, 0x3, 0x7a, 0x3d, 0xf3, 0xc2, 0x58 } };

// Define a new chip deriving from m3d::Chip.
class MYCHIPS_API MyChip : public m3d::Chip
{
	// This macro is needed for all new chips!
	CHIPDESC_DECL;
public:
	MyChip();
	~MyChip();
	
	void CallChip() override;
};
```
**`MyChip.cpp`**
```cpp 
#include "MyChip.h"

// This macro registrates the chip with the engine, sets a name for use 
// in the developer, and defines the run-time type hierarchy.
// There are many variations of this macro, but this is the most common.
CHIPDESCV1_DEF(MyChip, L"My Chip", MYCHIP_GUID, m3d::CHIP_GUID);

MyChip::MyChip()
{
}

MyChip::~MyChip()
{
}

void MyChip::CallChip()
{
	// Do your thing here!
}
```
The Important parts are to set a unique MYCHIP_GUID, add the CHIPDESC_DECL to the class definition, derive from the correct type of chip, 
and fill out the CHIPDESCV1_DEF macro in the cpp file. The `CallChip()` function is called when the Chip is _called_ in the game. 
This is where you will implement the chip's main functionality. Of course, there will be other functions as well for other types of chips.
There are also functions for (de)serializing (load/save) data in the chip.

Currently, you will be able to derive from these eight type of chips. It is important to set the correct base-GUID based on the type
of chip you derive from. You can also create your own type hierarchy by deriving from chips you already created!

| Base type           | Base-GUID                | Package   | Comment                                                |
| ---------------     | ------------------------ | --------  | ------------------------------------------------------ |
| `m3d::Chip`         | `m3d::CHIP_GUID`         | M3DEngine | Base type for all type of chips                        |
| `m3d::Text`         | `m3d::TEXT_GUID`         | StdChips  | Represents a piece of text.                            |
| `m3d::Value`        | `m3d::VALUE_GUID`        | StdChips  | Represents a floating point value (double).            |
| `m3d::Vector`       | `m3d::VECTOR_GUID`       | StdChips  | Represents a 4-component vector (float).               |
| `m3d::MatrixChip`   | `m3d::MATRIXCHIP_GUID`   | StdChips  | Represents a 4x4-component matrix (float).             |
| `m3d::ValueArray`   | `m3d::VALUEARRAY_GUID`   | StdChips  | Represents an array of floating point values (double). |
| `m3d::VectorArray`  | `m3d::VECTORARRAY_GUID`  | StdChips  | Represents an array of 4-component vectors.            |
| `m3d::MatrixArray`  | `m3d::MATRIXARRAY_GUID`  | StdChips  | Represents an array of 4x4-component matrices.         |

- Plese refer to the Example for how to set up the dialog project!
- Build to solution!   
- Copy the resulting DLL-files for the chips and the optional dialogs to the Chips\\ and Dialogs\\ directories in the SnaX install directory, typically "C:\\Program Files\\SnaX\\".
- Start SnaX Developer. Your new chips should be visible in the list of available chips. 

__TIP__: You can run SnaX Developer from Visual Studio to debug your code by setting SnaXDeveloper.exe as the _Debugger Command_!

## Running the example-project
The _Examples_ folder contains a project that can be opened and built using Visual Studio 2015-2019.
The post-build step will copy the new DLL-files to the correct folders in the SnaX installation directory,
assuming SnaX is installed to "C:\\Program Files\\SnaX\\". If SnaX is installed somewhere else, you will
need to update the post-build steps.

__IMPORTANT: You have to give the current user _write access_ to the _3rd_, _Chips_ and _Dialogs_ folders in the
SnaX installation directory. If not, Visual Studio will not be able to automatically copy the new files.__

For a more complete example for how to use the SDK, please have a look at the [PhysXForSnaX](https://github.com/snaxgameengine/PhysXForSnaX) repository!
