workspace "main"
    configurations { "Debug", "Release" }

project "matekx"
    kind "StaticLib"
    language "C++"
    targetdir "lib/%{cfg.buildcfg}"

    files { "source/**.cpp", "source/**.h" }
    
    filter "configurations:DEBUG"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "exprx"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files { "tests/*.cpp" }

    links { "matekx" }

    filter "configurations:DEBUG"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

