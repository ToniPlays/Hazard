#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Backend/Core/Pipeline/Pipeline.h"
#include "Backend/Core/Pipeline/Shader.h"

#include <Metal/Metal.hpp>

namespace HazardRenderer::Metal
{
    class MetalPipeline : public Pipeline
    {
    public:
        MetalPipeline(PipelineSpecification* specs);
        virtual ~MetalPipeline();
    };
}
#endif
