
-- Hazard Dependencies

VULKAN_SDK = "/Users/tonisimoska/VulkanSDK/1.3.216.0/macos"

Dependencies = {
    YAML = {
        IncludeDir = "%{wks.location}/Hazard/vendor/yaml-cpp/include",
        WindowsLibs = {
            Debug = { "yaml-cpp" },
            Release = { "yaml-cpp" }
        },
        MacosLibs = {
            Debug = { "yaml-cpp" },
            Release = { "yaml-cpp" }
        }
    },
    GLM = {
        IncludeDir = "%{wks.location}/Hazard/vendor/glm"
    },
    Spdlog = {
        IncludeDir = "%{wks.location}/Hazard/vendor/spdlog/include"
    },
    Box2D = {
        IncludeDir = "%{wks.location}/Hazard/vendor/Box2D/include"
    },
    EnTT = {
        IncludeDir = "%{wks.location}/Hazard/vendor/entt/include"
    },
    PortableFD = {
        IncludeDir = "%{wks.location}/Hazard/vendor/portable-file-dialogs"
    },
    Assimp = {
        IncludeDir = "%{wks.location}/Hazard/vendor/assimp/include",
        WindowsLibs = {
            Debug = {"%{wks.location}/Hazard/vendor/assimp/lib/assimp-vc142-mt.lib"},
            Release = {"%{wks.location}/Hazard/vendor/assimp/lib/assimp-vc142-mt.lib"}
        }
    },
    VMA = {
        IncludeDir = "%{wks.location}/Hazard-Renderer/vendor/VulkanMemoryAllocator"
    },
    ImGUI = {
        IncludeDir = "%{wks.location}/Hazard/vendor/ImGui",
        WindowsLibs = {
            Debug = { "ImGui" },
            Release = { "ImGui" }
        },
        MacosLibs = {
            Debug = { "ImGui" },
            Release = { "ImGui" }
        }
    },
    OpenGL = {},
    Vulkan = {
        IncludeDir = "%{VULKAN_SDK}/Include"
    },
    Metal = {
        IncludeDir = "%{wks.location}/Hazard/vendor/Metal-CPP",
        MacosLibs = {
            Debug = {"Cocoa.framework", "Foundation.framework", "CoreFoundation.framework", "Metal.framework", "MetalKit.framework", "IOKit.framework", "Cocoa.framework", "QuartzCore.framework"}
        }
    },
    ShaderC = {
        IncludeDir = "%{wks.location}/Hazard/vendor/shaderc/include"
    },
    SpirvCross = {
        IncludeDir = "%{VULKAN_SDK}/Include",
        LibraryDir = "%{wks.location}/Hazard/libmacos",
        MacosLibs = {
            Debug = { "libspirv-cross-core.a", "libspirv-cross-glsl.a", "libspirv-cross-msl.a", "libspirv-cross-reflect.a" },
            Release = {}
        }
    },
    StbImage = {
        IncludeDir = "%{wks.location}/Hazard/vendor/stb_image"
    },
    GLAD = {
        IncludeDir = "%{wks.location}/Hazard/vendor/Glad/include",
        WindowsLibs = {
            Debug = { "Glad" },
            Release = { "Glad" }
        },
        MacosLibs = {
            Debug = { "Glad" },
            Release = { "Glad" }
        }
    },
    GLFW = {
        IncludeDir = "%{wks.location}/Hazard/vendor/GLFW/include",
        WindowsLibs = {
            Debug = { "Glfw" },
            Release = { "Glfw" }
        },
        MacosLibs = {
            Debug = { "Glfw" },
            Release = { "Glfw" }
        }
    },
    Coral = {
        IncludeDir = "%{wks.location}/Hazard/vendor/Coral/Coral.Native/Include",
        WindowsLibs = {
            Debug = { "Coral.Native" },
            Release = { "Coral.Native" }
        },
        MacosLibs = {
            Debug = { "Coral.Native" },
            Release = { "Coral.Native" }
        }
    },
    HazardUtility = {
        IncludeDir = "%{wks.location}/Hazard-Utility/src",
        WindowsLibs = {
            Debug = { "Hazard-Utility" },
            Release = { "Hazard-Utility" }
        },
        MacosLibs = {
            Debug = { "Hazard-Utility" },
            Release = { "Hazard-Utility" }
        }
    },
    HazardScript = {
        IncludeDir = "%{wks.location}/Hazard-Script/src",
        WindowsLibs = {
            Debug = { "Hazard-Script" },
            Release = { "Hazard-Script" }
        },
        MacosLibs = {
            Debug = { "Hazard-Script" },
            Release = { "Hazard-Script" }
        }
    },
    HazardRenderer = {
        IncludeDir = "%{wks.location}/Hazard-Renderer/src",
        WindowsLibs = {
            Debug = { "Hazard-Renderer" },
            Release = { "Hazard-Renderer" }
        },
        MacosLibs = {
            Debug = { "Hazard-Renderer" },
            Release = { "Hazard-Renderer" }
        }
    },
    Hazard = {
        IncludeDir = "%{wks.location}/Hazard/src",
        WindowsLibs = {
            Debug = { "Hazard" },
            Release = { "Hazard" }
        },
        MacosLibs = {
            Debug = { "Hazard" },
            Release = { "Hazard" }
        }
    }
}

function References(reference)
    ref = Dependencies[reference]
    
    if(ref.IncludeDir ~= nil) then
        filter ""
        includedirs { ref.IncludeDir }
    end
    
    if(ref.LibraryDir ~= nil) then
        print(ref.LibraryDir)
        libdirs { ref.LibraryDir }
    end
    
    if(ref.MacosLibs ~= nil) then
        for key, value in pairs(ref.MacosLibs) do
            filter (string.format("configurations:%s", key))
            for k, lib in pairs(value) do
                links { lib }
            end
        end
    end
end

premake.override(_G, "project", function(base, ...)
	local rval = base(...)
	local args = {...}
	filter "system:ios"
		filename(args[1] .. "-iOS")
	filter {}
	return rval
	end)

premake.override(_G, "workspace", function(base, ...)
	local rval = base(...)
	local args = {...}
	filter "system:ios"
		filename(args[1] .. "-iOS")
	filter {}
	return rval
	end)

