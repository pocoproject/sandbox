--
-- premake4.lua
--
-- Top level Premake build script for irrlicht project
--
-- Copyright c 2008-2010 Jiang Shan
--
-- This software is provided 'as-is', without any express or implied
-- warranty.  In no event will the authors be held liable for any damages
-- arising from the use of this software.
-- 
-- Permission is granted to anyone to use this software for any purpose,
-- including commercial applications, and to alter it and redistribute it
-- freely, subject to the following restrictions:
--
-- 1. The origin of this software must not be misrepresented; you must not
--    claim that you wrote the original software. If you use this software
--    in a product, an acknowledgment in the product documentation would be
--    appreciated but is not required.
-- 2. Altered source versions must be plainly marked as such, and must not be
--    misrepresented as being the original software.
-- 3. This notice may not be removed or altered from any source distribution.
--

solution ( "Foundation" .. _ACTION)

platforms { "x32", "x64" }

-- clean             Remove all binaries and generated files
-- codeblocks        Generate Code::Blocks project files
-- codelite          Generate CodeLite project files
-- gmake             Generate GNU makefiles for POSIX, MinGW, and Cygwin
-- vs2002            Generate Microsoft Visual Studio 2002 project files
-- vs2003            Generate Microsoft Visual Studio 2003 project files
-- vs2005            Generate Microsoft Visual Studio 2005 project files
-- vs2008            Generate Microsoft Visual Studio 2008 project files
-- vs2010            Generate Visual Studio 2010 project files (experimental)
-- xcode3            Generate Apple Xcode 3 project files (experimental)

-- the following 4 configurations:
configurations { 
	"debug_shared", 
	"release_shared", 
	"debug_static_md", 
	"release_static_md",
	"debug_static_mt", 
	"release_static_mt",
}

location ("./" .. _ACTION)

dofile("build_foundation.lua")

--dofile("build_foundation_samples.lua")

--dofile("build_foundation_testcase.lua")


