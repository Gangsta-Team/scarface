workspace "scarface-rcf-explorer"
	configurations { "Release", "Debug" }
	platforms { "Win32", "Win64" }
	language "c#"
	location "..\\solution"

project "scarface-rcf-explorer"
	system "Windows"
	kind "ConsoleApp"

	architecture "x86_64"
	targetdir "$(SolutionDir)..\\output\\scarface-rcf-explorer_x86_64"
	objdir "$(SolutionDir)..\\output\\scarface-rcf-explorer_x86_64\\objects"

	symbols "Off"
	optimize "Off"
	
	clr "Unsafe"
	warnings "off"
	
	links { "System", "System.Windows.Forms", "System.Drawing", "System.Design" }

	filter "configurations:Debug"
		defines {
			"STWIY_DEBUG"
		}
		targetname "scarface-rcf-explorer_debug"
		
	filter "configurations:Release"
		defines {
			"STWIY_RELEASE"
		}
		targetname "scarface-rcf-explorer_release"
		
	filter {}

	files {
		--"..\\source\\**",
		"..\\source\\.editorconfig",
		"..\\source\\Window.cs",
		"..\\source\\SettingsForm.cs",
		"..\\source\\INIFile.cs",
		"..\\source\\KaitaiStruct.cs",
		"..\\source\\KaitaiStream.cs",
		"..\\source\\ProcessWithOutput.cs",
		"..\\source\\Brokenface\\**",
		--"..\\source\\Pure3D.cs",
		"..\\source\\Cement.cs",
	}
	
	filter "files:**.py"
		buildaction "Embed"
		
	includedirs {
		"$(SolutionDir)..\\source"
	}
