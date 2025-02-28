workspace "main"
    configurations { "Debug", "Release" }

project "MatekLib"
    kind "SharedLib"
    language "C++"
    targetdir "lib/%{cfg.buildcfg}"

    files { "source/**.cc", "source/**.h" }
    
    filter "configurations:DEBUG"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

project "matekx"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files { "tests/*.cc" }

    links { "MatekLib" }

    filter "configurations:DEBUG"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

