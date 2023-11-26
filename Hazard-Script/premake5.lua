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
        (Dependencies.HazardUtility.IncludeDir),
        (Dependencies.GLM.IncludeDir),
        (Dependencies.Spdlog.IncludeDir),
        (Dependencies.Coral.IncludeDir),
	}
