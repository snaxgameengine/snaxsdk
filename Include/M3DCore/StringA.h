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

class StringW;

class M3DCORE_API StringA
{
private:
	struct _sharedstr
	{
		std::atomic_uint32_t refs;
		size_t size;
		const size_t capacity;
#pragma warning(push)
#pragma warning (disable : 4200 )
		char str[]; // This is not standard, but but... :)
#pragma warning(pop)
		_sharedstr(size_t size, size_t capacity) : refs(1), size(size), capacity(capacity) {}
	} *_s;

	void _incr();
	void _decr();

	_sharedstr *_allocate(size_t size, size_t capacity);

	char *_str();

	StringA(size_t size, size_t capacity);

public:
	static const size_t npos = -1;

	StringA(const char *str = "", size_t maxChar = npos);

	StringA(const StringA &str);

	explicit StringA(char c);

	~StringA();

	StringA &operator=(const StringA &rhs) { return assign(rhs); }

	StringA &operator+=(const StringA &str);

	StringA &assign(const char *str, size_t maxChar = npos);

	StringA &assign(const StringA &str);

	StringA &append(const StringA &str, size_t maxChar = npos) { return *this += str.substr(0, maxChar); }

	StringA &erase(size_t offset = 0, size_t count = npos);

	void clear() { if (size()) *this = ""; }

	void reserve(size_t capacity);

	bool operator<(const StringA &rhs) const { return compare(rhs) < 0; };

	bool operator==(const StringA &str) const;

	bool operator!=(const StringA &str) const { return !(*this == str); }

	StringA operator+(const StringA &str) const;

	inline char &operator[](size_t index) { return _str()[index]; }
	inline const char &operator[](size_t index) const { return c_str()[index]; }
 
	inline const char *c_str() const { return _s->str; }

	inline size_t length() const { return _s->size; }
	inline size_t size() const { return _s->size; }
	inline size_t capacity() const { return _s->capacity; }
	inline bool empty() const { return _s->size == 0; }

	int compare(const StringA &rhs) const;
	int compareNoCase(const StringA &rhs, const Locale &locale = Locale::DEFAULT) const;

	size_t find(const StringA &str, size_t offset = 0) const;
	size_t rfind(const StringA &str, size_t offset = npos) const;

	size_t find_first_of(char t, size_t offset = 0) const;
	size_t find_first_of(const StringA &str, size_t offset = 0) const;

	size_t find_last_of(char t, size_t offset = npos) const;
	size_t find_last_of(const StringA &str, size_t offset = npos) const;

	size_t find_first_not_of(char t, size_t offset = 0) const;
	size_t find_first_not_of(const StringA &str, size_t offset = 0) const;

	size_t find_last_not_of(char t, size_t offset = npos) const;
	size_t find_last_not_of(const StringA &str, size_t offset = npos) const;

	StringA substr(size_t offset = 0, size_t count = npos) const;

	StringA trimmed(char t = L' ') const;

	StringA replace(const StringA &replaceThis, const StringA &withThis) const;
	StringA replace(const char replaceThis, const char withThis) const;

	StringA to_lower(const Locale &locale = Locale::DEFAULT) const;
	StringA to_upper(const Locale &locale = Locale::DEFAULT) const;

	enum Base { OCT, DEC, HEX };

	bool toNum(float &v, Base base = DEC) const; // handles nan, +inf, -inf
	bool toNum(double &v, Base base = DEC) const; // handles nan, +inf, -inf
	bool toNum(short &v, Base base = DEC) const;
	bool toNum(unsigned short &v, Base base = DEC) const;
	bool toNum(int &v, Base base = DEC) const;
	bool toNum(unsigned int &v, Base base = DEC) const;
	bool toNum(long long &v, Base base = DEC) const;
	bool toNum(unsigned long long &v, Base base = DEC) const;

	StringA arg(const StringA &arg) const;

	// For convinience!
	inline StringA arg(float v, const char *fmt = "%.9g") const { return arg(fromNum(v, fmt)); }
	inline StringA arg(double v, const char *fmt = "%.17g") const { return arg(fromNum(v, fmt)); }
	inline StringA arg(short v, const char *fmt = "%hi") const { return arg(fromNum(v, fmt)); }
	inline StringA arg(unsigned short v, const char *fmt = "%hu") const { return arg(fromNum(v, fmt)); }
	inline StringA arg(int v, const char *fmt = "%i") const { return arg(fromNum(v, fmt)); }
	inline StringA arg(unsigned int v, const char *fmt = "%u") const { return arg(fromNum(v, fmt)); }
	inline StringA arg(long long v, const char *fmt = "%I64d") const { return arg(fromNum(v, fmt)); }
	inline StringA arg(unsigned long long v, const char *fmt = "%I64u") const { return arg(fromNum(v, fmt)); }
	inline StringA arg(const StringW &str, unsigned codePage = CP_UTF8) const { return arg(fromStringW(str, codePage)); }

	static StringA fromNum(float v, const char *fmt = "%.9g"); // can return nan, +inf, -inf
	static StringA fromNum(double v, const char *fmt = "%.17g"); // can return nan, +inf, -inf
	static StringA fromNum(short v, const char *fmt = "%hi") { return format(fmt, v); }
	static StringA fromNum(unsigned short v, const char *fmt = "%hu") { return format(fmt, v); }
	static StringA fromNum(int v, const char *fmt = "%i") { return format(fmt, v); }
	static StringA fromNum(unsigned int v, const char *fmt = "%u") { return format(fmt, v); }
	static StringA fromNum(long long v, const char *fmt = "%I64d") { return format(fmt, v); }
	static StringA fromNum(unsigned long long v, const char *fmt = "%I64u") { return format(fmt, v); }

	static StringA fromStringW(const StringW &str, unsigned codePage = CP_UTF8);

	static StringA format(const char *str, ...);

};

static StringA operator+(const char *a, const StringA &b) { return StringA(a) + b; }


}
