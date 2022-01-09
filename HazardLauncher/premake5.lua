project "HazardLauncher"

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
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
		"%{wks.location}/Hazard/vendor/spdlog/include",
		"%{wks.location}/Hazard/src",
		"%{wks.location}/Hazard/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.VMA}",
		"%{IncludeDir.PortableFileDialogs}",
		"src",
	}
	links {
		"Hazard"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_WINDOWS"
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
