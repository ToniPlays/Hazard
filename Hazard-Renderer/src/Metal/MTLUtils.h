#pragma once

#include "Core/Core.h"

#ifdef HZR_INCLUDE_METAL

#include "Core/Rendering/Pipeline.h"
#include "Core/Rendering/Shader.h"

#include "MetalContext.h"
#include "MetalPhysicalDevice.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    static MTL::PrimitiveType DrawTypeToMTLPrimitive(uint32_t flags)
    {
        if(flags & PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
            return MTL::PrimitiveTypeTriangle;
        if(flags & PIPELINE_PRIMITIVE_TOPOLOGY_LINE_LIST)
            return MTL::PrimitiveTypeLine;
        if(flags & PIPELINE_PRIMITIVE_TOPOLOGY_POINT_LIST)
            return MTL::PrimitiveTypePoint;
        
        return MTL::PrimitiveTypeTriangle;
    }
    static MTL::PrimitiveTopologyClass DrawTypeToMTLTopology(uint32_t flags)
    {
        if(flags & PIPELINE_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
            return MTL::PrimitiveTopologyClassTriangle;
        if(flags & PIPELINE_PRIMITIVE_TOPOLOGY_LINE_LIST)
            return MTL::PrimitiveTopologyClassLine;
        if(flags & PIPELINE_PRIMITIVE_TOPOLOGY_POINT_LIST)
            return MTL::PrimitiveTopologyClassPoint;
        
        return MTL::PrimitiveTopologyClassUnspecified;
    }
    static MTL::VertexFormat ShaderDataTypeToMTLVertexFormat(const ShaderDataType& type)
    {
        switch (type) {
            case ShaderDataType::Bool:          return MTL::VertexFormatInt;
            case ShaderDataType::Float:         return MTL::VertexFormatFloat;
            case ShaderDataType::Float2:        return MTL::VertexFormatFloat2;
            case ShaderDataType::Float3:        return MTL::VertexFormatFloat3;
            case ShaderDataType::Float4:        return MTL::VertexFormatFloat4;
            case ShaderDataType::Int:           return MTL::VertexFormatInt;
            case ShaderDataType::Int2:          return MTL::VertexFormatInt2;
            case ShaderDataType::Int3:          return MTL::VertexFormatInt3;
            case ShaderDataType::Int4:          return MTL::VertexFormatInt4;
            default:                            return MTL::VertexFormatInvalid;
        }
    }
    static MTL::PixelFormat ImageFormatToMTLFormat(const ImageFormat& format)
    {
        switch (format)
        {
            case ImageFormat::RED32F:           return MTL::PixelFormatR32Float;
            case ImageFormat::RGB:              return MTL::PixelFormatETC2_RGB8;
            case ImageFormat::RGB16F:           return MTL::PixelFormatETC2_RGB8;
            case ImageFormat::RGB32F:           return MTL::PixelFormatETC2_RGB8;
            case ImageFormat::RGBA:             return MTL::PixelFormatRGBA8Unorm;
            case ImageFormat::RGBA16F:          return MTL::PixelFormatRGBA16Float;
            case ImageFormat::RGBA32F:          return MTL::PixelFormatRGBA32Float;
            case ImageFormat::RG16F:            return MTL::PixelFormatRG16Float;
            case ImageFormat::RG32F:            return MTL::PixelFormatRG32Float;
            case ImageFormat::SRGB:             return MTL::PixelFormatR8Unorm_sRGB;
            case ImageFormat::DEPTH32F:         return MTL::PixelFormatR32Float;
            case ImageFormat::DEPTH24STENCIL8:
            {
                auto device = MetalContext::GetMetalDevice();
                if(device->Depth24Stencil8Supported())
                    return MTL::PixelFormatDepth24Unorm_Stencil8;
                
                return MTL::PixelFormatDepth32Float;
            }
            case ImageFormat::RED32I:           return MTL::PixelFormatR32Uint;
            default: break;
        }
        HZR_ASSERT(false, "");
        return MTL::PixelFormatInvalid;
    }
    
    static bool FormatBlendable(const ImageFormat& format)
    {
        switch (format)
        {
            case ImageFormat::RED32I: return false;
            default: return true;
        }
    }
    
    static MTL::CompareFunction DepthOpToMTLDepthOp(const DepthOp& op)
    {
        switch (op)
        {
        case DepthOp::None:             return MTL::CompareFunctionNever;
        case DepthOp::Never:            return MTL::CompareFunctionNever;
        case DepthOp::NotEqual:         return MTL::CompareFunctionNotEqual;
        case DepthOp::Less:             return MTL::CompareFunctionLess;
        case DepthOp::LessOrEqual:      return MTL::CompareFunctionLessEqual;
        case DepthOp::Equal:            return MTL::CompareFunctionEqual;
        case DepthOp::Greater:          return MTL::CompareFunctionGreater;
        case DepthOp::GreaterOrEqual:   return MTL::CompareFunctionGreaterEqual;
        case DepthOp::Always:           return MTL::CompareFunctionAlways;
        }
        return MTL::CompareFunctionNever;
    }
    static MTL::CullMode CullModeToMTLCullMode(uint32_t flags)
    {
        if(flags & PIPELINE_CULL_FRONT_FACE)
            return MTL::CullModeFront;
        if(flags & PIPELINE_CULL_BACK_FACE)
            return MTL::CullModeBack;
        return MTL::CullModeNone;
    }
    static MTL::SamplerMinMagFilter FilterModeToMTLFilter(FilterMode mode)
    {
        switch(mode)
        {
            case FilterMode::Nearest: return MTL::SamplerMinMagFilterNearest;
            case FilterMode::Linear: return MTL::SamplerMinMagFilterLinear;
            case FilterMode::LinearMip:
                break;
            case FilterMode::NearestMip:
                break;
        }
        return MTL::SamplerMinMagFilterLinear;
    }
    static MTL::SamplerAddressMode ImageWrapToSamplerAddressMode(ImageWrap mode)
    {
        switch(mode)
        {
            case ImageWrap::Repeat: return MTL::SamplerAddressModeRepeat;
            case ImageWrap::RepeatMirror: return MTL::SamplerAddressModeMirrorRepeat;
            case ImageWrap::ClampBorder:
                return MTL::SamplerAddressModeClampToBorderColor;
            case ImageWrap::ClampEdge:
                return MTL::SamplerAddressModeClampToEdge;
        }
        return MTL::SamplerAddressModeRepeat;
    }

    template<typename T>
    static void SetDebugLabel(T* object, const std::string& label)
    {
        NS::String* l = NS::String::alloc()->init(label.c_str(), NS::UTF8StringEncoding);
        object->setLabel(l);
        l->release();
    }
}
#endif
