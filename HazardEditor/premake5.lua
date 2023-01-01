project "HazardEditor"

	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp",
		"%{wks.location}/Hazard/vendor/ImGui_Backend/**.h",
		"%{wks.location}/Hazard/vendor/ImGui_Backend/**.cpp"
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
        	"%{IncludeDir.Metal}",
		"%{IncludeDir.shaderc}",
		"%{IncludeDir.Assimp}",
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

	links
	{
		"ImGui",
		"Hazard",
		"Hazard-Script",
		"GLFW",
		"Glad",
		"Box2D",
		"yaml-cpp"
	}

	postbuildcommands {
		"{COPYDIR} \"%{wks.location}/HazardEditor/res\" \"%{cfg.targetdir}/res\""
	}


	filter "system:windows"
		defines 
		{
			"HZR_INCLUDE_MONO"
		}
		links {
			"%{Library.Vulkan}",
			"%{Library.VulkanUtils}",
			"Hazard-Script",
			"Optick",
			"%{Library.Mono_Debug_Lib}",
			"%{Library.Assimp_Lib}",
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
		links 
		{
			"IOKit.framework",
			"CoreFoundation.framework",
			"Cocoa.framework",
			"Metal.framework",
			"MetalKit.framework"
		}
        	files 
		{
                	"src/**.m",
                	"src/**.mm",
			"%{wks.location}/Hazard/vendor/ImGui_Backend/**.m",
			"%{wks.location}/Hazard/vendor/ImGui_Backend/**.mm"
        	}
