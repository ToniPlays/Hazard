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
        "%{wks.location}/Hazard/vendor"
	}

	    
    References("HazardUtility")
    References("HazardRenderer")
    References("HazardScript")
    References("Hazard")
    References("YAML")
    References("Assimp")
    References("GLM")
    References("Spdlog")
    References("ImGUI")
    References("YAML")
    References("EnTT")
    References("Coral")
    References("Box2D")
    References("GLAD")
    References("GLFW")
	References("VMA")
    References("SpirvCross")
    References("OpenGL")
    References("Vulkan")
    References("Metal")

	postbuildcommands {
		"{COPYDIR} \"%{wks.location}/HazardEditor/res\" \"%{cfg.targetdir}/res\""
	}


	filter "system:macosx"
	    files
	    {
            "src/**.m",
            "src/**.mm",
		    "%{wks.location}/Hazard/vendor/ImGui_Backend/**.m",
		    "%{wks.location}/Hazard/vendor/ImGui_Backend/**.mm"
        }