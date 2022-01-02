
include "Dependencies.lua"

workspace "Hazard"
	startproject "HazardEditor"

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

	filter "system:windows"
		architecture "x86_64"

	filter "system:macosx"
		architecture "universal"

	filter ""

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Hazard/vendor/Glad"
	include "Hazard/vendor/GLFW"
	include "Hazard/vendor/Box2D"
	include "Hazard/vendor/yaml-cpp"
	include "HazardEditor/vendor/imgui"
group ""

include "Hazard"
include "HazardEditor"
include "HazardLauncher"

if os.host() == "windows" then
include "HazardScripting"
end