include "Dependencies.lua"

workspace "Hazard"

	startproject "HazardEditor"
    	cppdialect "C++20"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}
	defines 
	{
		"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
        systemversion "latest"
        defines
        {
            "HZR_PLATFORM_WINDOWS"
        }

	filter "system:macosx"
        defines
        {
            "HZR_PLATFORM_MACOS"
        }
        
    filter "system:ios"
        defines
        {
            "HZR_PLATFORM_IOS"
        }
	architecture "ARM"
	xcodebuildsettings 
	{
		["SDKROOT"] = "iphoneos"
	}
 
    filter "system:ios or system:macosx"
        architecture "universal"
        xcodebuildsettings
        {
            ["MACOSX_DEPLOYMENT_TARGET"] = "13.0",
            ["IPHONEOS_DEPLOYMENT_TARGET"] = "14.0",
            ["ALWAYS_SEARCH_USER_PATHS"] = "YES",
            ["CLANG_ENABLE_OBJC_WEAK"] = "YES",
            ["DEAD_CODE_STRIPPING"] = "YES",
            ["CLANG_CXX_LANGUAGE_STANDARD"] = "c++20",
	    ["ONLY_ACTIVE_ARCH"] = "YES",
	    ["PRODUCT_BUNDLE_IDENTIFIER"] = "com.hazard",
	    ["INFOPLIST_FILE"] = "info.plist"
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

	filter ""

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"

	filter "system:windows or system:macosx"
		include "Hazard/vendor/Glad"
		include "Hazard/vendor/GLFW"
	filter ""

	include "Hazard/vendor/Box2D"
	include "Hazard/vendor/yaml-cpp"
	include "Hazard/vendor/imgui"

    include "Hazard/vendor/Coral/Coral.Managed"
    include "Hazard/vendor/Coral/Coral.Native"

group ""

include "Hazard-Utility"
include "Hazard-Renderer"
include "Hazard-UI"
include "HazardEditor"
include "Hazard"

if os.host() == "windows" then
	include "HazardScripting"
end

include "HazardLauncher"
include "Hazard-Test-App"
include "Hazard-Script"
