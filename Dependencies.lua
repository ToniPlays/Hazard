
-- Hazard Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

function file_exists(name)
   print(name)
   local f=io.open(name,"r")
   if f~=nil then io.close(f) return true else return false end
end


IncludeDir = {}
IncludeDir["yaml_cpp"] = "%{wks.location}/Hazard/vendor/yaml-cpp/include"
IncludeDir["glm"] = "%{wks.location}/Hazard/vendor/glm"

IncludeDir["Mono"] = "%{wks.location}/Hazard/vendor/mono/include"

IncludeDir["Vorbis"] = "%{wks.location}/Hazard/vendor/Vorbis/include"
IncludeDir["AudioFile"] = "%{wks.location}/Hazard/vendor/AudioFile"
IncludeDir["Libogg"] = "%{wks.location}/Hazard/vendor/libogg/include"
IncludeDir["Minimp3"] = "%{wks.location}/Hazard/vendor/minimp3"

IncludeDir["Box2D"] = "%{wks.location}/Hazard/vendor/Box2D/include"
IncludeDir["Entt"] = "%{wks.location}/Hazard/vendor/entt/include"

IncludeDir["PortableFileDialogs"] = "%{wks.location}/Hazard/vendor/portable-file-dialogs"
IncludeDir["Assimp"] = "%{wks.location}/Hazard/vendor/assimp/include"
IncludeDir["VMA"] = "%{wks.location}/Hazard-Renderer/vendor/VulkanMemoryAllocator"
IncludeDir["ImGui"] = "%{wks.location}/Hazard/vendor/ImGui"

IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"
IncludeDir["Metal"] = "%{wks.location}/Hazard/vendor/Metal-CPP"
IncludeDir["Glad"] = "%{wks.location}/Hazard/vendor/Glad/include"
IncludeDir["GLFW"] = "%{wks.location}/Hazard/vendor/GLFW/include"

IncludeDir["SPIRV_Cross"] = "%{wks.location}/Hazard/vendor/SPIRV-Cross"
IncludeDir["shaderc"] = "%{wks.location}/Hazard/vendor/shaderc/include"
IncludeDir["stb_image"] = "%{wks.location}/Hazard/vendor/stb_image"

IncludeDir["Hazard_Utility"] = "%{wks.location}/Hazard-Utility/src"
IncludeDir["Hazard_Renderer"] = "%{wks.location}/Hazard-Renderer/src"
IncludeDir["Hazard_Script"] = "%{wks.location}/Hazard-Script/src"
IncludeDir["Hazard_UI"] = "%{wks.location}/Hazard-UI/src"

IncludeDir["Coral"] = "%{wks.location}/Hazard/vendor/Coral/Coral.Native/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{VULKAN_SDK}/Bin"
LibraryDir["CoralNetHost"] = "%{wks.location}/Hazard/vendor/Coral/NetCore/7.0.7/nethost.lib"

Library = {}

  if os.host() == "windows" then

    IncludeDir["SPIRV_Cross"] = "%{VulkanSDK}/Include"

    Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"

    Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
    Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
    Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
    Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-Toolsd.lib"

    Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
    Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
    Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

    Library["Assimp_Lib"] = "%{wks.location}/Hazard/vendor/assimp/lib/assimp-vc142-mt.lib"

end
if os.host() == "macosx" then

    CELLAR = os.getenv("HOMEBREW_CELLAR") .. "/"

    IncludeDir["SPIRV_Cross"] = CELLAR .. "spirv-cross/2021-01-15/include"

    LibraryDir["Assimp_Lib"] = CELLAR .. "assimp/5.2.5/lib/"
    LibraryDir["SPIRV_Cross"] = CELLAR .. "spirv-cross/2021-01-15/lib/"
    LibraryDir["ShaderC"] = CELLAR .. "shaderc/2022.4/lib/"

    Library["Vulkan"] = "vulkan"
    Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK/VkLayer_utils.lib"

    Library["ShaderC_Debug"] = "shaderc/2022.4/lib/libshaderc.a"
    Library["SPIRV_Cross_Debug"] = CELLAR .. "spirv-cross/2021-01-15/lib/spirv-cross-core"
    Library["SPIRV_Cross_GLSL_Debug"] = CELLAR .. "/spirv-cross/2021-01-15/lib/spirv-cross-glsl"
    Library["SPIRV_Tools_Debug"] = CELLAR .. "spirv-cross/2021-01-15/lib/SPIRV-Tools"

    Library["ShaderC_Release"] = CELLAR .. "shaderc"
    Library["SPIRV_Cross_Release"] = CELLAR .. "/spirv-cross/2021-01-15/lib/spirv-cross-core"
    Library["SPIRV_Cross_GLSL_Release"] = CELLAR .. "spirv-cross/2021-01-15/lib/spirv-cross-glsl"
    Library["SPIRV_Tools_Release"] = "SPIRV-Tools"

    Library["Assimp_Lib"] =  CELLAR .. "assimp/5.2.5/lib/libassimp.5.2.4.dylib"
    
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

