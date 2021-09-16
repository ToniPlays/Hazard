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
	location "c:/dev/Hazard/yaml-cpp"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"C:/dev/Hazard/vendor/yaml-cpp/src/**.h",
		"C:/dev/Hazard/vendor/yaml-cpp/src/**.cpp"
	}

	includedirs	{
		"C:/dev/Hazard/vendor/yaml-cpp/include"
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
	location "c:/dev/Hazard/ImGui"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}");
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"c:/dev/hazard/vendor/IMGUI/imconfig.h",
		"c:/dev/hazard/vendor/IMGUI/imgui.h",
		"c:/dev/hazard/vendor/IMGUI/imgui_internal.h",
		"c:/dev/hazard/vendor/IMGUI/imstb_rectpack.h",
		"c:/dev/hazard/vendor/IMGUI/imstb_textedit.h",
		"c:/dev/hazard/vendor/IMGUI/imstb_truetype.h",
		"c:/dev/hazard/vendor/IMGUI/imgui.cpp",
		"c:/dev/hazard/vendor/IMGUI/imgui_demo.cpp",
		"c:/dev/hazard/vendor/IMGUI/imgui_draw.cpp",
		"c:/dev/hazard/vendor/IMGUI/imgui_tables.cpp",
		"c:/dev/hazard/vendor/IMGUI/imgui_widgets.cpp",
		"c:/dev/hazard/vendor/IMGUI/used_backends/imgui_impl_vulkan.h",
		"c:/dev/hazard/vendor/IMGUI/used_backends/imgui_impl_vulkan.cpp"
	}
	includedirs {
		"C:/dev/hazard/vendor/IMGUI",
		"C:/dev/hazard/vendor/Vulkan/include"
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

project "GLAD"
	location "c:/dev/Hazard/GLAD"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}");
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"c:/dev/Hazard/vendor/glad/include/glad/glad.h",
		"c:/dev/Hazard/vendor/glad/include/KHR/khrplatform.h",
		"c:/dev/Hazard/vendor/glad/src/glad.c"
	}

	includedirs {
		"c:/dev/Hazard/vendor/glad/include"
	}

	filter "system:windows"
		systemversion "latest"
		staticruntime "On"

