workspace "Hazard"
	architecture 'x64'

	configurations {
		"Debug",
		"Release",
		"Dist",
		"Game"
	}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "yaml-cpp"
	location "yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"vendor/yaml-cpp/src/**.h",
		"vendor/yaml-cpp/src/**.cpp"
	}

	includedirs	{
		"vendor/yaml-cpp/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Release"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "IMGUI"
	location "ImGui"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}");
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"vendor/IMGUI-docking/imconfig.h",
		"vendor/IMGUI-docking/imgui.h",
		"vendor/IMGUI-docking/imgui_internal.h",
		"vendor/IMGUI-docking/imstb_rectpack.h",
		"vendor/IMGUI-docking/imstb_textedit.h",
		"vendor/IMGUI-docking/imstb_truetype.h",
		"vendor/IMGUI-docking/imgui.cpp",
		"vendor/IMGUI-docking/imgui_demo.cpp",
		"vendor/IMGUI-docking/imgui_draw.cpp",
		"vendor/IMGUI-docking/imgui_tables.cpp",
		"vendor/IMGUI-docking/imgui_widgets.cpp"
	}
	includedirs {
		"vendor/IMGUI-docking",
		"vendor/Vulkan/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "GLFW"
	location "GLFW"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}");
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"vendor/glfw/include/GLFW/glfw3.h",
		"vendor/glfw/include/GLFW/glfw3native.h",
		"vendor/glfw/src/glfw_config.h",
		"vendor/glfw/src/context.c",
		"vendor/glfw/src/init.c",
		"vendor/glfw/src/input.c",
		"vendor/glfw/src/monitor.c",
		"vendor/glfw/src/vulkan.c",
		"vendor/glfw/src/window.c"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

		files
		{
			"vendor/glfw/src/win32_init.c",
			"vendor/glfw/src/win32_joystick.c",
			"vendor/glfw/src/win32_monitor.c",
			"vendor/glfw/src/win32_time.c",
			"vendor/glfw/src/win32_thread.c",
			"vendor/glfw/src/win32_window.c",
			"vendor/glfw/src/wgl_context.c",
			"vendor/glfw/src/egl_context.c",
			"vendor/glfw/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

project "GLAD"
	location "GLAD"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}");
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"vendor/glad/include/glad/glad.h",
		"vendor/glad/include/KHR/khrplatform.h",
		"vendor/glad/src/glad.c"
	}

	includedirs {
		"vendor/glad/include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

project "Box2D"
	location "Box2D"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}");
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"vendor/box2d/include/**.h",
		"vendor/box2d/include/**.cpp",
		"vendor/box2d/src/**.h",
		"vendor/box2d/src/**.cpp"
	}

	includedirs {
		"vendor/box2d/include",
		"vendor/box2d/src"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
project "Hazard"

	location "Hazard"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzrpch.h"
	pchsource "Hazard/src/hzrpch.cpp"


	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/**.cpp",
		"%{prj.name}/vendor/**.h",
		"%{prj.name}/vendor/**.hpp",
		"%{prj.name}/vendor/**.inl"
	}

	includedirs {
		"vendor/spdlog/include",
		"vendor/GLFW/include",
		"vendor/GLAD/include",
		"vendor/Vulkan/include",
		"vendor/yaml-cpp/include",
		"vendor/GLM",
		"Hazard/vendor/stb-image",
		"Hazard/vendor/assimp/include",
		"vendor/entt/single_include/entt",
		"vendor/mono/include",
		"vendor/SPIR-V/include",
		"vendor/OpenAL/include",
		"vendor/minimp3",
		"vendor/libogg/include",
		"vendor/Vorbis/include",
		"vendor/box2d/include",
		"vendor/VMA/include",
		"Hazard/src"
	}

	links {
		"C:/VulkanSDK/1.2.176.2/Lib/vulkan-1.lib",
		"vendor/SPIR-V/libs/shaderc_shared.lib",
		"msvcrt.lib",
		"opengl32.lib",
		"mono-2.0-sgen.dll",
		"GLAD",
		"yaml-cpp",
		"Box2D",
		"GLFW"
	}

	filter "system:windows"
		systemversion "latest"

		defines {
			"HZR_PLATFORM_WINDOWS",
			"HZR_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"AL_LIBTYPE_STATIC"
		}

	filter "configurations:Debug"
		defines "HZR_DEBUG"
		cppdialect "C++17"

	filter "configurations:Release"
		defines "HZR_RELEASE"
		optimize "On"
		cppdialect "C++17"

	filter "configurations:Dist"
		defines "HZR_DIST"
		optimize "On"
		cppdialect "C++17"

project "HazardEditor"
	location "HazardEditor"
	kind "WindowedApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzreditor.h"
	pchsource "HazardEditor/src/hzreditor.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/platform/**.h",
		"%{prj.name}/platform/**.cpp",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp"
	}

	includedirs {
		"vendor/imgui-node",
		"vendor/spdlog/include",
		"vendor/GLFW/include",
		"vendor/GLAD/include",
		"vendor/Vulkan/include",
		"vendor/yaml-cpp/include",
		"Hazard/vendor/assimp/include",
		"vendor/box2d/include",
		"vendor/IMGUI-docking",
		"vendor/OpenAL/lib/openAl32.lib",
		"vendor/ImGuizmo",
		"vendor/GLM",
		"vendor/entt/single_include/entt",
		"HazardEditor/Platform/GLFW",
		"HazardEditor/src",
		"Hazard/src",
		"Hazard/GLM/glm"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines {
		"HZR_PLATFORM_WINDOWS",
		"_CRT_SECURE_NO_WARNINGS"
	}

	links {
		"vendor/OpenAL/lib/openal32.lib",
		"Hazard/vendor/assimp/lib/assimp-vc142-mt.lib",
		"C:/VulkanSDK/1.2.176.2/Lib/vulkan-1.lib",
		"vendor/mono/lib/Debug/mono-2.0-sgen.lib",
		"yaml-cpp",
		"msvcrt.lib",
		"opengl32.lib",
		"Hazard",
		"GLAD",
		"IMGUI",
		"GLFW"
	}

	filter "files:platform/**.cpp"
	flags { "NoPCH" }
	filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

	filter "configurations:Debug"
		defines "HZR_DEBUG"

	filter "configurations:Release"
		defines "HZR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HZR_DIST"
		optimize "On"

project "HazardPlayer"
	location "HazardPlayer"
	kind "WindowedApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "playerpch.h"
	pchsource "HazardEditor/src/playerpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/platform/**.h",
		"%{prj.name}/platform/**.cpp"
	}

	includedirs {
		"vendor/imgui-node",
		"vendor/spdlog/include",
		"vendor/GLFW/include",
		"vendor/GLAD/include",
		"vendor/Vulkan/include",
		"vendor/yaml-cpp/include",
		"Hazard/vendor/assimp/include",
		"vendor/box2d/include",
		"vendor/OpenAL/lib/openAl32.lib",
		"vendor/GLM",
		"vendor/entt/single_include/entt",
		"HazardEditor/Platform/GLFW",
		"HazardPlayer/src",
		"Hazard/src",
		"Hazard/GLM/glm"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines {
		"HZR_PLATFORM_WINDOWS",
		"_CRT_SECURE_NO_WARNINGS"
	}

	links {
		"vendor/OpenAL/lib/openal32.lib",
		"Hazard/vendor/assimp/lib/assimp-vc142-mt.lib",
		"C:/VulkanSDK/1.2.176.2/Lib/vulkan-1.lib",
		"vendor/mono/lib/Debug/mono-2.0-sgen.lib",
		"yaml-cpp",
		"msvcrt.lib",
		"opengl32.lib",
		"Hazard",
		"GLAD",
		"GLFW"
	}

	filter "configurations:Debug"
		defines "HZR_DEBUG"

	filter "configurations:Release"
		defines "HZR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HZR_DIST"
		optimize "On"
