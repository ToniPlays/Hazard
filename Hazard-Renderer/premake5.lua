project "Hazard-Renderer"

	kind "ConsoleApp"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.VMA}",
		"%{IncludeDir.Hazard_Utility}",
		"%{IncludeDir.yaml_cpp}",
		"src"
	}
	links {
		"GLFW",
		"GLAD",
		"Hazard-Utility"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}
	filter "system:macos"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_MACOS",
			"GLFW_INCLUDE_NONE"
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