project "Box2D"
	location "c:/dev/Hazard/Box2D"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}");
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"c:/dev/Hazard/vendor/box2d/include/**.h",
		"c:/dev/Hazard/vendor/box2d/include/**.cpp",
		"c:/dev/Hazard/vendor/box2d/src/**.h",
		"c:/dev/Hazard/vendor/box2d/src/**.cpp"
	}

	includedirs {
		"c:/dev/Hazard/vendor/box2d/include",
		"c:/dev/Hazard/vendor/box2d/src"
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
		"c:/dev/Hazard/vendor/spdlog/include",
		"c:/dev/Hazard/vendor/GLFW/include",
		"c:/dev/Hazard/vendor/GLAD/include",
		"c:/dev/Hazard/vendor/Vulkan/include",
		"c:/dev/Hazard/vendor/JSON/include",
		"c:/dev/Hazard/vendor/yaml-cpp/include",
		"c:/dev/Hazard/vendor/GLM",
		"c:/dev/Hazard/Hazard/vendor/stb-image",
		"c:/dev/Hazard/Hazard/vendor/assimp/include",
		"c:/dev/Hazard/vendor/entt",
		"c:/dev/Hazard/vendor/IMGUI/examples",
		"c:/dev/Hazard/vendor/JSON-develop/include",
		"c:/dev/Hazard/vendor/mono/include",
		"c:/dev/Hazard/vendor/SPIR-V/include",
		"c:/dev/Hazard/vendor/OpenAL/include",
		"C:/dev/Hazard/vendor/minimp3",
		"C:/dev/Hazard/vendor/libogg/include",
		"C:/dev/Hazard/vendor/Vorbis/include",
		"C:/dev/Hazard/vendor/box2d/include",
		"Hazard/src"
	}

	links {
		"C:/dev/Hazard/vendor/glfw/lib-vc2019/glfw3.lib",
		"C:/VulkanSDK/1.2.176.1/Lib/vulkan-1.lib",
		"C:/dev/Hazard/vendor/SPIR-V/libs/shaderc_shared.lib",
		"msvcrt.lib",
		"opengl32.lib",
		"mono-2.0-sgen.dll",
		"GLAD",
		"yaml-cpp",
		"Box2D",
		"c:/dev/Hazard/vendor/mono/lib/Debug/eglib.lib",
		"c:/dev/Hazard/vendor/mono/lib/Debug/mono-2.0-sgen.lib",
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
		"c:/dev/Hazard/vendor/ImGuizmo/ImGuizmo.h",
		"c:/dev/Hazard/vendor/ImGuizmo/ImGuizmo.cpp",
		"c:/dev/Hazard/vendor/ImGui-node/*.h",
		"c:/dev/Hazard/vendor/ImGui-node/*.cpp"
	}

	includedirs {
		"c:/dev/Hazard/vendor/imgui-node",
		"c:/dev/Hazard/vendor/spdlog/include",
		"c:/dev/Hazard/vendor/GLFW/include",
		"c:/dev/Hazard/vendor/GLAD/include",
		"c:/dev/Hazard/vendor/Vulkan/include",
		"c:/dev/Hazard/vendor/JSON/include",
		"c:/dev/Hazard/vendor/yaml-cpp/include",
		"c:/dev/Hazard/vendor/JSON-develop/include",
		"c:/dev/Hazard/Hazard/vendor/assimp/include",
		"C:/dev/Hazard/vendor/box2d/include",
		"c:/dev/Hazard/vendor/IMGUI",
		"c:/dev/Hazard/vendor/IMGUI/used_backends",
		"C:/dev/Hazard/vendor/OpenAL/lib/openAl32.lib",
		"c:/dev/Hazard/vendor/ImGuizmo",
		"c:/dev/Hazard/vendor/GLM",
		"c:/dev/Hazard/vendor/entt",
		"HazardEditor/Platform",
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
		"C:/dev/Hazard/vendor/glfw/lib-vc2019/glfw3.lib",
		"C:/dev/Hazard/vendor/OpenAL/lib/openal32.lib",
		"C:/dev/Hazard/Hazard/vendor/assimp/lib/assimp-vc142-mt.lib",
		"C:/VulkanSDK/1.2.176.1/Lib/vulkan-1.lib",
		"yaml-cpp",
		"msvcrt.lib",
		"opengl32.lib",
		"Hazard",
		"GLAD",
		"IMGUI"
	}

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
		"c:/dev/Hazard/vendor/spdlog/include",
		"c:/dev/Hazard/vendor/GLFW/include",
		"c:/dev/Hazard/vendor/GLAD/include",
		"c:/dev/Hazard/vendor/Vulkan/include",
		"c:/dev/Hazard/vendor/JSON/include",
		"c:/dev/Hazard/vendor/yaml-cpp/include",
		"c:/dev/Hazard/vendor/JSON-develop/include",
		"c:/dev/Hazard/Hazard/vendor/assimp/include",
		"C:/dev/Hazard/vendor/OpenAL/lib/openAl32.lib",
		"c:/dev/Hazard/vendor/GLM",
		"c:/dev/Hazard/vendor/entt",
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
		"C:/dev/Hazard/vendor/glfw/lib-vc2019/glfw3.lib",
		"C:/dev/Hazard/vendor/OpenAL/lib/openal32.lib",
		"C:/dev/Hazard/Hazard/vendor/assimp/lib/assimp-vc142-mt.lib",
		"C:/VulkanSDK/1.2.176.1/Lib/vulkan-1.lib",
		"yaml-cpp",
		"msvcrt.lib",
		"opengl32.lib",
		"Hazard",
		"GLAD"
	}

	filter "configurations:Debug"
		defines "HZR_DEBUG"

	filter "configurations:Release"
		defines "HZR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HZR_DIST"
		optimize "On"
