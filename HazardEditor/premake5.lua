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
		"src",
	}

	links
	{
		"%{Library.Mono_Debug_Lib}",
		"%{Library.Assimp_Lib}",
		"%{Library.Vulkan}",
		"GLFW",
		"Glad",
		"Box2D",
		"yaml-cpp",
		"Hazard",
		"ImGui"
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
		links {"IOKit.framework", "CoreFoundation.framework", "Cocoa.framework"}
		links {"shaderc_util"}

	filter "configurations:Debug"
		defines "HZR_DEBUG"
		runtime "Debug"
		symbols "on"

		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
		}

		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

	filter "configurations:Release"
		defines "HZR_RELEASE"
		runtime "Release"
		optimize "on"
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

	filter "configurations:Dist"
		defines "HZR_DIST"
		runtime "Release"
		optimize "on"
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
