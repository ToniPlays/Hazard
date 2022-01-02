project "HazardEditor"

	kind "WindowedApp"
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

	defines {
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
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
		"src"
	}

	links
	{
		"ImGui",
		"Hazard",
		"%{Library.Mono_Debug_Lib}",
		"%{Library.Assimp_Lib}",
		"GLFW",
		"Glad"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_WINDOWS",
			"HZR_INCLUDE_OPENGL",
			"HZR_INCLUDE_VULKAN",
			"HZR_INCLUDE_OPENAL"
		}
		links {
			"%{Library.Vulkan}",
			"%{Library.VulkanUtils}",
			"%{Library.OpenAL_Lib}"
		}

	filter "system:macosx"
		defines {
			"HZR_PLATFORM_MACOS",
			"HZR_INCLUDE_METAL"
		}
		links {
			"IOKit.framework",
			"CoreFoundation.framework",
			"Cocoa.framework",
			"OpenGL.framework",
			"Metal.frameWork",
			"MetalKit.framework"
		}

	filter "configurations:Debug"
		defines "HZR_DEBUG"
		runtime "Debug"
		symbols "on"

		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
		}

	filter "configurations:Release"
		defines "HZR_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "HZR_DIST"
		runtime "Release"
		optimize "on"
