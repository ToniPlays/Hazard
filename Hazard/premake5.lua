project "Hazard"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")


	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
		"vendor/VulkanMemoryAllocator/**.h",
		"vendor/VulkanMemoryAllocator/**.cpp",
		"vendor/ImGui_Backend/**.h",
		"vendor/ImGui_Backend/**.cpp"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"IMGUI_IMPL_OPENGL_LOADER_GLAD",
		"_CRT_SECURE_NO_WARNINGS"
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
		"%{IncludeDir.glm}",
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Entt}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.VulkanSDK}",
		"%{IncludeDir.SPIRV_Cross}",
		"%{IncludeDir.shaderc}",
		"%{IncludeDir.VMA}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.Hazard_Utility}",
		"%{IncludeDir.Hazard_Renderer}",
		"%{IncludeDir.Hazard_Script}",
		"%{IncludeDir.Hazard_UI}",
		"%{IncludeDir.PortableFileDialogs}",
		"%{IncludeDir.Coral}"
	}

	links {
		"ImGui",
		"yaml-cpp",
		"Box2D",
		"Hazard-Renderer",
		"HazardScripting",
		"%{Library.Assimp_Lib}"
	}

	filter "system:windows"
		pchheader "hzrpch.h"
		defines 
		{
			"HZR_INCLUDE_MONO"
		}

		links {
			"%{Library.Vulkan}",
			"%{Library.VulkanUtils}",
			"opengl32.lib",
			"Hazard-Script"
		}
		includedirs {
			"%{IncludeDir.Optick}",
			"%{IncludeDir.Mono}"
		}

	filter "system:macosx or system:ios"
		pchheader "src/hzrpch.h"
		includedirs {
			"%{IncludeDir.SPIRV_Cross}",
			"%{IncludeDir.Metal}"
		}
		files
		{
			"src/**.m",
			"src/**.mm",
			"vendor/ImGui_Backend/**.m",
			"vendor/ImGui_Backend/**.mm"
		}

	filter "system:windows or system:macosx"
		links
		{
			"GLFW",
			"Glad"
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
