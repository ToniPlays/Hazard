#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Core/Rendering/Image2D.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalImage2D : public Image2D
    {
    public:
        MetalImage2D(Image2DCreateInfo* info);
        ~MetalImage2D();
        
        void Invalidate() override;
        void Release() override;
        void Release_RT();
        
        void Resize_RT(uint32_t width, uint32_t height);
        
        ImageType GetType() const override { return ImageType::Image2D; }
        const Extent& GetExtent() const override { return m_Extent; };
        ImageFormat GetFormat() const override { return m_Format; }
        const std::string& GetDebugName() const override { return m_DebugName; };
        virtual uint32_t GetMipLevels() const override { return m_MipLevels; };
        
        float GetAspectRatio() const override { return (float)m_Extent.Width / (float)m_Extent.Height; };
        
        //Metal specific
        MTL::Texture* GetMetalTexture() const { return m_MetalTexture; }
        
        uint32_t GetLayerCount() { return 1; }
        
    private:
        void Invalidate_RT();
        void GenerateMips();

    private:
        std::string m_DebugName;
        Extent m_Extent;
        uint32_t m_MipLevels;
        ImageFormat m_Format;
        ImageUsage m_Usage;
        Buffer m_LocalBuffer;
        
        MTL::Texture* m_MetalTexture = nullptr;
    };
}
#endif
