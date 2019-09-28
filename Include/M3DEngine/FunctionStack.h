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
#include "ChipDef.h"


// MAx size of the function call stack
#define FUNCTION_STACK_SIZE 1024 


namespace m3d
{

class Chip;
class FunctionCall;
class Function;

template<typename T>
class ChildPtr;

typedef ChildPtr<Chip> ChipChildPtr;


// This is for debugging.
struct FunctionStackTraceRecord
{
	Chip *caller; // Function call or parameter chip. nullptr for first record only.
	Chip *called; // Function or connected parameter. Can also be project start chip for first record. Not nullptr!
	ClassInstance *instance; // Current instance. Can be nullptr if static function or first record.

	FunctionStackTraceRecord(Chip *caller = nullptr, Chip *called = nullptr, ClassInstance *instance = nullptr) : caller(caller), called(called), instance(instance) {}
};

typedef List<FunctionStackTraceRecord> FunctionStackTrace;

struct FunctionStackRecord;

class M3DENGINE_API FunctionStack
{
public:
	enum PerfMon { PERF_NONE, PERF_FRAME, PERF_ACCUM };
private:
	// Counter incremented for each new record added to stack. Each record will get its unique id from this.
	unsigned _recordnrs;
	// Index into _functionStack for current record.
	unsigned _stackptr;
	// Index into top of the stack.
	unsigned _stackEnd;
	// The maximum reference count within a record. To prevent infinite loops!
	unsigned _refLimit;
	// The function stack.
	FunctionStackRecord *_functionStack; // I'm using a plain c-array for maximum performance!

	// For performance monitoring!
	PerfMon _perfMon;
	unsigned _perfFrame;
	unsigned _perfFrameCount;
	__int64 _perfTime;
	__int64 _qFreq;
	unsigned _perfCPPHitCount;

	void _popStack();

public:
	FunctionStack();
	~FunctionStack();

	// Check if stack overflow. If it returns false, no function call or parameter call can be made!
	inline bool CanAddRecord() const { return _stackEnd < FUNCTION_STACK_SIZE - 1; }
	// Adds a new function-call record on top of the stack.
	// Returns it's stackptr. The function does not change the current stackptr.
	unsigned AddFunctionCallRecord(FunctionCall *functionCall, Function *function, ClassInstance *instance);
	// Check for infinit loops within a function. 
	// NOTE: When _stackptr is 0, refcount will be -1 if outside a frame. This will effectively stop calls to GetChild(...) from dialog pages in the editor.
	bool CanIncrementRecordRef() const;
	// Set ref count limit.
	void SetReferenceCountLimit(unsigned l) { _refLimit = l; }
	// Get the ref count limit.
	unsigned GetReferenceCountLimit() const { return _refLimit; }
	// Are we between calls to StartOfFrame() and EndOfFrame()?
	bool IsRunning() const { return _stackEnd != -1; }
	// Increments the refcount of the record at the given stackptr.
	// Returns the given stackptr.
	unsigned IncrementRecordRef(unsigned stackptr);
	inline unsigned IncrementRecordRef() { return IncrementRecordRef(_stackptr); }
	// Adds a new parameter-call record on top of the stack.
	// Returns it's stackptr. The function does not change the current stackptr.
	unsigned AddParameterCallRecord(Parameter *p, Chip *lp);
	// Decrements the refcount on stackptr. If it's on top of the stack, and the counter
	// is 0 then the stack is poped recursivly as long as the top refcount is 0.
	void DecrementRecordRef(unsigned stackptr);
	// Sets the current stackptr. Returns the old stackptr. Called from ChildPtr::Call::Call(), this is, before a method call (eg CallChip()).
	unsigned SetStackPtr(unsigned stackptr);
	// Sets the current stackptr. Called from ChildPtr::Call::~Call(), that is, after a method call (eg CallChip()).
	void ResetStackPtr(unsigned stackptr);
	// Returns the current stack frame. NOTE: It always returns a function-call record.
	const FunctionStackRecord& GetCurrentRecord() const;
	// Gets data from current record.
	Chip *GetData(unsigned functionDataID);
	// Sets data on current record. Assumes GetData() is called and cid does not already exist (avoids these checks for performance)!
	void SetData(unsigned functionDataID, Chip *chip);
	// Adds a parameter to the parameter map of current record. (Parameter caching!)
	void SetParameter(unsigned index, const ChipChildPtr &p);
	// Called at the start of each frame
	void StartOfFrame();
	// Called at end of frame to validate stack and prepare new frame! Return false if stack corruption!
	bool EndOfFrame();
	// Dumps current stack trace. For debugging!
	void DumpStackTrace(FunctionStackTrace &trace);

	// Returns type of performance monitoring enabled
	inline PerfMon GetPrefMon() const { return _perfMon; }
	// Sets if performance monitoring should be enabled!
	inline void SetPerfMon(PerfMon perfMon) { _perfMon = perfMon; }
	// Only for PERF_ACCUM: On next frame, counters are reset. Should NOT be called between StartOfFrame() and EndOfFrame()!
	void ResetPerfFrame();
	// For PERF_FRAME: Incremented every frame. 
	// For PERF_ACCUM: Only incremented at ResetPerfAccum().
	inline unsigned GetPerfFrame() const { return _perfFrame; }
	// Returns the number of frame values are accumulated from. Should be 1 for PERF_FRAME!
	inline unsigned GetPerfFrameCount() const { return _perfFrameCount; }
	// if PERF_FRAME: Returns the time the last frame took (time between call to StartOfFrame() and EndOfFrame())
	// If PERF_ACCUM: Same as above, but values are not reset every frame 
	inline __int64 GetPerfTime() const { return _perfTime; }
	// The number of ChildPtr-> done last frame.
	inline unsigned GetPerfCPPCount() const { return _perfCPPHitCount; }
	// Returns the system query freq. To get time in seconds use: (stop-end)/GetQFreq()
	inline __int64 GetQFreq() const { return _qFreq; }

};

extern FunctionStack M3DENGINE_API functionStack;

}