project "Hazard-Renderer"

	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/VulkanMemoryAllocator/**.h",
		"vendor/VulkanMemoryAllocator/**.cpp",
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
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.SPIRV_Cross}",
		"%{IncludeDir.shaderc}",
		"%{IncludeDir.VMA}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.Optick}",
		"%{IncludeDir.PortableFileDialogs}",
		"%{wks.location}/Hazard/vendor/spdlog/include",
		"src",
		"examples"
	}
	links {
		"GLFW",
		"GLAD",
		"Hazard-Utility"
	}

	defines {
		"GLFW_INCLUDE_NONE"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_WINDOWS"
		}
		links {
			"%{Library.Vulkan}",
			"%{Library.VulkanUtils}",
			"opengl32.lib"
		}


	filter "system:macosx"
		defines {
			"HZR_PLATFORM_MACOS",
			"GLFW_INCLUDE_NONE"
		}
		links { 
			"IOKit.framework",
			"CoreFoundation.framework",
			"Cocoa.framework",
			"QuartzCore.framework",
			"Metal.framework",
			"MetalKit.framework"
		}
		includedirs {
			"%{IncludeDir.Metal}"
		}
		files {
			"src/**.mm"
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
