-- premake5.lua

-- workspace
workspace "Schizle Script"
configurations { "Debug", "Release" }
platforms { "windows", "mac", "linux" }

linkoptions { "-lm" }

filter "configurations:Debug"
defines { "DEBUG" }

filter "configurations:Release"
defines { "RELEASE" }

filter { "platforms:windows" }
defines { "WINDOWS" }
system "windows"

filter { "platforms:mac" }
defines { "UNIX" }
system "macosx"

filter { "platforms:linux" }
defines { "UNIX" }
system "linux"


-- lang

project "Schizle"
language "C"
targetdir "bin/%{cfg.buildcfg}"
kind "ConsoleApp"

-- files
files { "src/command.c" }
-- "src/include/*.h", "modules/*.h", "modules/*.c" }
