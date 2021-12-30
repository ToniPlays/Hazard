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
		"src",
	}

	links
	{
		"ImGui",
		"Hazard",
		"mono-2.0",
		"assimp",
		"vulkan",
		"glfw3",
		"Glad",
		"Box2D",
		"yaml-cpp",
		"shaderc_combined",
		"spirv-cross-core",
		"spirv-cross-glsl",
		"spirv-cross-hlsl",
		"spirv-cross-c",
		"spirv-cross-cpp",
		"spirv-cross-reflect",
		"SPIRV-Tools",
		"glslang",
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
		links {"IOKit.framework", "CoreFoundation.framework", "Cocoa.framework", "OpenGL.framework"}

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
		