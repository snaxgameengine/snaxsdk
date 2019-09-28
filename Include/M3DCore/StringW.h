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
#include "MLocale.h"
#include <atomic>

namespace m3d
{

class StringA;

class M3DCORE_API StringW
{
private:
	struct _sharedstr
	{
		std::atomic_uint32_t refs;
		size_t size;
		const size_t capacity;
#pragma warning(push)
#pragma warning (disable : 4200 )
		wchar_t str[]; // This is not standard, but but... :)
#pragma warning(pop)
		_sharedstr(size_t size, size_t capacity) : refs(1), size(size), capacity(capacity) {}
	} *_s;

	void _incr();
	void _decr();

	_sharedstr *_allocate(size_t size, size_t capacity);

	wchar_t *_str();

	StringW(size_t size, size_t capacity);

public:
	static const size_t npos = -1;

	StringW(const wchar_t *str = L"", size_t maxChar = npos);

	StringW(const StringW &str);

	explicit StringW(wchar_t c);

	~StringW();

	StringW &operator=(const StringW &rhs) { return assign(rhs); }

	StringW &operator+=(const StringW &str);

	StringW &assign(const wchar_t *str, size_t maxChar = npos);

	StringW &assign(const StringW &str);

	StringW &append(const StringW &str, size_t maxChar = npos) { return *this += str.substr(0, maxChar); }

	StringW &erase(size_t offset = 0, size_t count = npos);

	void clear() { if (size()) *this = L""; }

	void reserve(size_t capacity);

	bool operator<(const StringW &rhs) const { return compare(rhs) < 0; };

	bool operator==(const StringW &str) const;

	bool operator!=(const StringW &str) const { return !(*this == str); }

	StringW operator+(const StringW &str) const;

	inline wchar_t &operator[](size_t index) { return _str()[index]; }
	inline const wchar_t &operator[](size_t index) const { return c_str()[index]; }
 
	inline const wchar_t *c_str() const { return _s->str; }

	inline size_t length() const { return _s->size; }
	inline size_t size() const { return _s->size; }
	inline size_t capacity() const { return _s->capacity; }
	inline bool empty() const { return _s->size == 0; }

	int compare(const StringW &rhs) const;
	int compareNoCase(const StringW &rhs, const Locale &locale = Locale::DEFAULT) const;

	size_t find(const StringW &str, size_t offset = 0) const;
	size_t rfind(const StringW &str, size_t offset = npos) const;

	size_t find_first_of(wchar_t t, size_t offset = 0) const;
	size_t find_first_of(const StringW &str, size_t offset = 0) const;

	size_t find_last_of(wchar_t t, size_t offset = npos) const;
	size_t find_last_of(const StringW &str, size_t offset = npos) const;

	size_t find_first_not_of(wchar_t t, size_t offset = 0) const;
	size_t find_first_not_of(const StringW &str, size_t offset = 0) const;

	size_t find_last_not_of(wchar_t t, size_t offset = npos) const;
	size_t find_last_not_of(const StringW &str, size_t offset = npos) const;

	StringW substr(size_t offset = 0, size_t count = npos) const;

	StringW trimmed(wchar_t t = L' ') const;

	StringW replace(const StringW &replaceThis, const StringW &withThis) const;
	StringW replace(const wchar_t replaceThis, const wchar_t withThis) const;

	StringW to_lower(const Locale &locale = Locale::DEFAULT) const;
	StringW to_upper(const Locale &locale = Locale::DEFAULT) const;

	enum Base { OCT, DEC, HEX };

	bool toNum(float &v, Base base = DEC) const; // handles nan, +inf, -inf
	bool toNum(double &v, Base base = DEC) const; // handles nan, +inf, -inf
	bool toNum(short &v, Base base = DEC) const;
	bool toNum(unsigned short &v, Base base = DEC) const;
	bool toNum(int &v, Base base = DEC) const;
	bool toNum(unsigned int &v, Base base = DEC) const;
	bool toNum(long long &v, Base base = DEC) const;
	bool toNum(unsigned long long &v, Base base = DEC) const;

	StringW arg(const StringW &arg) const;

	// For convinience!
	inline StringW arg(float v, const wchar_t *fmt = L"%.9g") const { return arg(fromNum(v, fmt)); }
	inline StringW arg(double v, const wchar_t *fmt = L"%.17g") const { return arg(fromNum(v, fmt)); }
	inline StringW arg(short v, const wchar_t *fmt = L"%hi") const { return arg(fromNum(v, fmt)); }
	inline StringW arg(unsigned short v, const wchar_t *fmt = L"%hu") const { return arg(fromNum(v, fmt)); }
	inline StringW arg(int v, const wchar_t *fmt = L"%i") const { return arg(fromNum(v, fmt)); }
	inline StringW arg(unsigned int v, const wchar_t *fmt = L"%u") const { return arg(fromNum(v, fmt)); }
	inline StringW arg(long long v, const wchar_t *fmt = L"%I64d") const { return arg(fromNum(v, fmt)); }
	inline StringW arg(unsigned long long v, const wchar_t *fmt = L"%I64u") const { return arg(fromNum(v, fmt)); }
	inline StringW arg(const StringA &str, unsigned codePage = CP_UTF8) const { return arg(fromStringA(str, codePage)); }

	static StringW fromNum(float v, const wchar_t *fmt = L"%.9g"); // can return nan, +inf, -inf
	static StringW fromNum(double v, const wchar_t *fmt = L"%.17g"); // can return nan, +inf, -inf
	static StringW fromNum(short v, const wchar_t *fmt = L"%hi") { return format(fmt, v); }
	static StringW fromNum(unsigned short v, const wchar_t *fmt = L"%hu") { return format(fmt, v); }
	static StringW fromNum(int v, const wchar_t *fmt = L"%i") { return format(fmt, v); }
	static StringW fromNum(unsigned int v, const wchar_t *fmt = L"%u") { return format(fmt, v); }
	static StringW fromNum(long long v, const wchar_t *fmt = L"%I64d") { return format(fmt, v); }
	static StringW fromNum(unsigned long long v, const wchar_t *fmt = L"%I64u") { return format(fmt, v); }

	static StringW fromStringA(const StringA &str, unsigned codePage = CP_UTF8);

	static StringW format(const wchar_t *str, ...);

};

static StringW operator+(const wchar_t *a, const StringW &b) { return StringW(a) + b; }


}

