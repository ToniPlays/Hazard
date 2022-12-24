project "HazardLauncher"

	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"%{wks.location}/Hazard/vendor/vendor/ImGui_Backend/**.h",
		"%{wks.location}/Hazard/vendor/ImGui_Backend/**.cpp"
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
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.VulkanSDK}",
        	"%{IncludeDir.Metal}",
		"%{IncludeDir.SPIRV_Cross}",
		"%{IncludeDir.Hazard_Utility}",
		"%{IncludeDir.Hazard_Renderer}",
		"%{IncludeDir.Hazard_Script}",
		"%{IncludeDir.Hazard_UI}",
		"%{IncludeDir.PortableFileDialogs}",
		"%{IncludeDir.VMA}",
		"%{IncludeDir.Optick}",
		"src"
	}
	links {
		"ImGui",
		"Hazard",
		"GLFW",
		"Glad",
		"Box2D",
		"yaml-cpp",
		"Optick"
	}
	defines {
		"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_WINDOWS",
			"HZR_INCLUDE_MONO"
		}
		links {
			"%{Library.Vulkan}",
			"%{Library.VulkanUtils}",
			"%{Library.Mono_Debug_Lib}",
			"%{Library.Assimp_Lib}",
			"Hazard-Script"
		}
		includedirs {

			"%{IncludeDir.Optick}",
			"%{IncludeDir.Mono}"
		}
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\"",
			"{COPY} %{wks.location}/Hazard/vendor/assimp/lib/assimp-vc142-mt.dll %{cfg.targetdir}",
			"{COPY} %{wks.location}/scripts/res/mono-2.0-sgen.dll %{cfg.targetdir}"
		}

	filter "system:macosx"
		defines {
			"HZR_PLATFORM_MACOS",
		}
		links {
			"IOKit.framework",
			"CoreFoundation.framework",
			"Cocoa.framework",
			"Metal.framework",
			"MetalKit.framework"
		}
        	files {
               		"src/**.m",
                	"src/**.mm"
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
