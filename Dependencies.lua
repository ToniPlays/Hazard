
-- Hazard Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

Dependencies = {
    YAML = {
        IncludeDir = "%{wks.location}/Hazard/vendor/yaml-cpp/include",
        CommonLib = "yaml-cpp"
    },
    GLM = {
        IncludeDir = "%{wks.location}/Hazard/vendor/glm"
    },
    Spdlog = {
        IncludeDir = "%{wks.location}/Hazard/vendor/spdlog/include"
    },
    Box2D = {
        IncludeDir = "%{wks.location}/Hazard/vendor/Box2D/include",
        CommonLib = "Box2D"
    },
    EnTT = {
        IncludeDir = "%{wks.location}/Hazard/vendor/entt/include"
    },
    PortableFD = {
        IncludeDir = "%{wks.location}/Hazard/vendor/portable-file-dialogs"
    },
    Assimp = {
        IncludeDir = "%{wks.location}/Hazard/vendor/assimp/include",
        LibraryDir = "%{wks.location}/Hazard/libmacos",
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
        CommonLib = "ImGui"
    },
    OpenGL = {},
    Vulkan = {
        IncludeDir = "%{VULKAN_SDK}/Include",
        LibraryDir = "%{VULKAN_SDK}/Lib",
        WindowsLibs = {
            Debug = { "%{VULKAN_SDK}/Lib/vulkan-1.lib" },
            Release = { "%{VULKAN_SDK}/Lib/vulkan-1.lib" }
        }
    },
    Metal = {
        IncludeDir = "%{wks.location}/Hazard/vendor/Metal-CPP",
        MacosLibs = {
            Debug = {"Cocoa.framework", "Foundation.framework", "CoreFoundation.framework", "Metal.framework", "MetalKit.framework", "IOKit.framework", "QuartzCore.framework"}
        },
        IosLibs = {
            Debug = {"UIKit.framework", "Foundation.framework", "CoreFoundation.framework", "Metal.framework", "MetalKit.framework", "IOKit.framework", "QuartzCore.framework"}
        }
    },
    VMA = {
        IncludeDir = "%{wks.location}/Hazard-Renderer/vendor/VulkanMemoryAllocator"
    },
    ShaderC = {
        IncludeDir = "%{wks.location}/Hazard/vendor/shaderc/include",
        WindowsLibs = {
            Debug = { "%{VULKAN_SDK}/Lib/shaderc_sharedd.lib" },
            --Release = { "%{VULKAN_SDK}/Lib/shaderc_shared.lib" }
        }
    },
    SpirvCross = {
        IncludeDir = "%{VULKAN_SDK}/Include",
        LibraryDir = "%{wks.location}/Hazard/libmacos",
        WindowsLibs = {
            Debug = { "%{VULKAN_SDK}/Lib/spirv-cross-cored.lib", "%{VULKAN_SDK}/Lib/spirv-cross-glsld.lib", "%{VULKAN_SDK}/Lib/spirv-cross-msld.lib" },
            Release = { "%{VULKAN_SDK}/Lib/spirv-cross-core.lib", "%{VULKAN_SDK}/Lib/spirv-cross-glsl.lib", "%{VULKAN_SDK}/Lib/spirv-cross-msl.lib" },
        }
    },
    StbImage = {
        IncludeDir = "%{wks.location}/Hazard/vendor/stb_image"
    },
    GLAD = {
        IncludeDir = "%{wks.location}/Hazard/vendor/Glad/include",
        CommonLib = "Glad"
    },
    GLFW = {
        IncludeDir = "%{wks.location}/Hazard/vendor/GLFW/include",
        CommonLib = "GLFW"
    },
    Coral = {
        IncludeDir = "%{wks.location}/Hazard/vendor/Coral/Coral.Native/Include",
        CommonLib = "Coral.Native"
    },
    HazardUtility = {
        IncludeDir = "%{wks.location}/Hazard-Utility/src",
        CommonLib = "Hazard-Utility"
    },
    HazardScript = {
        IncludeDir = "%{wks.location}/Hazard-Script/src",
        CommonLib = "Hazard-Script"
    },
    HazardRenderer = {
        IncludeDir = "%{wks.location}/Hazard-Renderer/src",
        CommonLib = "Hazard-Renderer"
    },
    Hazard = {
        IncludeDir = "%{wks.location}/Hazard/src",
        CommonLib = "Hazard"
    }
}

local function LinkLibs(libs)
    for key, value in pairs(libs) do
        if key ~= "Common" then
            --filter (string.format("configurations:%s", key))
        end
        for k, lib in pairs(value) do
            links { lib }
        end
        if key ~= "Common" then
            --print("")
        end
    end
end

function References(reference)
    local ref = Dependencies[reference]
    
    if(ref.IncludeDir ~= nil) then
        includedirs { ref.IncludeDir }
    end
    
    if(ref.LibraryDir ~= nil) then
        libdirs { ref.LibraryDir }
    end

    if(ref.CommonLib ~= nil) then
        links { ref.CommonLib }
    end
    
    if(ref.WindowsLibs ~= nil and os.target() == "windows") then
        LinkLibs(ref.WindowsLibs)
    end
    if(ref.MacosLibs ~= nil and os.target() == "macosx") then
        LinkLibs(ref.MacosLibs)
    end
    if(ref.IosLibs ~= nil and os.target() == "ios") then
        LinkLibs(ref.IosLibs)
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

