#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Shader.h"
#include "Backend/Core/Pipeline/Buffers.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    class MetalShader : public Shader
    {
    public:
      
        MetalShader(const std::vector<ShaderStageCode>& shaderCode);
        ~MetalShader();
        
        void Reload() override;
        void Set(const std::string& name, uint32_t index, Ref<Image2D> image) override;
        void Set(const std::string& name, uint32_t index, Ref<CubemapTexture> cubemap) override;
        void Set(const std::string& name, uint32_t index, Ref<AccelerationStructure> accelerationStructure) override;
        void Set(const std::string& name, uint32_t index, Ref<BufferBase> buffer) override;

        const ShaderData& GetShaderData() override { return m_ShaderData; };
        std::unordered_map<ShaderStage, Buffer> GetShaderCode() const override { return m_ShaderCode; };
        
        //Metal specific
        MTL::Function* GetFunction(const ShaderStage& stage) { return m_Functions[stage]; }
        
    private:
        ShaderData m_ShaderData;
        std::unordered_map<ShaderStage, Buffer> m_ShaderCode;
        std::unordered_map<ShaderStage, MTL::Function*> m_Functions;
    };
}
#endif
