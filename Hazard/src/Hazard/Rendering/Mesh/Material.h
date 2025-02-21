#pragma once

#include "Hazard/Assets/Asset.h"
#include "Core/Rendering/Pipeline.h"
#include "Core/Rendering/DescriptorSet.h"
#include "Hazard/RenderContext/Texture2D.h"

#include "UtilityCore.h"

#include "Core/Rendering/Shader.h"

namespace Hazard
{
	struct MaterialParam
	{
		std::string Name;
		HazardRenderer::ShaderDataType Type;
		uint32_t Offset;
	};

	struct TextureParam
	{
		std::string Name;
		uint32_t Binding;
		Ref<HazardRenderer::Image> Value;
		AssetHandle Handle = INVALID_ASSET_HANDLE;
	};

	class Material : public Asset
	{
	public:
		Material() = default;
		Material(AssetHandle pipeline);
		~Material();

		AssetType GetType() const override { return AssetType::Material; }

		const std::unordered_map<std::string, MaterialParam> GetMaterialParams() const { return m_MaterialParams; }
		const std::unordered_map<std::string, TextureParam> GetTextureParams() const { return m_TextureParams; }

		Buffer GetPushConstantData() const { return m_PushConstants; }
		void SetPushConstantData(Buffer data) { return m_PushConstants.Write(data.Data, data.Size); }

		AssetHandle GetPipelineHandle() { return m_PipelineHandle; }
		Ref<HazardRenderer::DescriptorSet> GetDescriptorSet() const { return m_DescriptorSet; };

		void SetPipeline(AssetHandle handle);
		bool Set(const std::string& name, Ref<HazardRenderer::Cubemap> cubemap);
		bool Set(const std::string& name, Ref<HazardRenderer::Image2D> texture);
		bool Set(const std::string& name, Ref<Texture2DAsset> texture)
		{
			bool success = Set(name, texture->GetSourceImage());
			if (success)
				m_TextureParams[name].Handle = texture->GetHandle();
			return success;
		}

		template<typename T>
		T GetConstant(const std::string& name)
		{
			if constexpr (std::is_same<T, void*>::value)
			{
				if (!m_MaterialParams.contains(name)) return nullptr;

				auto& param = m_MaterialParams[name];
				return m_PushConstants.ReadBytes(ShaderDataTypeSize(param.Type), param.Offset);
			}
			else 
			{
				if (!m_MaterialParams.contains(name)) return T();

				auto& param = m_MaterialParams[name];
				return m_PushConstants.Read<T>(param.Offset);
			}
		}

		template<typename T>
		void SetConstant(const std::string& name, T value)
		{
			auto& param = m_MaterialParams[name];
			if constexpr (std::is_same<T, void*>::value)
				m_PushConstants.Write(value, ShaderDataTypeSize(param.Type), param.Offset);
			else
				m_PushConstants.Write(&value, ShaderDataTypeSize(param.Type), param.Offset);
		}

	private:
		void Invalidate();
		void InvalidatePushConstants();
		void InvalidateDescriptorSet();

	private:
		AssetHandle m_PipelineHandle = INVALID_ASSET_HANDLE;
		Ref<HazardRenderer::DescriptorSet> m_DescriptorSet;
		std::unordered_map<std::string, MaterialParam> m_MaterialParams;
		std::unordered_map<std::string, TextureParam> m_TextureParams;

		Buffer m_PushConstants;
	};
}
