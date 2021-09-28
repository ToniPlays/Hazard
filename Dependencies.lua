
-- Hazard Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/Hazard/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Hazard/vendor/yaml-cpp/include"
IncludeDir["Glad"] = "%{wks.location}/Hazard/vendor/Glad/include"
IncludeDir["GLFW"] = "%{wks.location}/Hazard/vendor/GLFW/include"
IncludeDir["glm"] = "%{wks.location}/Hazard/vendor/glm"
IncludeDir["Mono"] = "%{wks.location}/Hazard/vendor/mono/include"
IncludeDir["OpenAL"] = "%{wks.location}/Hazard/vendor/OpenAL/include"
IncludeDir["Vorbis"] = "%{wks.location}/Hazard/vendor/Vorbis/include"
IncludeDir["AudioFile"] = "%{wks.location}/Hazard/vendor/AudioFile"
IncludeDir["Libogg"] = "%{wks.location}/Hazard/vendor/libogg/include"
IncludeDir["Box2D"] = "%{wks.location}/Hazard/vendor/Box2D/include"
IncludeDir["Minimp3"] = "%{wks.location}/Hazard/vendor/minimp3"
IncludeDir["Entt"] = "%{wks.location}/Hazard/vendor/entt/include"
IncludeDir["Assimp"] = "%{wks.location}/Hazard/vendor/assimp/include"
IncludeDir["shaderc"] = "%{wks.location}/Hazard/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/Hazard/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/Hazard/vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{wks.location}/Hazard/vendor/VulkanSDK/Bin"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"

Library["OpenAL_Lib"] = "%{wks.location}/Hazard/vendor/OpenAl/lib/openal32.lib"
Library["Mono_Debug_Lib"] = "%{wks.location}/Hazard/vendor/mono/lib/Debug/mono-2.0-sgen.lib"
Library["Assimp_Lib"] = "%{wks.location}/Hazard/vendor/assimp/lib/assimp-vc142-mt.lib"
