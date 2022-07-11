
#include "MetalPipeline.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalRenderCommandBuffer.h"
#include "MetalContext.h"
#include "File.h"

namespace HazardRenderer::Metal
{
    static MTL::PrimitiveType DrawTypeToMTLPrimitive(const DrawType& type) {
        
        switch(type) {
            case DrawType::Fill: return MTL::PrimitiveTypeTriangle;
            case DrawType::Line: return MTL::PrimitiveTypeLine;
            case DrawType::Point: return MTL::PrimitiveTypePoint;
        }
        return MTL::PrimitiveTypeTriangle;
    }

    MetalPipeline::MetalPipeline(PipelineSpecification* specs)
    {
        m_Specs = *specs;
        std::string file = File::ReadFile("res/" + specs->ShaderPath);
       
        NS::Error* error = nullptr;
        
        NS::String* source = NS::String::alloc()->string(file.c_str(), NS::UTF8StringEncoding);
        
        MTL::Device* device = MetalContext::GetMetalDevice();
        MTL::CompileOptions* options = MTL::CompileOptions::alloc()->init();
        MTL::LibraryType type = MTL::LibraryType::LibraryTypeExecutable;
        options->setLibraryType(type);
        
        MTL::Library* lib = device->newLibrary(source, options, &error);
        if(error) {
            std::cout << error->localizedDescription()->utf8String() << std::endl;
        }
        HZR_ASSERT(!error, "No library here");

        
        m_Shader = Shader::Create(specs->ShaderPath);

        NS::String* vertFunc = NS::String::alloc()->string("Vertex", NS::UTF8StringEncoding);
        NS::String* fragFunc = NS::String::alloc()->string("Fragment", NS::UTF8StringEncoding);
        
        MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
        
        descriptor->setVertexFunction(lib->newFunction(vertFunc));
        descriptor->setFragmentFunction(lib->newFunction(fragFunc));
        descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        
        NS::Error* err;
        m_Pipeline = device->newRenderPipelineState(descriptor, &err);
        descriptor->release();
    }
    MetalPipeline::~MetalPipeline()
    {
        m_Pipeline->release();
    }
    void MetalPipeline::Invalidate()
    {
        
    }
    void MetalPipeline::Bind(Ref<RenderCommandBuffer> commandBuffer)
    {
        MTL::RenderCommandEncoder* encoder = commandBuffer.As<MetalRenderCommandBuffer>()->GetEncoder();
        encoder->setRenderPipelineState(m_Pipeline);
    }
    void MetalPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        auto cmdBuffer = commandBuffer.As<MetalRenderCommandBuffer>();
        MTL::RenderCommandEncoder* encoder = cmdBuffer->GetEncoder();
        encoder->drawIndexedPrimitives(DrawTypeToMTLPrimitive(m_Specs.DrawType), count, MTL::IndexTypeUInt32, cmdBuffer->GetBoundIndexBuffer(), 0, 1);
    }
    void MetalPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        MTL::RenderCommandEncoder* encoder = commandBuffer.As<MetalRenderCommandBuffer>()->GetEncoder();
        //encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, 0, count, 1);
    }
    void MetalPipeline::DrawInstanced(Ref<RenderCommandBuffer> commandBuffer, uint32_t count, uint32_t instanceCount) {
        
        auto cmdBuffer = commandBuffer.As<MetalRenderCommandBuffer>();
        MTL::RenderCommandEncoder* encoder = cmdBuffer->GetEncoder();
        encoder->drawIndexedPrimitives(DrawTypeToMTLPrimitive(m_Specs.DrawType), count, MTL::IndexTypeUInt32, cmdBuffer->GetBoundIndexBuffer(), 0, instanceCount);
    }
}
#endif
