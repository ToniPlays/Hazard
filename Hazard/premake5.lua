project "Hazard"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchsource "src/hzrpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/VulkanMemoryAllocator/**.h",
		"vendor/VulkanMemoryAllocator/**.cpp"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD"
	}

	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Mono}",
		"%{IncludeDir.Vorbis}",
		"%{IncludeDir.Minimp3}",
		"%{IncludeDir.Libogg}",
		"%{IncludeDir.OpenAL}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Entt}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.SPIRV_Cross}",
		"%{IncludeDir.shaderc}",
		"%{IncludeDir.PortableFileDialogs}",
		"%{IncludeDir.VMA}"
	}

	links {
		"GLFW",
		"Glad",
		"yaml-cpp",
		"Box2D",
		"Hazard-Renderer",
		"%{Library.Assimp_Lib}"
	}

	filter "system:windows"
		systemversion "latest"
		pchheader "hzrpch.h"
		defines {
			"HZR_PLATFORM_WINDOWS",
			"HZR_INCLUDE_OPENGL",
			"HZR_INCLUDE_VULKAN",
			"HZR_INCLUDE_MONO"
		}

		links {
			"%{Library.Vulkan}",
			"%{Library.VulkanUtils}",
			"opengl32.lib"
		}
		removefiles {
			"src/Platform/Rendering/Metal/**"
		}
		includedirs {
			"%{IncludeDir.Optick}"
		}

	filter "system:macosx"

		pchheader "src/hzrpch.h"
		defines {
			"HZR_PLATFORM_MACOS",
			"HZR_INCLUDE_METAL"
		}
		includedirs {
			"%{IncludeDir.SPIRV_Cross}",
			"%{IncludeDir.Metal}"
		}
		removefiles {
			"src/Platform/Rendering/Vulkan/**",
			"src/Platform/Rendering/OpenGL/**",
			"vendor/VulkanMemoryAllocator/**"
		}

	filter "configurations:Debug"
		defines "HZR_DEBUG"
		runtime "Debug"
		symbols "on"

		if os.host() == "windows" then
			links {
				"%{Library.ShaderC_Debug}",
				"%{Library.SPIRV_Cross_Debug}",
				"%{Library.SPIRV_Cross_GLSL_Debug}",
			}
		end

	filter "configurations:Release"
		defines "HZR_RELEASE"
		runtime "Release"
		optimize "on"


		if os.host() == "windows" then
			links {
				"%{Library.ShaderC_Release}",
				"%{Library.SPIRV_Cross_Release}",
				"%{Library.SPIRV_Cross_GLSL_Release}",
			}
		end

	filter "configurations:Dist"
		defines "HZR_DIST"
		runtime "Release"
		optimize "on"

		if os.host() == "windows" then
			links {
				"%{Library.ShaderC_Release}",
				"%{Library.SPIRV_Cross_Release}",
				"%{Library.SPIRV_Cross_GLSL_Release}",
			}
		end
