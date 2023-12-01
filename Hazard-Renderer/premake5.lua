project "Hazard-Renderer"

	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/VulkanMemoryAllocator/**.h",
		"vendor/VulkanMemoryAllocator/**.cpp",
		"examples/**.cpp"
	}
    
	includedirs 
	{
		"src"
	}

	References("HazardUtility")
    References("GLM")
    References("SpirvCross")
    References("GLFW")
    References("GLAD")
    References("Spdlog")
	References("VMA")
    References("OpenGL")
    References("Vulkan")
    References("Metal")

	filter "system:windows or system:macosx"
		References("ShaderC")
    
	filter "system:macosx or system:ios"
		files 
		{
			"src/**.mm",
			"src/**.m"
		}
