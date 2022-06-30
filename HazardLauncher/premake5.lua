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
		"%{IncludeDir.glm}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.Entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.Mono}",
		"%{IncludeDir.SPIRV_Cross}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.VMA}",
		"%{IncludeDir.PortableFileDialogs}",
		"%{IncludeDir.Hazard_Utility}",
		"%{IncludeDir.Hazard_Renderer}",
		"%{IncludeDir.Hazard_Script}",
		"%{IncludeDir.Hazard_UI}",
		"src"
	}
	links {
		"%{Library.Mono_Debug_Lib}",
		"Hazard-Utility",
		"Hazard-Renderer",
		"Hazard-Script",
		"Hazard-UI",
		"Glad",
		"GLFW"
	}
	defines {
		"GLFW_INCLUDE_NONE"

	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_WINDOWS"
		}

	filter "system:macosx"
		defines {
			"HZR_PLATFORM_MACOS"
		}
		links { 
			"IOKit.framework",
			"CoreFoundation.framework",
			"Cocoa.framework",
			"Metal.framework",
			"MetalKit.framework",
			"QuartzCore.framework",
			"Quartz.framework"
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
