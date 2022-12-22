
#include "MetalPipeline.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalRenderCommandBuffer.h"
#include "MetalContext.h"
#include "File.h"

namespace HazardRenderer::Metal
{
    static MTL::PrimitiveType DrawTypeToMTLPrimitive(const DrawType& type) {
        
        switch(type)
        {
            case DrawType::Fill: return MTL::PrimitiveTypeTriangle;
            case DrawType::Line: return MTL::PrimitiveTypeLine;
            case DrawType::Point: return MTL::PrimitiveTypePoint;
            default: return MTL::PrimitiveTypePoint;
        }
        return MTL::PrimitiveTypeTriangle;
    }

    MetalPipeline::MetalPipeline(PipelineSpecification* specs)
    {
        
    }
    MetalPipeline::~MetalPipeline()
    {
        
    }
}
#endif
