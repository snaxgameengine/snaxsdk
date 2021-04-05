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
#include "M3DCore/List.h"
#include "M3DCore/Map.h"
#include "M3DCore/MString.h"


namespace m3d
{


struct ChipDesc
{
	enum Usage { STANDARD, HIDDEN };
	const wchar_t *name;
	GUID type;
	GUID basetype;
	Usage usage;
	Version version;
	const char *factoryFunc;
	const wchar_t *filters; // For importers only. File types supported by Importer-chip, eg "dae;xml;3ds"
};


// Each packet implements this function in its exports.cpp file.
extern const ChipDesc &RegisterChipDesc(const wchar_t *name, GUID type, GUID basetype, ChipDesc::Usage usage, unsigned version, const char *factoryFunc, const wchar_t *filters = L"");

// CHIPDESC_DECL is placed in all new chips.
#define CHIPDESC_DECL public: static const m3d::ChipDesc DESC; virtual const m3d::ChipDesc &GetChipDesc() const override { return DESC; } 
// CHIPDESCV1_DEF, CHIPDESCV1_DEF_HIDDEN, CHIPDESCV1_DEF_VIRTUAL OR CHIPDESCV1_DEF_IMPORTER is placed in the cpp file of the chip, or in the exports.cpp of the packet.
// TODO: If placed in cpp file of chip, confirm that RegisterChipDesc(...) is always called on library loading, and that this technique is safe!
#define CHIPDESCV1_DEF(clazz, name, guid, baseGuid) const m3d::ChipDesc clazz::DESC = RegisterChipDesc(name, guid, baseGuid, m3d::ChipDesc::STANDARD, VERSION1, #clazz"_FACTORY"); extern "C" __declspec( dllexport ) m3d::Chip* __cdecl clazz##_FACTORY() throw(...) { return mmnew clazz(); }
#define CHIPDESCV1_DEF_HIDDEN(clazz, name, guid, baseGuid) const m3d::ChipDesc clazz::DESC = RegisterChipDesc(name, guid, baseGuid, m3d::ChipDesc::HIDDEN, VERSION1, #clazz"_FACTORY"); extern "C" __declspec( dllexport ) m3d::Chip* __cdecl clazz##_FACTORY() throw(...) { return mmnew clazz(); }
#define CHIPDESCV1_DEF_VIRTUAL(clazz, name, guid, baseGuid) const m3d::ChipDesc clazz::DESC = RegisterChipDesc(name, guid, baseGuid, m3d::ChipDesc::HIDDEN, VERSION1, #clazz"_FACTORY"); extern "C" __declspec( dllexport ) m3d::Chip* __cdecl clazz##_FACTORY() throw(...) { return nullptr; }
#define CHIPDESCV1_DEF_IMPORTER(clazz, name, guid, baseGuid, filters) const m3d::ChipDesc clazz::DESC = RegisterChipDesc(name, guid, baseGuid, m3d::ChipDesc::HIDDEN, VERSION1, #clazz"_FACTORY", filters); extern "C" __declspec( dllexport ) m3d::Chip* __cdecl clazz##_FACTORY() throw(...) { return mmnew clazz(); }
// These are for version other than 1.0.0.0:
#define CHIPDESC_DEF(clazz, name, guid, baseGuid, version) const m3d::ChipDesc clazz::DESC = RegisterChipDesc(name, guid, baseGuid, m3d::ChipDesc::STANDARD, version, #clazz"_FACTORY"); extern "C" __declspec( dllexport ) m3d::Chip* __cdecl clazz##_FACTORY() throw(...) { return mmnew clazz(); }
#define CHIPDESC_DEF_HIDDEN(clazz, name, guid, baseGuid, version) const m3d::ChipDesc clazz::DESC = RegisterChipDesc(name, guid, baseGuid, m3d::ChipDesc::HIDDEN, version, #clazz"_FACTORY"); extern "C" __declspec( dllexport ) m3d::Chip* __cdecl clazz##_FACTORY() throw(...) { return mmnew clazz(); }
#define CHIPDESC_DEF_VIRTUAL(clazz, name, guid, baseGuid, version) const m3d::ChipDesc clazz::DESC = RegisterChipDesc(name, guid, baseGuid, m3d::ChipDesc::HIDDEN, version, #clazz"_FACTORY"); extern "C" __declspec( dllexport ) m3d::Chip* __cdecl clazz##_FACTORY() throw(...) { return nullptr; }
#define CHIPDESC_DEF_IMPORTER(clazz, name, guid, baseGuid, filters, version) const m3d::ChipDesc clazz::DESC = RegisterChipDesc(name, guid, baseGuid, m3d::ChipDesc::HIDDEN, version, #clazz"_FACTORY", filters); extern "C" __declspec( dllexport ) m3d::Chip* __cdecl clazz##_FACTORY() throw(...) { return mmnew clazz(); }



struct ChildConnectionDesc
{
	enum DataDirection { UP=-1, BOTH, DOWN};
	enum ConnectionType { SINGLE, GROWING, MULTI };
	GUID type;
	ConnectionType connType;
	DataDirection dataDirection;
	String name;
};

class Chip;

struct SubConnection
{
	// The chip connected.
	Chip *chip;
	// Timestamp of the last time GetRawChild() was called. It is used to provide visual feedback of link activity in the editor.
	long lastHit;
	SubConnection(Chip *chip = nullptr) : chip(chip), lastHit(0) {}
};


typedef List<SubConnection> SubConnectionList;

typedef List<Chip*> ChipList;
template class M3DENGINE_API List<Chip*>;

struct ChildConnection
{
	const ChildConnectionDesc desc;
	const ChipTypeIndex chipTypeIndex;
	SubConnectionList connections;
	ChildConnection(const ChildConnectionDesc &desc, ChipTypeIndex chipTypeIndex) : desc(desc), chipTypeIndex(chipTypeIndex) {}
};

typedef List<ChildConnection*> ChildConnectionList;
template class M3DENGINE_API List<ChildConnection*>;


#define CREATE_CHILD(index, type, growing, dataDirection, name) { m3d::ChildConnectionDesc desc = {type, (growing ? m3d::ChildConnectionDesc::GROWING : m3d::ChildConnectionDesc::SINGLE), m3d::ChildConnectionDesc::dataDirection, name}; SetConnection(index, desc, false); }
#define CREATE_CHILD_KEEP(index, type, growing, dataDirection, name) { m3d::ChildConnectionDesc desc = {type, (growing ? m3d::ChildConnectionDesc::GROWING : m3d::ChildConnectionDesc::SINGLE), m3d::ChildConnectionDesc::dataDirection, name}; SetConnection(index, desc, true); }

struct ChipExceptionScope
{
	static Chip M3DENGINE_API *currentChip;
	Chip *previousChip;
	ChipExceptionScope(Chip *chip) : previousChip(currentChip) { currentChip = chip; }
	~ChipExceptionScope() { currentChip = previousChip; }
};

struct ChipException
{
	String category;
	String msg;
	MessageSeverity severity;
	Chip *chip;
	ChipException(String category, String msg, MessageSeverity severity) : category(category), msg(msg), severity(severity), chip(ChipExceptionScope::currentChip) {}
	ChipException(String category, String msg, MessageSeverity severity, Chip *chip) : category(category), msg(msg), severity(severity), chip(chip) {}
};

struct ChipMessage
{
	String category;
	String msg;
	MessageSeverity severity;
	unsigned hitCount; // Initialize to 0.

