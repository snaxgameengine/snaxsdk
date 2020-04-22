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
#include "MString.h"


namespace m3d
{

class M3DCORE_API Path
{
public:
	// Default constructor creates an invalid path
	Path() {}
	// Create a path, absolute or relative. End it with a \ to create a directory path. Else it will be a file path.
	Path(String name);
	Path(wchar_t *name);
	// Create a path, relative to some other path.
	Path(Path p, Path relativeTo);
	// Creates a new file path.
	static Path File(Path name, Path relativeTo = Path());
	// Creates a new directory path.
	static Path Dir(Path name, Path relativeTo = Path());
	// Creates a path with the system temp directory.
	static Path TempDir();
	// Creates a path with the current diretory.
	static Path CurrentDir();
	// Case insensitive comparisons.
	bool operator==(const Path &rhs) const;
	bool operator!=(const Path &rhs) const;
	bool operator<(const Path &rhs) const;
	// Returns a clean string representation of the path. Ends with a \ for a directory. Starts with a '.' if relative. Empty if invalid. If relative it may contain '..'.
	String AsString() const { return _path; }
	// Returns true if path is file or directory.
	bool IsValid() const;
	// Returns true if path is directory.
	bool IsDirectory() const;
	// Returns true if absolute path or a relative path that does not contain any '..'.
	bool IsDetermined() const;
	// Returns true if root directory, drive (x:\) or relative (.\).
	bool IsRoot() const;
	// Returns true if drive (x:\).
	bool IsDrive() const;
	// Returns true if path is file
	bool IsFile() const;
	// Returns true if relative path.
	bool IsRelative() const;
	// Returns true if absolute path.
	bool IsAbsolute() const;
	// Returns the absolute path. If relative, it makes it relative to CurrentDir().
	Path GetAbsolute() const;
	// Returns the drive (eg c:\) of the path
	Path GetDrive() const; 
	// For a file: Its parent directory. For dir: itself!
	Path GetDirectory() const;
	// Returns the parent directory of file or dir.
	Path GetParentDirectory() const;
	// Gets the file/dir name without parent directories.
	String GetName() const;
	// Returns file extension (name after last dot)
	String GetFileExtention() const;
	// Returns filename excluding last dot.
	String GetFileNameWithoutExtention() const;
	// Checks if this path is in a subdirectory of the given path. Both must be relative or absolute. If releative, they are considered relative to the same thing.
	bool IsInSubFolder(Path checkIfInSubFolderToThis) const;
	// Returns a string representation of this path relative to the given path. Both must be relative or absolute.
	Path GetRelativePath(Path relativeTo) const;
	// Compares file name and returns true if equal
	bool CompareName(String name) const;
	// Compares the file extention and returns true if equal.
	bool CompareFileExtention(String ext) const;
	// Returns a new path with a new file extension.
	Path ChangeFileExtention(String ext) const;
	// Returns a new path with a new file base name.
	Path ChangeBaseName(String baseName) const;
	// Check if the path does not contain any special characters like |*?<>:\"
	bool ContainsValidCharactersOnly() const;

	// These functions operates on the file system.
	bool Copy(Path newPath, bool failIfExists = true) const;
	bool Move(Path newPath) const;
	bool CreateDir(bool recursive = false) const;
	bool CheckExistence() const;
	bool Delete() const;

private:
	String _path;


};

}