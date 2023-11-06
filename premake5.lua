-- premake5.lua

-- workspace
workspace "Schizle Script"
configurations { "Debug", "Release" }
platforms { "windows", "mac", "linux" }

filter "configurations:Debug"
defines { "DEBUG" }

filter "configurations:Release"
defines { "RELEASE" }

filter { "platforms:windows" }
defines { "WINDOWS" }
system "windows"

filter { "platforms:mac" }
linkoptions { "-lm" }
defines { "UNIX" }
system "macosx"

filter { "platforms:linux" }
linkoptions { "-lm" }
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
