#pragma once

#include "Core/Rendering/Cubemap.h"

#ifdef HZR_INCLUDE_METAL

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalCubemap : public Cubemap
    {
    public:
        MetalCubemap() = default;
        MetalCubemap(CubemapCreateInfo* createInfo);
        ~MetalCubemap();
        
        const std::string& GetDebugName() const override { return m_DebugName; }
        ImageFormat GetFormat() const override { return m_Format; };
        const Extent& GetExtent() const override { return m_Extent; };
        uint32_t GetMipLevels() const override { return m_MipLevels; }
        float GetAspectRatio() const override { return 1.0f; };
        void RegenerateMips() override;
        
        void Invalidate() override;
        void Release() override;
        
        //Metal specific
        MTL::Texture* GetMetalTexture() const { return m_MetalTexture; }
        
    private:
        ImageFormat m_Format = ImageFormat::None;
        Extent m_Extent;
        uint32_t m_MipLevels = 1;
        ImageUsage m_Usage;
        
        Buffer m_LocalBuffer;
        
        std::string m_DebugName = "";
        MTL::Texture* m_MetalTexture = nullptr;
    };
}
#endif
