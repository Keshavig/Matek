workspace "fun_math"
    configurations { "Debug", "Release" }

project "evaluater"
    kind "ConsoleApp"
    language "C++"
    targetdir "bin/%{cfg.buildcfg}"

    files { "src/**.h", "src/**.cc" }
    removefiles { "./src/All_In_One_Eval.cc" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
