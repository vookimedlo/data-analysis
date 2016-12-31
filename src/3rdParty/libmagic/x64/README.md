file-win win32/x64
==================
Forked from the https://github.com/alberthdev/file-win32 which is the Win32 native port of file by Albert Huang ([@alberthdev])
Original file tool and libmagic by Ian F. Darwin and others;
maintained 1995-present by Christos Zoulas and others.

Hosted: https://github.com/vookimedlo/file-win.

Focusing on x64 architecture built by VS2015.

About
------
file-win is a dirty (but "native") win32/x64 port of the original file
tool (from http://www.darwinsys.com/file/) and corresponding libmagic
library.

This port specifically allows for building natively on Visual Studio,
removing the requirement for installing Cygwin or MinGW to build
Windows binaries.

This port was created for other projects requiring magic1.dll built for
win32 or x64 architecture. This was achieved by using the most recent
version of [glibc regex2 library](https://github.com/vookimedlo/glibc-win/releases/tag/glibc-2.24-regex-vs2015) and [zlib library](https://github.com/vookimedlo/zlib/releases/tag/ZLIB_1_2_8__VS2015), which could be built for both architectures.

Unlike the Albert's binaries, these libraries haven't been tested with Python, but as a part of the C++ project.
Python users, if you are fine just with the win32 architecture, please, use the [Albert's Win32 port.](https://github.com/alberthdev/file-win32)

Download
---------
Binaries and source code are available on the [release page][1].

A binary release includes the file tool executable, the libmagic DLL
library, and the development files needed to link with the libmagic
library.

Toolchain
-------------
Visual Studio 2015

Building
---------
Simply open up win\file.sln and build your preferred target(s) either for win32 or x64 architecture.
Available targets are:

 * Release EXE - file executable, no debugging symbols.
 * Debug EXE - file executable, with debugging symbols
 * Release - libmagic DLL, no debugging symbols
 * Debug - libmagic DLL, with debugging symbols

DLL targets use default Debug and Release target so you can include the project file into your own solution if you require to build the library as part of your build process.

Note that `file.exe` (the file tool executable) does NOT require
libmagic.

The resulting binaries (both `file.exe` and `magic1.dll`) require two
DLLs: [`regex2.dll`](https://github.com/vookimedlo/glibc-win/releases/tag/glibc-2.24-regex-vs2015) and [`zlib1.dll`](https://github.com/vookimedlo/zlib/releases/tag/ZLIB_1_2_8__VS2015).

Once you have finished building, you should copy those DLLs from the
`win\*-lib` directories to the `Release*\` and/or `Debug*\`
directories, as well as any other directories containing file-win
binaries.

Finally, `magic.mgc` is not creatable on Windows at the moment. You
need to build the source from a Linux machine to create this file.
This file is required for the tool to work. For convenience, a
pre-generated file is available on the [release page][1].

License
--------
As with the original tool, the license is the BSD 2-clause license.

    Copyright (c) Ian F. Darwin 1986-1995.
    Software written by Ian F. Darwin and others;
    maintained 1995-present by Christos Zoulas and others;
    win32 native (Visual Studio-compatible) port by Albert Huang and others.
    x64 VS2015 port by Michal Duda
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    1. Redistributions of source code must retain the above copyright
       notice immediately at the beginning of the file, without modification,
       this list of conditions, and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     
    THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR
    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.

[1]: https://github.com/vookimedlo/file-win/releases
[@alberthdev]: https://github.com/alberthdev
