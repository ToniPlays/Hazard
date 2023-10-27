project "Hazard-Test-App"

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
		"%{IncludeDir.VulkanSDK}",
        	"%{IncludeDir.Metal}",
		"%{IncludeDir.shaderc}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.SPIRV_Cross}",
		"%{IncludeDir.Hazard_Utility}",
		"%{IncludeDir.Hazard_Renderer}",
		"%{IncludeDir.Hazard_Script}",
		"%{IncludeDir.Hazard_UI}",
		"%{IncludeDir.PortableFileDialogs}",
		"%{IncludeDir.VMA}",
		"%{IncludeDir.Coral}",
		"%{IncludeDir.Optick}",
		"src"
	}

	links
	{
		"Hazard",
		"Hazard-Script",
		"Box2D",
		"yaml-cpp"
	}

	filter "system:windows"
		kind "ConsoleApp"
		defines {
			"_CRT_SECURE_NO_WARNINGS",
		}
		links {
			"%{Library.Vulkan}",
			"Hazard-Script",
			"%{Library.Assimp_Lib}"
		}
		postbuildcommands
		{
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\"",
			"{COPY} %{wks.location}/Hazard/vendor/assimp/lib/assimp-vc142-mt.dll %{cfg.targetdir}",
		}

	filter "system:macosx"
		kind "ConsoleApp"
		links
		{
			"Cocoa.framework"
		}

	filter "system:ios"
		kind "WindowedApp"
		links {
			"UIKit.framework",
		}

	filter "system:macosx or system:ios"
		links 
		{
			"IOKit.framework",
			"Foundation.framework",
			"CoreGraphics.framework",
			"CoreFoundation.framework",
			"QuartzCore.framework",
			"Metal.framework",
			"MetalKit.framework",
			"%{Library.Assimp_Lib}",
		}
		libdirs {
			"%{LibraryDir.Assimp_Lib}",			
		}
        	files 
		{
                	"src/**.m",
                	"src/**.mm",
			"%{wks.location}/Hazard/vendor/ImGui_Backend/**.m",
			"%{wks.location}/Hazard/vendor/ImGui_Backend/**.mm"
        	}
