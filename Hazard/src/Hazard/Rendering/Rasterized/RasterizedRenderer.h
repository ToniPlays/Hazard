#pragma once

#include "Backend/Core/RenderCommandBuffer.h"
#include "Hazard/Rendering/RendererDrawList.h"

#include "HazardRendererCore.h"

namespace Hazard
{
    struct DrawCall
    {
        size_t InstanceCount;
        size_t IndexCount;
        size_t TransformOffset;
        
        Ref<Material> Material;
        Ref<HazardRenderer::VertexBuffer> VertexBuffer;
        Ref<HazardRenderer::IndexBuffer> IndexBuffer;
    };

	class RasterizedRenderer
	{
	public:
        void Render(Ref<HazardRenderer::RenderCommandBuffer> buffer, const RendererDrawList& drawList);
    private:
        
        void PrepareCamera(const WorldCameraData& camera);
        
        void PreRender(const RendererDrawList& drawList);
        void ShadowMapPass();
        void PreDepthPass() {};
        void LightCullingPass() {};
        void GeometryPass(const RendererDrawList& drawList);
        void BloomPass() {};
        void CompositePass(const EnvironmentData& environment);
      
    private:
        Ref<HazardRenderer::RenderCommandBuffer> m_Buffer;
        
        std::vector<DrawCall> m_DrawCalls;
	};
}
