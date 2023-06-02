#pragma once

#include "Utility/YamlUtils.h"
#include "Shader.h"

#include <glm/glm.hpp>

namespace HazardRenderer::Utils
{
    void Deserialize(YAML::Node node, size_t index, Buffer buffer, ShaderDataType type)
    {
        switch(type)
        {
        case ShaderDataType::Float:
        {
            float value = node.as<float>();
            buffer.Write(&value, sizeof(float), index);
            return;
        }
        case ShaderDataType::Float2:
        {
            glm::vec2 value = node.as<glm::vec2>();
            buffer.Write(&value, sizeof(glm::vec2), index);
            return;
        }
        case ShaderDataType::Float3:
        {
            glm::vec3 value = node.as<glm::vec3>();
            buffer.Write(&value, sizeof(glm::vec3), index);
            return;
        }
        case ShaderDataType::Float4:
        {
            glm::vec4 value = node.as<glm::vec4>();
            buffer.Write(&value, sizeof(glm::vec4), index);
            return;
        }
        case ShaderDataType::Int:
        {
            int value = node.as<int>();
            buffer.Write(&value, sizeof(int), index);
            return;
        }
        case ShaderDataType::Int2:
        {
            return;
        }
        case ShaderDataType::Int3:
        {
            return;
        }
        case ShaderDataType::Int4:
        {
            return;
        }
        case ShaderDataType::UInt:
        {
            return;
        }
        case ShaderDataType::UInt2:
        {
            return;
        }
        case ShaderDataType::UInt3:
        {
            return;
        }
        case ShaderDataType::UInt4:
        {
            return;
        }
        case ShaderDataType::Bool:
        {
            bool value = node.as<bool>();
            buffer.Write(&value, sizeof(bool), index);
            return;
        }
        default: break;
        }
    }
    void Serialize(YAML::Emitter& out, const std::string& key, void* value, ShaderDataType type)
    {
        switch(type)
        {
            case ShaderDataType::Float:
                YamlUtils::Serialize<float>(out, key, *(float*)value);
                break;
            case ShaderDataType::Float2:
                YamlUtils::Serialize(out, key, *(glm::vec2*)value);
                break;
            case ShaderDataType::Float3:
                YamlUtils::Serialize(out, key, *(glm::vec3*)value);
                break;
            case ShaderDataType::Float4:
                YamlUtils::Serialize(out, key, *(glm::vec4*)value);
                break;
            case ShaderDataType::Int:
                YamlUtils::Serialize<int>(out, key, *(int*)value);
                break;
            case ShaderDataType::Int2:
                YamlUtils::Serialize(out, key, *(glm::vec2*)value);
                break;
            case ShaderDataType::Int3:
                YamlUtils::Serialize(out, key, *(glm::vec3*)value);
                break;
            case ShaderDataType::Int4:
                YamlUtils::Serialize(out, key, *(glm::vec4*)value);
                break;
            case ShaderDataType::UInt:
                YamlUtils::Serialize(out, key, *(uint32_t*)value);
                break;
            case ShaderDataType::UInt2:
                YamlUtils::Serialize(out, key, *(glm::vec2*)value);
                break;
            case ShaderDataType::UInt3:
                YamlUtils::Serialize(out, key, *(glm::vec3*)value);
                break;
            case ShaderDataType::UInt4:
                YamlUtils::Serialize(out, key, *(glm::vec4*)value);
                break;
            case ShaderDataType::Bool:
                YamlUtils::Serialize(out, key, *(bool*)value);
                break;
            default: break;
        }
    }
}
