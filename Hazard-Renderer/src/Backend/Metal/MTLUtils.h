#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_METAL
#include "Backend/Core/Pipeline/Pipeline.h"
#include "Backend/Core/Pipeline/Shader.h"

#include "MetalContext.h"
#include "MetalPhysicalDevice.h"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

namespace HazardRenderer::Metal
{
    static MTL::PrimitiveType DrawTypeToMTLPrimitive(const DrawType& type) {
    
        switch(type)
        {
            case DrawType::Fill:    return MTL::PrimitiveTypeTriangle;
            case DrawType::Line:    return MTL::PrimitiveTypeLine;
            case DrawType::Point:   return MTL::PrimitiveTypePoint;
            default:                return MTL::PrimitiveTypePoint;
        }
    }
    static MTL::PrimitiveTopologyClass DrawTypeToMTLTopology(const DrawType& type)
    {
        switch (type) {
            case DrawType::None:    return MTL::PrimitiveTopologyClassUnspecified;
            case DrawType::Fill:    return MTL::PrimitiveTopologyClassTriangle;
            case DrawType::Line:    return MTL::PrimitiveTopologyClassLine;
            case DrawType::Point:   return MTL::PrimitiveTopologyClassPoint;
        }
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
            case ImageFormat::RED32I:           return MTL::PixelFormatR32Sint;
        }
        HZR_ASSERT(false, "");
        return MTL::PixelFormatInvalid;
    }
    
    bool FormatBlendable(const ImageFormat& format)
    {
        switch (format)
        {
            case ImageFormat::RED32I: return false;
            default: return true;
        }
    }
    
    MTL::CompareFunction DepthOpToMTLDepthOp(const DepthOp& op)
    {
        switch (op)
        {
        case DepthOp::None:                 return MTL::CompareFunctionNever;
        case DepthOp::Never:                return MTL::CompareFunctionNever;
        case DepthOp::NotEqual:             return MTL::CompareFunctionNotEqual;
        case DepthOp::Less:                 return MTL::CompareFunctionLess;
        case DepthOp::LessOrEqual:          return MTL::CompareFunctionLessEqual;
        case DepthOp::Equal:          return MTL::CompareFunctionEqual;
        case DepthOp::Greater:              return MTL::CompareFunctionGreater;
        case DepthOp::GreaterOrEqual:       return MTL::CompareFunctionGreaterEqual;
        case DepthOp::Always:               return MTL::CompareFunctionAlways;
        }
        return MTL::CompareFunctionNever;
    }
    MTL::CullMode CullModeToMTLCullMode(const CullMode& mode)
    {
        switch(mode)
        {
            case CullMode::FrontFace:   return MTL::CullModeFront;
            case CullMode::BackFace:    return MTL::CullModeBack;
            case CullMode::None:        return MTL::CullModeNone;
        }
    }

    template<typename T>
    void SetDebugLabel(T* object, const std::string& label)
    {
        NS::String* l = NS::String::alloc()->string(label.c_str(), NS::UTF8StringEncoding);
        object->setLabel(l);
    }
    uint32_t GetMipLevelCount(uint32_t width, uint32_t height)
    {
        return (uint32_t)std::floor(std::log2(glm::min(width, height))) + 1;
    }
}
#endif
