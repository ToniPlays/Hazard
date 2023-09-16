project "Hazard-Script"

	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Mono}",
		"%{IncludeDir.Hazard_Utility}",
		"%{IncludeDir.Optick}",
		"%{IncludeDir.Coral}"
	}

	links 
	{
		"%{LibraryDir.CoralNetHost}",
		"Hazard-Utility",
		"Coral.Native"
	}