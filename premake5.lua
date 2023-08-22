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
files { "src/**.c", "src/include/**.h", "std/**.h" }
