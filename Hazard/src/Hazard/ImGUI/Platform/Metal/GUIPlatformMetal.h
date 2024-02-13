#pragma once

#include <hzrpch.h>
#include "HazardRendererCore.h"

#ifdef HZR_INCLUDE_METAL

#include "Metal/MetalContext.h"
#include <GLFW/glfw3.h>

#include <Metal/Metal.hpp>
#include <Foundation/Foundation.hpp>
#include <QuartzCore/QuartzCore.hpp>

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
    MTL::RenderPassDescriptor* m_Descriptor;
};
#endif
