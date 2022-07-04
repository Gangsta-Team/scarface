workspace "scarface-mod"
	configurations { "Release", "Debug" }
	cppdialect "C++17"
	platforms { "Win32", "Win64" }
	language "c++"
	location "..\\solution"
	targetextension ".asi"

project "scarface-mod"
	characterset ("MBCS")
	system "Windows"
	kind "SharedLib"

	architecture "x86"
	targetdir "$(SolutionDir)..\\output\\scarface-mod"
	objdir "$(SolutionDir)..\\output\\scarface-mod\\objects"

	symbols "Off"
	optimize "Off"
	
	clr "off"
	warnings "off"

	filter "configurations:Debug"
		defines {
			"STWIY_DEBUG"
		}
		targetname "scarface-mod_debug"
		
	filter "configurations:Release"
		defines {
			"STWIY_RELEASE"
		}
		targetname "scarface-mod_release"
	
	filter {}

	libdirs { 
		"..\\source\\detours",
		"$(DXSDK_DIR)\\Lib\\x86" 
	}
	linkoptions "/SAFESEH:NO"

	files {
		"..\\source\\**",
		"..\\source\\DllMain.cpp",
		"..\\source\\DllMain.h",
		"..\\source\\CConsole.h",
		"..\\source\\CINI.h",
		"..\\source\\CLog.h",
	}
	
	includedirs {
		"$(SolutionDir)..\\source",
		"$(DXSDK_DIR)\\Include",
	}