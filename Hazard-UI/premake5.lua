project "Hazard-UI"

	kind "StaticLib"
	language "C++"
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
		links {
			"%{Library.Vulkan}",
			"%{Library.VulkanUtils}",
			"opengl32.lib"
		}


	
	filter "configurations:Debug"

		if os.host() == "windows" then
			links {
				"%{Library.ShaderC_Debug}",
				"%{Library.SPIRV_Cross_Debug}",
				"%{Library.SPIRV_Cross_GLSL_Debug}",
			}
		end

	filter "configurations:Release"
		if os.host() == "windows" then
			links {
				"%{Library.ShaderC_Release}",
				"%{Library.SPIRV_Cross_Release}",
				"%{Library.SPIRV_Cross_GLSL_Release}",
			}
		end

	filter "configurations:Dist"
		if os.host() == "windows" then
			links {
				"%{Library.ShaderC_Release}",
				"%{Library.SPIRV_Cross_Release}",
				"%{Library.SPIRV_Cross_GLSL_Release}",
			}
		end
