
#include "MetalPipeline.h"
#ifdef HZR_INCLUDE_METAL

#include "MetalRenderCommandBuffer.h"
#include "MetalContext.h"

//#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    MetalPipeline::MetalPipeline(PipelineSpecification* specs)
    {
        /*
        return;
        std::string shader = "#include <metal_stdlib>\n"
            "using namespace metal;\n"
            "\n"
            "vertex float4 basic_vertex(                           // 1\n"
            "  const device packed_float3* vertex_array [[ buffer(0) ]], // 2 \n"
            "  unsigned int vid [[ vertex_id ]]) {                 // 3\n"
            "  return float4(vertex_array[vid], 1.0);              // 4\n"
            "}\n"
            "\n"
            "fragment half4 basic_fragment() { // 1\n"
            "  return half4(1.0);              // 2\n"
            "}";
        
        NS::Error* error = nullptr;
        
        NS::String* source = NS::String::alloc()->string(shader.c_str(), NS::UTF8StringEncoding);
        
        MTL::Device* device = MetalContext::GetMetalDevice();
        MTL::CompileOptions* options = MTL::CompileOptions::alloc()->init();
        MTL::LibraryType type = MTL::LibraryType::LibraryTypeExecutable;
        options->setLibraryType(type);
        
        MTL::Library* lib = device->newLibrary(source, options, &error);
        ASSERT(!error, "No library here");

        
        m_Shader = Shader::Create(specs->ShaderPath);

        NS::String* vertFunc = NS::String::alloc()->string("basic_vertex", NS::UTF8StringEncoding);
        NS::String* fragFunc = NS::String::alloc()->string("basic_fragment", NS::UTF8StringEncoding);
        
        MTL::RenderPipelineDescriptor* descriptor = MTL::RenderPipelineDescriptor::alloc()->init();
        
        descriptor->setVertexFunction(lib->newFunction(vertFunc));
        descriptor->setFragmentFunction(lib->newFunction(fragFunc));
        descriptor->colorAttachments()->object(0)->setPixelFormat(MTL::PixelFormatBGRA8Unorm);
        
        NS::Error* err;
        m_Pipeline = device->newRenderPipelineState(descriptor, &err);
         */
    }
    MetalPipeline::~MetalPipeline()
    {
        
    }
    void MetalPipeline::Invalidate() {
        
    }
    void MetalPipeline::Bind(Ref<RenderCommandBuffer> commandBuffer)
    {
        //MTL::RenderCommandEncoder* encoder = commandBuffer.As<MetalRenderCommandBuffer>()->GetEncoder();
        //encoder->setRenderPipelineState(m_Pipeline);
    }
    void MetalPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        //MTL::RenderCommandEncoder* encoder = commandBuffer.As<MetalRenderCommandBuffer>()->GetEncoder();
        //encoder->drawPrimitives(MTL::PrimitiveTypeTriangle, (NS::UInteger)0, (NS::UInteger)count);
    }
    void MetalPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
    {
        
    }
}
#endif
