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
		"vendor/ImGui_Backend/**.h",
		"vendor/ImGui_Backend/**.cpp"
	}

    includedirs
    {
        "src"
    }

	defines
	{
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
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
	References("VMA")
    References("Assimp")
    References("SpirvCross")
    References("Metal")

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