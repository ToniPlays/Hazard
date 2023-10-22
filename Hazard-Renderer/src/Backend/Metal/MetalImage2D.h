#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Image2D.h"

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
        
        Buffer ReadPixels(const ImageCopyRegion& region) override;
        
        TextureType GetType() const override { return TextureType::Image2D; }
        uint32_t GetWidth() override { return m_Width; };
        uint32_t GetHeight() override { return m_Height; };
        ImageFormat GetFormat() const override { return m_Format; }
        const std::string& GetDebugName() const override { return m_DebugName; };
        virtual uint32_t GetMipLevels() const override { return m_MipLevels; };
        
        virtual float GetAspectRatio() override { return (float)m_Width / (float)m_Height; };
        virtual Buffer GetBuffer() const override { return m_LocalBuffer; };
        virtual const Buffer& GetBuffer() override { return m_LocalBuffer; };
        
        //Metal specific
        MTL::Texture* GetMetalTexture() const { return m_MetalTexture; }
        
        uint32_t GetLayerCount() { return 1; }
        
    private:
        void Invalidate_RT();
        void UploadImageData_RT();

    private:
        std::string m_DebugName;
        uint32_t m_Width;
        uint32_t m_Height;
        uint32_t m_MipLevels;
        ImageFormat m_Format;
        ImageUsage m_Usage;
        Buffer m_LocalBuffer;
        
        MTL::Texture* m_MetalTexture;
    };
}
#endif
