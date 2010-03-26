project "SpiderMonkey"
  language "C++"
  kind "SharedLib"
  targetname "PocoSpiderMonkey"
  location   ( solution().location )

  configuration "Debug"
    targetsuffix "d"
    
  configuration { }
  
  defines { 
            "SpiderMonkey_EXPORTS"
          }
  
  files {
            "src/*.cpp"
          , "include/**.h"
        }

  includedirs
  {
    "include"
  , "../../include"
  }

  configuration "windows"
    includedirs {
                  poco_dir .. "/Foundation/include"
                , poco_dir .. "/Util/include"
                }

  configuration "linux"
    includedirs {
                  poco_dir
                }

  -- Set the library paths
  configuration "windows"
    libdirs { 
                poco_dir .. "/lib" 
              , "../../lib"
            }

  configuration { }
              
  configuration "Debug"
    links {
              "PocoFoundationd"
            , "PocoScriptd"
          }
    
  configuration "Release"
    links {   
              "PocoFoundation"
            , "PocoScript"
          }
 
  configuration { "windows", "codelite or codeblock" }
    links { "iphlpapi" }
 
  configuration { }
    links { 
            js_lib 
          }
