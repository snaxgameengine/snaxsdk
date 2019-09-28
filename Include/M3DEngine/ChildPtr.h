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

#include "FunctionStack.h"
#include "Function.h"

namespace m3d
{

class Parameter;
class ClassInstance;
class ShellChip;

template<typename T>
class ChildPtr
{
	template<typename S> friend class ChildPtr;
private:
	T* t;
	//	const Chip *childProvider; // <= 2/5/12: Highly experimental stuff for the idea with the Shell-Chip...
	unsigned stackptr;

public:
	ChildPtr() : t(nullptr)/*, childProvider(nullptr)*/, stackptr(-1) {}
	ChildPtr(const ChildPtr& s) : t(s.t)/*, childProvider(s.childProvider)*/, stackptr(functionStack.IncrementRecordRef(s.stackptr)) {}
	ChildPtr(T* t) : t(t)/*, childProvider(t ? t->GetChildProvider() : nullptr)*/, stackptr(functionStack.IncrementRecordRef()) {}
	ChildPtr(Function* f, FunctionCall* functionCall, ClassInstance* instance = nullptr) : t(f->GetChip())/*, childProvider(t ? t->GetChildProvider() : nullptr)*/, stackptr(functionStack.AddFunctionCallRecord(functionCall, f, instance)) {}
	ChildPtr(T* t, Parameter* p) : t(t)/*, childProvider(t ? t->GetChildProvider() : nullptr)*/, stackptr(functionStack.AddParameterCallRecord(p, t)) {}
	ChildPtr(T* t, ShellChip* shell) : t(t)/*, childProvider(shell)*/, stackptr(functionStack.IncrementRecordRef()) {}
	template<typename S>
	ChildPtr(const ChildPtr<S>& s) : t(dynamic_cast<T*>(s.t))/*, childProvider(t ? s.childProvider : nullptr)*/, stackptr(functionStack.IncrementRecordRef(s.stackptr)) {}
	~ChildPtr() { functionStack.DecrementRecordRef(stackptr); }

	ChildPtr& operator=(const ChildPtr& rhs)
	{
		functionStack.DecrementRecordRef(stackptr);
		t = rhs.t;
		//childProvider = rhs.childProvider;
		stackptr = functionStack.IncrementRecordRef(rhs.stackptr); // Changed 25/7/12 because I think the previous assignment was wrong: functionStack.IncrementRecordRef();
		return *this;
	}

	template<typename S>
	ChildPtr& operator=(const ChildPtr<S>& rhs)
	{
		functionStack.DecrementRecordRef(stackptr);
		t = dynamic_cast<T*>(rhs.t);
		//childProvider = t ? rhs.childProvider : nullptr;
		stackptr = functionStack.IncrementRecordRef(rhs.stackptr); // Changed 25/7/12 because I think the previous assignment was wrong: functionStack.IncrementRecordRef();
		return *this;
	}

	operator bool() const { return t != nullptr; }
	bool operator==(T* s) const { return t == s; }
	bool operator!=(T* s) const { return t != s; }

	template<typename S>
	bool operator==(const ChildPtr<S>& s) { return t == s.t; }
	template<typename S>
	bool operator!=(const ChildPtr<S>& s) { return t != s.t; }

	// For explanation about this technique, see [Stroustrup]: Wrapping C++ Member Function Calls
	class Call
	{
	public:
		T* const t;
		//const Chip *const oldChildProvider;
		const unsigned oldstackptr;

		Call(T* const t/*, const Chip *const childProvider*/, unsigned stackptr) : t(t)/*, oldChildProvider(t->ReplaceChildProvider(childProvider))*/, oldstackptr(functionStack.SetStackPtr(stackptr))
		{
			// Enter function call! Called right before ANY function call on child (eg 'someChild->CallChip()') is executed. 
			// Adjust the function stack frame.
		}

		~Call()
		{
			// Leave function call! Called right after a function call on child has returned!
			// Adjust the stack back!
			//t->SetChildProvider(oldChildProvider);
			//functionStack.SetStackPtr(oldstackptr);
			functionStack.ResetStackPtr(oldstackptr);
		}

		T* operator->() const { return t; }
	};


	// NOTE, IMPORTANT:
	// 1) You should NEVER do a function call (B) on an object returned by a function call (A) (using ->) on ChildPtr.
	//    Example:
	//    ChildPtr<SomeChip> ch0 = GetChild(0);
	//    ch0->SomeFunction()->SomeOtherFunction(); // DO NOT DO THIS!
	//    Do this instead:
	//    SomeObject *obj = ch0->SomeFunction();
	//    obj->SomeOtherFunction();
	// 2) Do NOT use the result from a function call (A) (using ->) on ChildPtr as a parameter to another function (B).
	//    Example:
	//    ChildPtr<SomeChip> ch0 = GetChild(0);
	//    SomeFunction(ch0->SomeOtherFunction()); // DO NOT DO THIS!
	//    Do this instead:
	//    int result = ch0->SomeOtherFunction();
	//    SomeFunction(result);
	// 3) Also, is you violate 2), absolutely do NOT do this for multiple parameters or if function (B) is also a function call on a ChildPtr.
	const Call operator->() const { return Call(t/*, childProvider*/, stackptr); }
};

typedef ChildPtr<Chip> ChipChildPtr;

template class M3DENGINE_API ChildPtr<Chip>;

}