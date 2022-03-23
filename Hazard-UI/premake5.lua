project "Hazard-UI"

	kind "StaticLib"
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
		"%{IncludeDir.Hazard_Utility}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Entt}",
		"src"
	}
	links {
		"Hazard-Utility",
		"Hazard-Renderer"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"HZR_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}
		links {
			"%{Library.Vulkan}",
			"%{Library.VulkanUtils}",
			"opengl32.lib"
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
