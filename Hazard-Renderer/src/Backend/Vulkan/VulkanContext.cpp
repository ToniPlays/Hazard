
#include "VulkanContext.h"
#ifdef HZR_INCLUDE_VULKAN
#include "Backend/Core/Renderer.h"
#include "Backend/Core/Window.h"
#include "Core/VulkanAllocator.h"
#include "VkUtils.h"

#include "VulkanRenderCommandBuffer.h"
#include "spdlog/fmt/fmt.h"

#define VK_KHR_WIN32_SURFACE_EXTENSION_NAME "VK_KHR_win32_surface"

namespace HazardRenderer::Vulkan 
{

	static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, const VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
	{
		const bool performanceWarn = false;
		if (!performanceWarn) {
			if (messageType & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) return VK_FALSE;
		}

		std::string labels, objects;
		if (pCallbackData->cmdBufLabelCount) 
		{
			labels = fmt::format("\tLables({}): \n", pCallbackData->cmdBufLabelCount);
			for (uint32_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) 
			{
				const auto& label = pCallbackData->pCmdBufLabels[i];
				const std::string colorStr = fmt::format("[ {}, {}, {}, {}]", label.color[0], label.color[1], label.color[2], label.color[3]);

				labels.append(fmt::format("\t\t- Command Buffer Label[{0}]: name {1}, color {2}\n", i, label.pLabelName ? label.pLabelName : "NULL", colorStr));
			}
		}
		if (pCallbackData->objectCount) {
			objects = fmt::format("\tObjects({}): \n", pCallbackData->objectCount);
			for (uint32_t i = 0; i < pCallbackData->objectCount; i++)
			{
				const auto& obj = pCallbackData->pObjects[i];
				labels.append(fmt::format("\t\t- Object[{0}]: name {1}, type {2}, handle: {3:#x}\n", i, obj.pObjectName ? obj.pObjectName : "NULL", VkUtils::VkObjectTypeToString(obj.objectType), obj.objectHandle));
			}
		}

		RenderMessage message = {};
		message.Severity = Severity::Debug;
		message.Description = fmt::format("{0} {1}\n\t{2}\n {3} {4}", VkUtils::VkDebugUtilsMessageType(messageType), VkUtils::VkDebugUtilsMessageSeverity(messageSeverity), pCallbackData->pMessage, labels, objects);

		Window::SendDebugMessage(message);

		return VK_FALSE;
	}


	static bool CheckDriverAPIVersion(uint32_t minimumAPIVersion) {
		uint32_t instanceVersion;
		vkEnumerateInstanceVersion(&instanceVersion);

		return instanceVersion >= minimumAPIVersion;
	}


	VulkanContext::VulkanContext(WindowProps* props)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		if (!glfwVulkanSupported()) {
			HZR_ASSERT(false, "Vulkan not supported");
		}
		Renderer::Init();
		s_Instance = this;
	}

	VulkanContext::~VulkanContext()
	{
		m_VulkanDevice = nullptr;
		vkDestroyInstance(m_VulkanInstance, nullptr);
	}
	void VulkanContext::Init(Window* window, HazardRendererCreateInfo* info)
	{
		if (!CheckDriverAPIVersion(VK_API_VERSION_1_2)) {
			HZR_ASSERT(false, "API version not supported");
		}

		std::vector<const char*> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
		instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		if (info->Logging)
		{
			instanceExtensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			instanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
		}

		VkValidationFeatureEnableEXT enables[] = { VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT };
		VkValidationFeaturesEXT features = {};
		features.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
		features.enabledValidationFeatureCount = 1;
		features.pEnabledValidationFeatures = enables;


		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = info->pAppInfo->AppName.c_str();
		appInfo.pEngineName = "Hazard";
		appInfo.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo instanceInfo = {};
		instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceInfo.pApplicationInfo = &appInfo;
		instanceInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
		instanceInfo.ppEnabledExtensionNames = instanceExtensions.data();

		const char* validationName = "VK_LAYER_KHRONOS_validation";

		if (info->Logging)
		{
			uint32_t instanceLayerCount;
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
			std::vector<VkLayerProperties> instanceProperties(instanceLayerCount);
			vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceProperties.data());
			bool validationPresent = false;

			for (const VkLayerProperties& prop : instanceProperties)
			{
				if (strcmp(prop.layerName, validationName) == 0) {
					validationPresent = true;
					break;
				}
			}
			if (validationPresent) {
				instanceInfo.ppEnabledLayerNames = &validationName;
				instanceInfo.enabledLayerCount = 1;
			}
		}
		auto result = vkCreateInstance(&instanceInfo, nullptr, &m_VulkanInstance);
		VK_CHECK_RESULT(result, "Failed to create VkInstance");
		//Instance and surface
		
		if (info->Logging) 
		{
			auto vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(m_VulkanInstance, "vkCreateDebugUtilsMessengerEXT");

			VkDebugUtilsMessengerCreateInfoEXT debugUtilsInfo = {};
			debugUtilsInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugUtilsInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT;
			debugUtilsInfo.messageType |= VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
			debugUtilsInfo.messageType |= VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

			debugUtilsInfo.pfnUserCallback = VulkanDebugCallback;
			debugUtilsInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
			debugUtilsInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

			VK_CHECK_RESULT(vkCreateDebugUtilsMessengerEXT(m_VulkanInstance, &debugUtilsInfo, nullptr, &m_DebugMessenger), "Failed to create VkDebutUtilsMessengerEXT");
		}

		m_VulkanDevice = VulkanPhysicalDevice::Create(-1); // Auto select: -1, others indexed
	}
}
#endif
