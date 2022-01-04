
#include <hzrpch.h>
#include "MetalPipeline.h"

namespace Hazard::Rendering::Metal {
    MetalPipeline::MetalPipeline(PipelineSpecification* specs) {
        m_Shader = Shader::Create(specs->ShaderPath);

    }
}
