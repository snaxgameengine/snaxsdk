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
#include "CriticalSection.h"

namespace m3d
{

class M3DCORE_API MemStack
{
private:
	unsigned char **_stack;
	unsigned _stackSize;
	unsigned _size;
	unsigned _capacity;
	size_t _fragmentSize;
	bool _allocFailed;

public:
	MemStack();
	~MemStack();

	unsigned getSize() const { return _size; }
	unsigned getCapacity() const { return _capacity; }

	void setFragmentSize(size_t fs) { _fragmentSize = fs; }
	size_t getFragmentSize() const { return _fragmentSize; }

	bool getAllocFailed() const { return _allocFailed; }
	void setAllocFailed(bool b) { _allocFailed = b; }

	unsigned char *pop();
	void push(unsigned char *mem);
	bool pushNew(unsigned char *mem);
};

class M3DCORE_API MemBlock
{
private:
	unsigned char *_mem;
	size_t _size;
	size_t _free;

public:
	MemBlock();
	~MemBlock();

	size_t getSize() const { return _size; }
	size_t getFree() const { return _free; }

	bool init(size_t size);
	void release();

	unsigned fill(MemStack &ms, unsigned maxCount, unsigned char stackID);

};

struct MemStat;

class M3DCORE_API MemoryManager
{
public:
	static const size_t MAX_MEMORY = 1024; // do not set to high. The memory manager is ment to speed up relatively small (and frequent!) allocations!
	static const unsigned char STACK_COUNT = 64; // max 255  (1024/64=16 byte steps.. you waste max 15 bytes pr allocation. average 7-8 bytes?)
	static const size_t ALIGNMENT = 16; // Got to be power of 2!
private:
	MemStack _stack[STACK_COUNT];
	MemBlock _block[256];
	size_t _blockSize;
	unsigned char _stackMap[MAX_MEMORY];

	CriticalSection _cs;

	MemStat *_stat;

	void *_getCustomMemory(size_t size);

public:
	MemoryManager(size_t blockSize = 1024 * 1024);
	~MemoryManager();
	
	void *get(size_t size); // Every allocation adds 1 extra byte of metadata in front of the returned memory address! Memory is guarantied ALIGNMENT memory alignement!
	void free(void *mem);
	void *getDebug(size_t size, const char *file, int line); // Debug version. Use freeDebug() for memory allocated with this! This method add several bytes of meta data, but guaranties alignment.
	void freeDebug(void *mem); // Debug version
	void *realloc(void *mem, size_t size);
	void *reallocDebug(void *mem, size_t size, const char *file, int line);
};


extern M3DCORE_API MemoryManager &mm(); // Global memory manager

#if defined(DEBUG) | defined(_DEBUG)
	#define mmnew new //new(mm(), __FILE__, __LINE__)

	template<typename T> void mmdelete(const T* p)
	{
		//p->~T();
		//mm().freeDebug((T*)p);
		delete p;
	}
#else
	// DO NOT USE mmnew for arrays!!!
	#define mmnew new // new(mm())

	// DO NOT USE mmdelete for arrays!!!
	template<typename T> void mmdelete(const T* p)
	{
		//p->~T();
		//mm().free((T*)p);
		delete p;
	}
#endif



#if defined(DEBUG) | defined(_DEBUG)
	#define mmalloc(size) mm().getDebug(size, __FILE__, __LINE__)
	#define mmfree(pMem) mm().freeDebug(pMem);
	#define mrealloc(pMem, size) mm().reallocDebug(pMem, size, __FILE__, __LINE__)
#else
	#define mmalloc(size) mm().get(size)
	#define mmfree(pMem) mm().free(pMem);
	#define mrealloc(pMem, size) mm().realloc(pMem, size)
#endif

}
/*
static void* operator new(size_t size, m3d::MemoryManager& memManager)
{
	return memManager.get(size);
}

static void* operator new(size_t size, m3d::MemoryManager& memManager, const char *file, int line) // Debug version
{
	return memManager.getDebug(size, file, line);
}

static void operator delete(void *mem, m3d::MemoryManager &memManager)
{
	return memManager.free(mem);
}

static void operator delete(void *mem, m3d::MemoryManager &memManager, const char *file, int line) // Debug version
{
	return memManager.freeDebug(mem);
}
*/

/*
template<typename T> void mmdeletea(T* p, MemoryManager &memManager = mm)
{
//	unsigned char *m = (unsigned char*)p - sizeof(size_t);
//	while ((size_t&)*m > 0)
//		p[--(size_t&)*m].~T();
//	memManager.freeMemory(m);
//	operator delete[](p, mm);
}

static void* operator new[](size_t size, m3d::MemoryManager& memManager)
{
//	void *m = memManager.getMemory(size + sizeof(size_t));
//	*((size_t*)m) = size;
//	return (unsigned char*)m + sizeof(size_t);
	return memManager.getMemory(size); // the run-time adds sizeof(size_t) bytes of data to the front of the data returned. this contains size. Is this always a safe assumtion?
}
*/
//static void operator delete[](void *mem, m3d::MemoryManager &memManager)
//{
//	return memManager.freeMemory((unsigned char*)mem);// - sizeof(size_t));
//}
