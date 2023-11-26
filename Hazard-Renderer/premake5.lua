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
    
	includedirs {
		"src",
        (Dependencies.HazardUtility.IncludeDir),
        (Dependencies.GLM.IncludeDir),
        (Dependencies.Spdlog.IncludeDir),
        (Dependencies.GLAD.IncludeDir),
        (Dependencies.GLFW.IncludeDir),
        (Dependencies.SpirvCross.IncludeDir),
        (Dependencies.Vulkan.IncludeDir),
        (Dependencies.Metal.IncludeDir)
	}
    
	filter "system:macosx or system:ios"
		files {
			"src/**.mm",
			"src/**.m"
		}
        
    References("HazardUtility")
    References("GLM")
    References("SpirvCross")
    References("GLFW")
    References("GLAD")
    References("Spdlog")
    References("OpenGL")
    References("Vulkan")
    References("Metal")
