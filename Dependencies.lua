
-- Hazard Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["yaml_cpp"] = "%{wks.location}/Hazard/vendor/yaml-cpp/include"
IncludeDir["glm"] = "%{wks.location}/Hazard/vendor/glm"

IncludeDir["Mono"] = "%{wks.location}/Hazard/vendor/mono/include"
IncludeDir["Optick"] = "%{wks.location}/Hazard/vendor/Optick/src"

IncludeDir["OpenAL"] = "%{wks.location}/Hazard/vendor/OpenAL/include"
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


LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Hazard/vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{wks.location}/Hazard/vendor/VulkanSDK/Bin"

Library = {}

  if os.host() == "windows" then

    IncludeDir["SPIRV_Cross"] = "%{VulkanSDK}/Include"

    Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
    Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

    Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
    Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
    Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
    Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-Toolsd.lib"

    Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
    Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
    Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

    Library["OpenAL_Lib"] = "%{wks.location}/Hazard/vendor/OpenAl/lib/openal32.lib"
    Library["Mono_Debug_Lib"] = "%{wks.location}/Hazard/vendor/mono/lib/Debug/mono-2.0-sgen.lib"
    Library["Assimp_Lib"] = "%{wks.location}/Hazard/vendor/assimp/lib/assimp-vc142-mt.lib"

end
if os.host() == "macosx" then

    IncludeDir["SPIRV_Cross"] = "/opt/homebrew/Cellar/spirv-cross/2021-01-15/include"

    --Library["Vulkan"] = "vulkan"
    -- Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK/VkLayer_utils.lib"

    --Library["ShaderC_Debug"] = "shaderc"
    Library["SPIRV_Cross_Debug"] = "/opt/homebrew/Cellar/spirv-cross/2021-01-15/lib/spirv-cross-core"
    Library["SPIRV_Cross_GLSL_Debug"] = "/opt/homebrew/Cellar//spirv-cross/2021-01-15/lib/spirv-cross-glsl"
    Library["SPIRV_Tools_Debug"] = "/opt/homebrew/Cellar/spirv-cross/2021-01-15/lib/SPIRV-Tools"

    --Library["ShaderC_Release"] = "shaderc"
    Library["SPIRV_Cross_Release"] = "/opt/homebrew/Cellar//spirv-cross/2021-01-15/lib/spirv-cross-core"
    Library["SPIRV_Cross_GLSL_Release"] = "/opt/homebrew/Cellar/spirv-cross/2021-01-15/lib/spirv-cross-glsl"
    Library["SPIRV_Tools_Release"] = "SPIRV-Tools"

    Library["Mono_Debug_Lib"] = "/usr/local/homebrew/Cellar/mono/6.12.0.122/lib/mono-2.0"
    Library["Assimp_Lib"] = "/opt/homebrew/Cellar/assimp/5.1.4/lib/assimp"
end
