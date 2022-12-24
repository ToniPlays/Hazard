#pragma once

#include <hzrpch.h>
#include "../EditorGuiPlatform.h"
#include "HazardRendererCore.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Metal/MetalContext.h"
#include <GLFW/glfw3.h>
//#include <Metal/Metal.hpp>

class EditorPlatformMetal : public EditorGuiPlatform {
public:

    EditorPlatformMetal(HazardRenderer::Window& window);
    ~EditorPlatformMetal();

    void BeginFrame() override;
    void EndFrame() override;
    void Close() override;
private:
    HazardRenderer::Window* m_Window;
    HazardRenderer::Metal::MetalContext* m_Context;
    //MTL::RenderPassDescriptor* m_Descriptor;
};
#endif
