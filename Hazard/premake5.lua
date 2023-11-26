project "Hazard"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/VulkanMemoryAllocator/**.h",
		"vendor/VulkanMemoryAllocator/**.cpp",
		"vendor/ImGui_Backend/**.h",
		"vendor/ImGui_Backend/**.cpp"
	}

    includedirs {
        "src",
        (Dependencies.HazardUtility.IncludeDir),
        (Dependencies.HazardRenderer.IncludeDir),
        (Dependencies.HazardScript.IncludeDir),
        (Dependencies.Hazard.IncludeDir),
        (Dependencies.Assimp.IncludeDir),
        (Dependencies.GLM.IncludeDir),
        (Dependencies.Spdlog.IncludeDir),
        (Dependencies.ImGUI.IncludeDir),
        (Dependencies.YAML.IncludeDir),
        (Dependencies.EnTT.IncludeDir),
        (Dependencies.Coral.IncludeDir),
        (Dependencies.Box2D.IncludeDir),
        (Dependencies.GLAD.IncludeDir),
        (Dependencies.GLFW.IncludeDir),
        (Dependencies.SpirvCross.IncludeDir),
        (Dependencies.Vulkan.IncludeDir),
        (Dependencies.Metal.IncludeDir)
    }

	defines
	{
		"GLFW_INCLUDE_NONE",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD",
		"_CRT_SECURE_NO_WARNINGS"
	}

	filter "system:windows"
		pchheader "hzrpch.h"

	filter "system:macosx or system:ios"
		pchheader "src/hzrpch.h"
        
		files
		{
			"src/**.m",
			"src/**.mm",
			"vendor/ImGui_Backend/**.m",
			"vendor/ImGui_Backend/**.mm"
		}

    References("HazardUtility")
    References("HazardRenderer")
    References("HazardScript")
    References("GLM")
    References("Spdlog")
    References("ImGUI")
    References("YAML")
    References("EnTT")
    References("Coral")
    References("Box2D")
    References("GLAD")
    References("GLFW")
    References("Assimp")
    References("SpirvCross")
    References("Metal")
