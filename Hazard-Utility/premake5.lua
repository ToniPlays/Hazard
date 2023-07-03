project "Hazard-Utility"

	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"examples/**.cpp"
	}

	includedirs
	{
		"src",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Optick}",
		"%{IncludeDir.PortableFileDialogs}",
		"%{wks.location}/Hazard/vendor/spdlog/include",
	}

	links 
	{
		"yaml-cpp"
	}