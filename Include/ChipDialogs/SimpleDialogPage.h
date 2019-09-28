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
#include "DialogPage.h"



namespace m3d
{

typedef void* RPtr;


enum RSortOrder
{
	AscendingOrder,
	DescendingOrder
};

enum RCheckState 
{
	Unchecked,
	PartiallyChecked,
	Checked
};

typedef unsigned RData;


class CHIPDIALOGS_EXPORT RVariant
{
public:
	RVariant();
	RVariant(const RVariant& rhs);
	RVariant(RPtr ptr);
	RVariant(bool d);
	RVariant(int d);
	RVariant(unsigned int d);
	RVariant(long long d);
	RVariant(unsigned long long d);
	RVariant(float d);
	RVariant(double d);
	RVariant(String d);
	~RVariant();

	RVariant& operator=(const RVariant& rhs);
	bool operator==(const RVariant& rhs) const;
	bool operator!=(const RVariant& rhs) const;

	bool IsNull() const;
	bool IsValid() const;

	bool ToBool() const;
	int ToInt() const;
	unsigned int ToUInt() const;
	long long ToInt64() const;
	unsigned long long ToUInt64() const;
	float ToFloat() const;
	double ToDouble() const;
	String ToString() const;

	RPtr ptr() const { return _ptr; }
private:
	RPtr _ptr;
};

class CHIPDIALOGS_EXPORT SimpleDialogPage : public DialogPage
{
protected:
	SimpleDialogPage();
	~SimpleDialogPage();

	QWidget* GetWidget() override;

	void setPreferredSize(int w, int h);

	RPtr _ptr;
};

}