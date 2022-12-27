#pragma once

#include "Backend/Core/Texture/Texture.h"

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
        uint32_t GetMipLevels() const override { return 1; }
        const std::string& GetPath() const override { return m_FilePath; };
        
        Ref<Image2D> GetSourceImage() override { return m_SourceImage; }
        
        //Metal specific
        MTL::Texture* GetMetalTexture() const { return m_MetalTexture; }
        MTL::SamplerState* GetMetalSamplerState() const { return m_MetalSampler; }
        
    private:
        void CreateSampler();
        void GenerateFromCubemap(CubemapGen& generationData) {};
        
    private:
        ImageFormat m_Format = ImageFormat::None;
        uint32_t m_Width = 0;
        uint32_t m_Height = 0;
        
        std::string m_FilePath = "";
        std::string m_DebugName = "";
        
        Ref<Image2D> m_SourceImage = nullptr;
        
        MTL::Texture* m_MetalTexture;
        MTL::SamplerState* m_MetalSampler;
    };
}
