#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Shader.h"
#include "Backend/Core/Pipeline/Buffers.h"

namespace HazardRenderer::Metal
{
    class MetalShader : public Shader
    {
    public:
        MetalShader(const std::string& filePath);
        MetalShader(const std::string& name, const std::string& vertexPath, const std::string& fragmenPath);
        ~MetalShader();
        
    };
}
#endif
