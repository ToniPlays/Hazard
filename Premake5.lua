include "Dependencies.lua"

workspace "Hazard"
	architecture "x86_64"
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

	filter "system:macosx"
		architecture "universal"

	filter ""

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "Hazard/vendor/Glad"
	include "Hazard/vendor/GLFW"
	include "Hazard/vendor/Box2D"

if os.host() == "windows" then
	include "Hazard/vendor/Optick"
end

	include "Hazard/vendor/yaml-cpp"
	include "HazardEditor/vendor/imgui"
group ""

include "Hazard"
include "Hazard-Utility"
include "Hazard-Renderer"
include "Hazard-UI"
include "HazardEditor"
include "HazardLauncher"

if os.host() == "windows" then
include "HazardScripting"
end
