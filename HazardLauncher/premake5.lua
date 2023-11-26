project "HazardLauncher"

	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"%{wks.location}/Hazard/vendor/ImGui_Backend/**.h",
		"%{wks.location}/Hazard/vendor/ImGui_Backend/**.cpp"
	}

	includedirs
	{
		"src"
	}

	postbuildcommands 
	{
		"{COPYDIR} \"%{wks.location}/HazardEditor/res\" \"%{cfg.targetdir}/res\""
	}


	filter "system:windows"
		defines {
			"_CRT_SECURE_NO_WARNINGS",
		}
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\"",
			"{COPY} %{wks.location}/Hazard/vendor/assimp/lib/assimp-vc142-mt.dll %{cfg.targetdir}",
		}

	filter "system:ios"
		kind "WindowedApp"

	filter "system:macosx or system:ios"
		files
		{
            "src/**.m",
            "src/**.mm",
			"%{wks.location}/Hazard/vendor/ImGui_Backend/**.m",
			"%{wks.location}/Hazard/vendor/ImGui_Backend/**.mm"
        }
	filter "system:windows or system:macosx"
		kind "ConsoleApp"
		
	filter ""
