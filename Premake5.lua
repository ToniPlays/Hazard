include "Config.lua"
include "Dependencies.lua"
include "scripts/setup/premakeUtils/XCOdeBuildSettings.lua"

workspace "Hazard"

	startproject (STARTUP_PROJECT)
    cppdialect (CPP_DIALECT)

	configurations (CONFIGS)

	flags
	{
		"MultiProcessorCompile"
	}
    
	defines
	{
		"GLFW_INCLUDE_NONE",
        "NOMINMAX"
	}
 

	filter "system:windows"
        systemversion "latest"
		architecture "x86_64"
        defines
        {
            "HZR_PLATFORM_WINDOWS"
        }

	filter "system:macosx"
        architecture "universal"
        defines
        {
            "HZR_PLATFORM_MACOS"
        }
        
    filter "system:ios"
        architecture "universal"
        defines
        {
            "HZR_PLATFORM_IOS"
        }
        
    filter "configurations:Debug"
        defines "HZR_DEBUG"
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines "HZR_RELEASE"
        runtime "Release"
        optimize "on"
 
    filter "configurations:Dist"
        defines "HZR_DIST"
        runtime "Release"
        optimize "on"

    IncludeXCodeBuildSettings()

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"

	if os.target() == "windows" or os.target() == "macosx" then
		include "Hazard/vendor/Glad"
		include "Hazard/vendor/GLFW"
	end

	include "Hazard/vendor/Box2D"
	include "Hazard/vendor/yaml-cpp"
	include "Hazard/vendor/imgui"

    include "Hazard/vendor/Coral/Coral.Managed"
    include "Hazard/vendor/Coral/Coral.Native"

group ""

include "Hazard-Utility"
include "Hazard-Renderer"
include "Hazard-UI"
include "HazardScripting"
include "HazardEditor"
include "Hazard"

include "HazardLauncher"
include "Hazard-Test-App"
include "Hazard-Script"
