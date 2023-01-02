#pragma once

#include "Backend/Core/Texture/Texture.h"

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
        MetalCubemapTexture(CubemapTextureCreateInfo* createInfo);
        
        ImageFormat GetFormat() const override { return m_Format; };
        uint32_t GetWidth() const override { return m_Width; };
        uint32_t GetHeight() const override { return m_Height; }
        glm::uvec2 GetSize() const override { return { m_Width, m_Height }; };
        uint32_t GetMipLevels() const override { return m_MipLevels; }
        const std::string& GetPath() const override { return m_FilePath; };
        
        Ref<Image2D> GetSourceImage() override { return m_SourceImage; }
        
        //Metal specific
        MTL::Texture* GetMetalTexture() const { return m_MetalTexture; }
        MTL::SamplerState* GetMetalSamplerState() const { return m_MetalSampler; }
        
        void SetImageData(const Buffer& data);
        void UploadImageData_RT();
        
        void GenerateMipmaps_RT(MTL::CommandBuffer* commandBuffer);
        
    private:
        void CreateSampler();
        void GenerateFromCubemap(CubemapGen& generationData) {};
        
    private:
        ImageFormat m_Format = ImageFormat::None;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        uint32_t m_MipLevels = 1;
        ImageUsage m_Usage;
        
        Buffer m_LocalBuffer;
        
        std::string m_FilePath = "";
        std::string m_DebugName = "";
        
        Ref<Image2D> m_SourceImage = nullptr;
        
        MTL::Texture* m_MetalTexture = nullptr;
        MTL::SamplerState* m_MetalSampler = nullptr;
    };
}
#endif