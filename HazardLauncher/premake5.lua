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
	
	References("YAML")
	References("GLM")
	References("Spdlog")
	References("EnTT")
	References("GLFW")
	References("VMA")
	References("SpirvCross")
	References("OpenGL")
	References("Vulkan")
	References("Metal")
	References("Box2D")
	References("ImGUI")
	References("Coral")
	References("HazardUtility")
	References("HazardRenderer")
	References("HazardScript")
	References("Hazard")

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