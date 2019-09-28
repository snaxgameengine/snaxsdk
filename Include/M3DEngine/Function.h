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

namespace m3d
{

class Parameter;

struct ParameterConnection
{
	Parameter *p; 
	unsigned c;
	ParameterConnection() : p(0), c(-1) {}
	ParameterConnection(Parameter *p, unsigned c = -1) : p(p), c(c) {}
	bool operator<(const ParameterConnection &rhs) const { return p < rhs.p && c != rhs.c; }
	bool operator==(const ParameterConnection &rhs) const { return p == rhs.p && c == rhs.c; }
	bool operator!=(const ParameterConnection& rhs) const { return !(*this == rhs); }
};

typedef Set<ParameterConnection> ParameterConnectionSet;
template class M3DENGINE_API Set<ParameterConnection>;

typedef Set<FunctionCall*> FunctionCallSet;
template class M3DENGINE_API Set<FunctionCall*>;

class Chip;

class M3DENGINE_API Function
{
public:
	enum Type { STATIC, NONVIRTUAL, VIRTUAL };
	enum Access { PUBLIC, PROTECTED, PRIVATE };

protected:
	Chip *_chip;
	Function *_vFunction;

	Type _type;
	Access _access;
	ParameterConnectionSet _parameters;
	FunctionSignatureID _signature;
	FunctionCallSet _functionCalls;

	// For performance monitoring
	__int64 _callTime;
	__int64 _callTimeExclSubFunc;
	unsigned _lastFrame;
	unsigned _hitCount;
	unsigned _cppHitCount;

public:
	Function(Chip *chip);
	~Function();

	Chip *GetChip() const { return _chip; }

	Type GetType() const { return _type; }
	Access GetAccess() const { return _access; }
	const ParameterConnectionSet &GetParameters() const { return _parameters; }
	unsigned GetParameterIndex(Parameter *param);
	Parameter *GetParameterFromIndex(unsigned index);
	FunctionSignatureID GetSignature() const { return _signature; }
	const FunctionCallSet &GetFunctionCalls() const { return _functionCalls; }

	void Set(const String &name, Type type, Access access, const ParameterConnectionSet &parameters);
	void Set(const String &name);
	void RemoveParameter(Parameter *param);

	void RegisterFunctionCall(FunctionCall *fc);
	void UnregisterFunctionCall(FunctionCall *fc);

	// For performance monitoring
	void AddCallTime(__int64 callTime, __int64 callTimeExclSubFunc, unsigned ccpHitCount);
	inline __int64 GetCallTime() const { return _callTime; }
	inline __int64 GetCallTimeExclSubFunc() const { return _callTimeExclSubFunc; }
	inline unsigned GetLastFrame() const { return _lastFrame; }
	inline unsigned GetHitCount() const { return _hitCount; }
	inline unsigned GetCPPHitCount() const { return _cppHitCount; }

};



}