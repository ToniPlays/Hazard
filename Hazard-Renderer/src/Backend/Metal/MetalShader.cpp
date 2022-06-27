
#include "MetalShader.h"
#ifdef HZR_INCLUDE_METAL

namespace HazardRenderer::Metal
{
    MetalShader::MetalShader(const std::string& path) {
        
    }
    MetalShader::MetalShader(const std::string& name, const std::string& vertexPath, const std::string& fragmenPath)
    {
        
    }
    MetalShader::~MetalShader() {
        
    }
    void MetalShader::Reload() {
        
    }
    void MetalShader::Bind(Ref<RenderCommandBuffer> cmdBufer) {
        
    }
    void MetalShader::Unbind() {
        
    }
    void MetalShader::SetUniformBuffer(const std::string &name, void *data, uint32_t size) {
        //m_UniformBuffers[name]->SetData(data);
    }
    void MetalShader::Set(const std::string& name, uint32_t index, uint32_t value) {
        
    }
    void MetalShader::Set(const std::string& name, uint32_t index, Ref<Texture2D>& value) {
        
    }
}
#endif
