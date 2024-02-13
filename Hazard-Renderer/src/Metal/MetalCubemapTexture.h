#pragma once

#include "Core/Rendering/Cubemap.h"

#ifdef HZR_INCLUDE_METAL

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalCubemapTexture : public CubemapTexture
    {
    public:
        MetalCubemapTexture() = default;
        MetalCubemapTexture(CubemapCreateInfo* createInfo);
        
        ImageFormat GetFormat() const override { return m_Format; };
        uint32_t GetWidth() const override { return m_Width; };
        uint32_t GetHeight() const override { return m_Height; }
        glm::uvec2 GetSize() const override { return { m_Width, m_Height }; };
        uint32_t GetMipLevels() const override { return m_MipLevels; }
        void RegenerateMips() override;
        
        //Metal specific
        MTL::Texture* GetMetalTexture() const { return m_MetalTexture; }
        const std::string& GetDebugName() const { return m_DebugName; }
        
    private:
        ImageFormat m_Format = ImageFormat::None;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_MipLevels = 1;
        ImageUsage m_Usage;
        
        Buffer m_LocalBuffer;
        
        std::string m_DebugName = "";
        MTL::Texture* m_MetalTexture = nullptr;
    };
}
#endif
