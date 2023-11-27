project "HazardEditor"

	kind "ConsoleApp"
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

	defines 
	{
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

	includedirs
	{
		"src",
        "%{wks.location}/Hazard/vendor",
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

	postbuildcommands {
		"{COPYDIR} \"%{wks.location}/HazardEditor/res\" \"%{cfg.targetdir}/res\""
	}


	filter "system:windows"
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\"",
			"{COPY} %{wks.location}/Hazard/vendor/assimp/lib/assimp-vc142-mt.dll %{cfg.targetdir}",
		}

	filter "system:macosx"
	files
	{
            "src/**.m",
            "src/**.mm",
		"%{wks.location}/Hazard/vendor/ImGui_Backend/**.m",
		"%{wks.location}/Hazard/vendor/ImGui_Backend/**.mm"
    }
    
    References("HazardUtility")
    References("HazardRenderer")
    References("Hazard")
    References("GLM")
    References("Spdlog")
    References("ImGUI")
    References("YAML")
    References("EnTT")
    References("Coral")
    References("Box2D")
    References("GLAD")
    References("GLFW")
    References("SpirvCross")
    References("OpenGL")
    References("Vulkan")
    References("Metal")