	ChipMessage() : severity(DINFO), hitCount(0) {}
	ChipMessage(String category, String msg, MessageSeverity severity) : category(category), msg(msg), severity(severity), hitCount(0) {}
	ChipMessage(const ChipException &exc) : ChipMessage(exc.category, exc.msg, exc.severity) {}
};

#define __WIDEN(x) L##x
#define CHIPMSG(className, severity, msg) struct className : m3d::ChipException { className(m3d::String m = msg, m3d::MessageSeverity s = severity) : m3d::ChipException(__WIDEN(#className), m, s) {} className(m3d::Chip *chip, m3d::String m = msg, m3d::MessageSeverity s = severity) : m3d::ChipException(__WIDEN(#className), m, s, chip) {} };
#define CHIPMSGV(className, severity, msg, ...) struct className : m3d::ChipException { className(__VA_ARGS__, m3d::MessageSeverity s = severity) : m3d::ChipException(__WIDEN(#className), msg, s) {} className(m3d::Chip *chip, __VA_ARGS__, m3d::MessageSeverity s = severity) : m3d::ChipException(__WIDEN(#className), msg, s, chip) {} };


typedef List<ChipMessage> ChipMessageList;



class ShellChip;
class ProxyChip;
class Parameter;
class FunctionCall;
class FunctionData;
class InstanceData;
class ClassInstance;
class Shortcut;


class ProjectDependencies;
class RenderWindow;

class DocumentLoader;
class DocumentSaver;



class M3DENGINE_API RefreshManager
{
	friend struct RefreshT;
public:
	enum RefreshMode { ALWAYS, FUNCTION, FRAME, ONCE, NEVER };
private:
	RefreshMode _rm;

	unsigned _lastFrame; // The last frame nr we where hit. 0 if unhit.
	unsigned _lastStack; // The last stack nr we where hit.

public:
	RefreshManager() : _rm(FUNCTION), _lastFrame(0), _lastStack(0) {}
	operator bool();

	RefreshMode GetRefreshMode() const { return _rm; }
	void SetRefreshMode(RefreshMode rm) { _rm = rm; }

	void Reset() { _lastFrame = 0; _lastStack = 0; }
};

struct RefreshT
{
	RefreshManager &rm;
	bool b;
	unsigned t1, t2;
	RefreshT(RefreshManager &rm) : rm(rm), b(rm), t1(rm._lastFrame), t2(rm._lastStack) {}
	~RefreshT() { rm._lastFrame = t1; rm._lastStack = t2; }
	inline operator bool() const { return b; }
};

/*
struct ChipEditorData 
{
	Chip * const chip;
	String comment;
	ChipEditorData(Chip *chip) : chip(chip) {}
};
*/

struct ChipEditorData;

typedef unsigned UpdateStamp;

extern UpdateStamp M3DENGINE_API GenerateUpdateStamp();


}