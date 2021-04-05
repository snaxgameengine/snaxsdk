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
#include "GlobalDef.h"
#include "M3DCore/CommonTemplates.h"

namespace m3d
{


class HighPrecisionTimer;
class FPS;
struct ChipMessage;
struct EngineImpl;
class Environment;

enum EditMode { EM_RUN, EM_EDIT_RUN, EM_EDIT };

// Accesses the engine after it is created using Engine::Create()
extern Engine M3DENGINE_API *engine;

class M3DENGINE_API Engine
{
	template<typename T> friend void mmdelete(const T*);
	friend struct EngineAccessor;
public:
	// Creates the engine. One of the first things an application do.
	static bool Create();
	// Destroys the engine just before quitting. There is no need for the Application to exist no more! Do NOT call engine after this!
	static void Destroy();

	// Initiates the system. Searches for chips, sets up graphics engine etc.
	bool Init(Application *application, Path chipDir, Path thirdDir, const List<Path> &libDirs);
	// Resets the engine. Makes it ready to load a new project.
	void Reset();
	// Clears the engine. Calls Reset() and destroys graphics. Must be called before Application is destroyed!
	void Clear();

	// Get the application running this thing. Qt editor, viewer, WP-viewer etc.
	Application* GetApplication() const;
	// Returns the graphics engine. Loaded at startup.
	Graphics* GetGraphics();
	// Returns the manager keeping track of documents.
	DocumentManager* GetDocumentManager();
	// Returns the manager keeping track of chips.
	ChipManager* GetChipManager();
	// Returns the manager keeping track of classes.
	ClassManager* GetClassManager();
	// Returns the manager keeping track of function signatures.
	FunctionSignatureManager* GetFunctionSignatureManager();
	// Returns the environment.
	Environment* GetEnvironment();

	// Sets the file to write debug messages to.
	void SetMessageFile(Path p);
	// Adds a new debug message. THREAD SAFE!
	void Message(MessageSeverity severity, String message, ClassID clazzID = InvalidClassID, ChipID chipID = InvalidChipID);
	// Called by a chip when reporting an issue. 
	void ChipMessageAdded(Chip *chip, const ChipMessage &msg);
	// Called to remove a chip message.
	void ChipMessageRemoved(Chip *chip, const ChipMessage &msg);
	
	// Called by application framework once every frame.
	void Run();
	// Current frame number.
	unsigned GetFrameNr() const;
	// Coarse frame timer updated every time Run() is called.
	long GetFrameTime() const;
	// Used for GetFrameTime.
	long GetClockTime() const;
	// Stops the clock timer. (To be used for break points)
	void StopClockTime();
	// Starts the clock timer. (To be used for break points)
	void StartClockTime();
	// Timer ticked every frame.
	const HighPrecisionTimer& GetTimer() const;
	// Gets frame time limited to 2 hz - 10000 hz.
	__int64 GetDt() const;
	// Gets current application time.
	__int64 GetAppTime() const;
	// Called when normal execution is halted, eg when application loses focus, at breakpoints etc.
	void StopAppTime();
	// Starts the app timer again. Called automatically at Run().
	void StartAppTime();
	// Gets framerate statistics.
	const FPS& GetFPS() const;
	// Called when program execution is to be paused (eg for break points)
	void Break(Chip *chip);
	bool IsBreakMode() const;
	// Returns true if Run() is called, but not yet returned.
	bool IsRunning() const;

	// The directory where 3rd-party dependencies are found
	Path GetThirdDepsDirectory() const;
	// Gets the command line arguments set when starting the viewer.
	const List<String>& GetCmdLineArguments() const;
	// Framework sets the command line arguments.
	void SetCmdLineArguements(const List<String>& args);
	// Returns the platform compiled for.
	//AppPlatform GetPlatform() const;

	EditMode GetEditMode() const;
	void SetEditMode(EditMode editMode);

	// This call will be distributed to the system to destroy all device objects. Mostly called by device itself.
	void DestroyDeviceObjects();

private:
	Engine();
	~Engine();

	EngineImpl* _impl;

};




}

