-- premake5.lua

-- workspace
workspace "Schizle"
configurations { "Debug", "Release" }

-- lang

project "Schizle"
language "C"
targetdir "bin/%{cfg.buildcfg}"
kind "ConsoleApp"

-- files
files { "src/**.c", "src/include/**.h" }


-- standard library
project "standard"
language "C"
targetdir "lib/std/%{cfg.buildcfg}"
kind "None"

-- files
files { "src/**.c", "src/include/**.h" }
