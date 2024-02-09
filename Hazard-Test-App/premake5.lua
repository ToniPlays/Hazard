project "Hazard-Test-App"

	language "C++"
	staticruntime "off"
    kind "ConsoleApp"

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
		"src",
        "%{wks.location}/Hazard/vendor"
	}
    
    References("HazardUtility")
    References("HazardRenderer")
    References("Hazard")
    References("GLM")
    References("Spdlog")
    References("YAML")
    References("Coral")
    References("Assimp")
    References("ImGUI")
    References("OpenGL")
    References("Vulkan")
    References("Metal")
    References("VMA")

	filter "system:windows"
        References("GLFW")
		References("GLAD")
        References("SpirvCross")

	filter "system:macosx"
		References("GLFW")
        References("SpirvCross")

	filter "system:ios"
		kind "WindowedApp"
  
        files
        {
            "**.storyboard",
            "info.plist"
        }

	filter "system:macosx or system:ios"
        files
		{
            "src/**.m",
            "src/**.mm"
        }
   