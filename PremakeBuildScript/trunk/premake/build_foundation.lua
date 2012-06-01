--
-- build_foundation.lua
--
-- Premake build script for Poco Foundation
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

-- first we build all the dependencies ... 

function build_foundation (project_name)

    project (project_name)

    language "C++"

--    targetdir ("../lib/".._ACTION)

    platforms "x32"
        targetdir ("../lib/".._ACTION)

    platforms "x64"
        targetdir ("../lib64/".._ACTION)

    flags       { "No64BitChecks"}	

    
    files {
        "../Foundation/include/**.h",
        "../Foundation/src/**.h",
        "../Foundation/src/**.cpp",
        "../Foundation/src/**.c",
        "../Foundation/**.rc",
    }

    includedirs { 
        "../Foundation/include",
    }

    excludes { 
        "../Foundation/src/*_*.cpp",
    }

    configuration "vs*"
		excludes     { 
            "../Foundation/src/SyslogChannel.cpp" ,
            "../Foundation/src/OpcomChannel.cpp" ,
        }

    vpaths { 
        ["Cache/Header Files"] = { 
                                 "**/AbstractCache.h",
                                 "**/AbstractStrategy.h",
                                 "**/AccessExpirationDecorator.h",
                                 "**/AccessExpireCache.h",
                                 "**/AccessExpireLRUCache.h",
                                 "**/AccessExpireStrategy.h",
                                 "**/ExpirationDecorator.h",
                                 "**/ExpireCache.h",
                                 "**/ExpireLRUCache.h",
                                 "**/ExpireStrategy.h",
                                 "**/KeyValueArgs.h",
                                 "**/LRUCache.h",
                                 "**/LRUStrategy.h",
                                 "**/StrategyCollection.h",
                                 "**/UniqueAccessExpireCache.h",
                                 "**/UniqueAccessExpireLRUCache.h",
                                 "**/UniqueAccessExpireStrategy.h",
                                 "**/UniqueExpireCache.h",
                                 "**/UniqueExpireLRUCache.h",
                                 "**/UniqueExpireStrategy.h",
                                 "**/ValidArgs.h"},

        ["Cache/Source Files"] = {},

        ["RegularExpression/Header Files"] = { "**/RegularExpression.h", },

        ["RegularExpression/PCRE Header Files"] = {
                                 "**/pcre.h",
                                 "**/pcre_config.h",
                                 "**/pcre_internal.h",
                                 "**/ucp.h"},

        ["RegularExpression/PCRE Source Files"] = {
                                 "**/pcre_chartables.c",
                                 "**/pcre_compile.c",
                                 "**/pcre_exec.c",
                                 "**/pcre_fullinfo.c",
                                 "**/pcre_globals.c",
                                 "**/pcre_maketables.c",
                                 "**/pcre_newline.c",
                                 "**/pcre_ord2utf8.c",
                                 "**/pcre_study.c",
                                 "**/pcre_tables.c",
                                 "**/pcre_try_flipped.c",
                                 "**/pcre_ucd.c",
                                 "**/pcre_valid_utf8.c",
                                 "**/pcre_xclass.c" },

        ["RegularExpression/Source Files"] = { "**/RegularExpression.cpp" },
    }

    configuration "*static_mt"
        kind "StaticLib"
        defines { "POCO_STATIC", "PCRE_STATIC", "Foundation_EXPORTS"}
        flags { "StaticRuntime" }

    configuration "*static_md"
        kind "StaticLib"
        defines { "POCO_STATIC", "PCRE_STATIC", "Foundation_EXPORTS"}

    configuration "*shared"
        kind "SharedLib"
        defines { "_USRDLL", "PCRE_STATIC", "Foundation_EXPORTS"}

    configuration "debug*"
        defines     "_DEBUG"
        flags       { "Symbols", "ExtraWarnings" }

    configuration "release*"
        defines     "NDEBUG"
        flags       { "OptimizeSpeed"}

	configuration "debug_static_mt"
        targetname ("Poco"..project_name.."mtd")
	configuration "release_static_mt"
        targetname ("Poco"..project_name.."mt")
	configuration "debug_static_md"
        targetname ("Poco"..project_name.."mdd")
	configuration "release_static_md"
        targetname ("Poco"..project_name.."md")
	configuration "debug_shared"
        targetname ("Poco"..project_name.."d")
	configuration "release_shared"
        targetname ("Poco"..project_name)

    configuration "vs*"
		defines     { "_CRT_SECURE_NO_WARNINGS" }

    configuration "code*"
		defines     { "__GNUWIN32__",}

	configuration {"windows"}
        links {
            "ws2_32",
            "iphlpapi",
        }
		defines {
			"WIN32",
			"_WINDOWS",
		}

	configuration { "linux" }
        links {
            "pthread"
        }

end

build_foundation("Foundation")

