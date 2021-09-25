#pragma once

#include "Hazard.h"
#include "../EditorGuiPlatform.h"

#if 0 

class EditorPlatformVulkan : public EditorGuiPlatform {
public:

	EditorPlatformVulkan(GLFWwindow* window, Rendering::Vulkan::VKContext* context);
	~EditorPlatformVulkan();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;

	void AddTexture(Ref<Rendering::Texture> texture);

private:
	Rendering::Vulkan::VKContext* m_Context = nullptr;
};
#endif