-- premake5.lua

-- workspace
workspace "Schizle"
configurations { "Debug", "Release" }
platforms { "windows", "mac", "linux" }

filter "configurations:Debug"
defines { "DEBUG" }

filter "configurations:Release"
defines { "RELEASE" }

filter { "platforms:windows" }
system "windows"

filter { "platforms:mac" }
system "macosx"

filter { "platforms:linux" }
system "linux"


-- lang

project "schizle_interpreter"
language "C"
targetdir "bin/%{cfg.buildcfg}"
kind "ConsoleApp"

-- files
files { "src/Schizle.c", "src/include/*.h", "modules/*.h" }

-- builder

project "schizle_builder"
language "C"
targetdir "bin/%{cfg.buildcfg}"
kind "ConsoleApp"

-- files
files { "src/builder.c", "src/include/*.h", "modules/*.h" }

project "Schizle"
language "C"
targetdir "bin/%{cfg.buildcfg}"
kind "ConsoleApp"

-- files
files { "src/command.c", "src/include/*.h", "modules/*.h" }
dependson { "schizle_interpreter", "schizle_builder" }
