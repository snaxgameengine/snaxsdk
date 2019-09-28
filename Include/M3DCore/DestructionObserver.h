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
#include "Set.h"


namespace m3d
{

class DestructionObservable;

template class M3DCORE_API Set<DestructionObservable*>;

class M3DCORE_API DestructionObserver
{
	friend class DestructionObservable;
private:
	Set<DestructionObservable*> _observed;

protected:
	DestructionObserver();
	virtual ~DestructionObserver();

	virtual void OnDestruction(DestructionObservable *observable) = 0;
};

template class M3DCORE_API Set<DestructionObserver*>;

class M3DCORE_API DestructionObservable
{
	friend class DestructionObserver;
private:
	Set<DestructionObserver*> _observers;

protected:
	DestructionObservable();
	virtual ~DestructionObservable();

	void NotifyDestuction();

public:
	void AddDestructionObserver(DestructionObserver *obs);
	void RemoveDestructionObserver(DestructionObserver *obs);

	const Set<DestructionObserver*> &GetDestructionObservers() const { return _observers; }
};

}