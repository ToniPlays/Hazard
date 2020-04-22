workspace "Hazard"
	architecture 'x64'

	configurations {
		"Debug",
		"Release",
		"Dist"
	}
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project "IMGUI"
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
		"c:/dev/hazard/vendor/IMGUI/imgui_widgets.cpp"
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
		buildoptions { "-std=c11", "-lgdi32" }
		systemversion "latest"
		staticruntime "On"
	filter { "system:windows", "configurations:Release" } 
		buildoptions "/MT"


project "Hazard"

	location "Hazard"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "hzrpch.h"
	pchsource "Hazard/src/hzrpch.cpp"


	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"c:/dev/Hazard/vendor/spdlog/include",
		"c:/dev/Hazard/vendor/GLFW/include",
		"c:/dev/Hazard/vendor/GLAD/include",
		"Hazard/src"
	}

	links {
		"C:/dev/Hazard/vendor/glfw/lib-vc2019/glfw3.lib",
		"C:/dev/Hazard/vendor/glfw/lib-vc2019/glfw3dll.lib",
		"msvcrt.lib",
		"opengl32.lib",
		"GLAD"
	}

	filter "system:windows"
		cppdialect "default"
		staticruntime "On"
		systemversion "latest"

		defines {
			"HZR_PLATFORM_WINDOWS",
			"HZR_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{COPY} C:/dev/Hazard/bin/" .. outputdir .. "/Hazard/ C:/dev/Hazard/bin/" .. outputdir .."/HazardEditor")
		}


	filter "configurations:Debug"
		defines "HZR_DEBUG"

	filter "configurations:Release"
		defines "HZR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HZR_DIST"
		optimize "On"

project "HazardEditor"
	location "HazardEditor"
	kind "ConsoleApp"

	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"c:/dev/Hazard/vendor/spdlog/include",
		"c:/dev/Hazard/vendor/GLFW/include",
		"c:/dev/Hazard/vendor/GLAD/include",
		"c:/dev/Hazard/vendor/IMGUI",
		"HazardEditor/src",
		"Hazard/src"
	}

	filter "system:windows"
		cppdialect "default"
		staticruntime "On"
		systemversion "latest"

		defines {
			"HZR_PLATFORM_WINDOWS"
		}
	links {
		"C:/dev/Hazard/vendor/glfw/lib-vc2019/glfw3.lib",
		"C:/dev/Hazard/vendor/glfw/lib-vc2019/glfw3dll.lib",
		"msvcrt.lib",
		"opengl32.lib",
		"Hazard",
		"GLAD",
		"IMGUI",

	}

	filter "configurations:Debug"
		defines "HZR_DEBUG"

	filter "configurations:Release"
		defines "HZR_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HZR_DIST"
		optimize "On"
