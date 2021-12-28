
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
