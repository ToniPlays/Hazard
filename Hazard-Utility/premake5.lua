project "Hazard-Utility"

	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
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
		"%{IncludeDir.glm}"
	}

	links {
		"yaml-cpp"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_WINDOWS"
		}
	filter "system:macos"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_MACOS"
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
