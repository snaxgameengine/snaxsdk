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

#include "MemoryManager.h"
#include <type_traits>

namespace m3d
{


template<typename T>
class List
{
private:
	T *_t;
	size_t _capacity;
	size_t _size;

	void _reserve(size_t capacity)
	{
		if (capacity == _capacity)
			return;
		T *t = _t;
		size_t size = _size;
		_t = nullptr;
		_size = 0;
		_capacity = capacity;
		if (_capacity) {
			_t = (T*)mmalloc(_capacity * sizeof(T));
			_add(_capacity < size ? _capacity : size, t);
		}
		if (!std::is_trivially_destructible<T>::value) // Need to call destructors?
			while (size)
				t[--size].~T();
		mmfree(t);
	}

	void _ensure_capacity(size_t size)
	{
		if (size > _capacity)
			_reserve((_capacity * 2) > size ? (_capacity * 2) : size);
	}

	void _add(size_t count, const T &t)
	{
		_ensure_capacity(_size + count);
		for (size_t i = 0; i < count; i++)
			_add(t);
	}

	void _add(size_t count, const T *t)
	{
		_ensure_capacity(_size + count);
		if (std::is_trivially_copyable<T>::value) { // Can do with simple copy?
			memcpy(_t + _size, t, sizeof(T) * count);
			_size += count;
		}
		else
			for (size_t i = 0; i < count; i++)
				_add(t[i]);
	}

	inline void _add(const T &t) { new (_t + _size++) T(t); } // placement new (no memory allocated)!
	inline void _set(size_t at, const T &t) { new (_t + at) T(t); } // placement new (no memory allocated)!

	void _erase(size_t count, bool realloc)
	{
		if (std::is_trivially_destructible<T>::value) // Need to call destructors?
			_size -= count;
		else
			while (count-- > 0)
				_t[--_size].~T();
		if (realloc && _capacity > _size * 4)
			_reserve(_size * 2);
	}

	void _clear(bool realloc)
	{
		if (std::is_trivially_destructible<T>::value) // Need to call destructors?
			_size = 0;
		else
			while (_size)
				_t[--_size].~T();
		if (realloc) {
			mmfree(_t);
			_capacity = 0;
			_t = nullptr;
		}
	}

public:
	List() : _t(nullptr), _capacity(0), _size(0) 
	{
	}
	
	List(const List &list) : _t(nullptr), _capacity(0), _size(0) 
	{
		*this = list;
	}

	List(List &&list) : _t(list._t), _capacity(list._capacity), _size(list._size) 
	{
		list._t = nullptr;
		list._capacity = 0;
		list._size = 0;
	}

	List(const std::initializer_list<T> &v)
	{
		reserve(v.size());
		for (auto itr = v.begin(); itr != v.end(); itr++)
			push_back(*itr);
	}

	explicit List(size_t capacity) : _t(nullptr), _capacity(0), _size(0)
	{
		_reserve(capacity);
	}

	List(size_t size, const T t) : _t(nullptr), _capacity(0), _size(0)
	{
		_add(size, t);
	}

	List(size_t size, const T *t) : _t(nullptr), _capacity(0), _size(0)
	{
		_add(size, t);
	}
		
	~List()
	{
		_clear(true);
	}

	List &operator=(const List &list)
	{
		if (this != &list) {
			_clear(false);
			_reserve(list._capacity);
			_add(list._size, list._t);
		}
		return *this;
	}

	List &operator=(List &&list)
	{
		if (this != &list) {
			_clear(true);
			_t = list._t;
			_capacity = list._capacity;
			_size = list._size;
			list._t = nullptr;
			list._capacity = 0;
			list._size = 0;
		}
		return *this;
	}


	inline size_t capacity() const { return _capacity; }
	inline size_t size() const { return _size; }
	inline bool empty() const { return _size == 0; }

	inline void clear(bool realloc = false) { _clear(realloc); }

	inline T &at(size_t index) { return _t[index]; }
	inline const T &at(size_t index) const { return _t[index]; }

	inline T &operator[](size_t index) { return _t[index]; }
	inline const T &operator[](size_t index) const { return _t[index]; }

	inline T &front() { return _t[0]; }
	inline const T &front() const { return _t[0]; }
	inline T &back() { return _t[_size - 1]; }
	inline const T &back() const { return _t[_size - 1]; }

	inline void reserve(size_t capacity) { _reserve(capacity); }

	inline void push_back(const T &t) { _add(1, t); }
	inline void pop_back() { _erase(1, false); }

	inline void append(size_t size, const T *t) { _add(size, t); }
	inline void append(const List &list) { _add(list._size, list._t); }
	
	inline void insert(size_t index, const T &t) { insert(index, 1, t); }

	void insert(size_t index, size_t n, const T &t)
	{
		if (index == _size)
			return resize(_size + n, t);

		_ensure_capacity(_size + n);

		for (size_t i = _size; i > index; i--) {
			_set(i + n - 1, _t[i - 1]);
			_t[i - 1].~T();
		}

		for (size_t i = 0; i < n; i++)
			_set(index + i, t);

		_size += n;
	}

	inline void erase(size_t index) { erase(index, 1); }

	void erase(size_t from, size_t count)
	{
		for (size_t i = 0, j = _size - from - count; i < j; i++) {
			_t[from + i].~T();
			new (_t + from + i) T(_t[from + count + i]);
		}
		_erase(count, false);
	}

	void resize(size_t size, const T t = T())
	{
		if (size > _size)
			_add(size - _size, t);
		else if (size < _size)
			_erase(_size - size, false);
	}

	void shift(size_t index, size_t count, int npos) {
		if (npos < 0) { // move up
			for (size_t j = 0, n = -npos; j < n; j++)
				for (size_t i = 0; i < count; i++)
					std::swap(_t[index + i - j - 1], _t[index + i - j]);
		}
		else { // move down
			for (size_t j = 0, n = npos; j < n; j++)
				for (size_t i = 0; i < count; i++)
					std::swap(_t[count + index + j - i - 1], _t[count + index + j - i]);
		}
	}
};


template<typename T>
bool operator==(const List<T>& a, const List<T>& b)
{
	if (&a == &b)
		return true;
	if (a.size() != b.size())
		return false;
	if (a.empty())
		return true;
	if (std::is_pod<T>::value) // Plain old data type?
		return memcmp(&a.front(), &b.front(), sizeof(T) * a.size()) == 0;
	for (size_t i = 0; i < a.size(); i++)
		if (!(a[i] == b[i]))
			return false;
	return true;
}

template<typename T>
inline bool operator!=(const List<T>& a, const List<T>& b)
{
	if (&a == &b)
		return false;
	if (a.size() != b.size())
		return true;
	if (a.empty())
		return false;
	if (std::is_pod<T>::value) // Plain old data type?
		return memcmp(&a.front(), &b.front(), sizeof(T) * a.size()) != 0;
	for (size_t i = 0; i < a.size(); i++)
		if (!(a[i] != b[i]))
			return true;
	return false;
}

}

