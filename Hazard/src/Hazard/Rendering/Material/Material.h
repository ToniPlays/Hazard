#pragma once

#include "../RenderUtils.h"

namespace Hazard::Rendering {

	struct MaterialCreateInfo 
	{

	};
	struct MaterialBuffer 
	{
		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float metallic = 0.0f;
		float smoothness = 0.0f;
	};

	class Material : public RefCount {
	public:
		Material();
		~Material();

		void Bind();

		MaterialBuffer GetMaterialData() const { return m_MaterialBuffer; }
		void SetPipeline(Ref<GraphicsPipeline> pipeline) { 
			m_Pipeline = std::move(pipeline); 
		}
		GraphicsPipeline* GetPipeline() { return m_Pipeline.Raw(); }

		template<typename T>
		T Get(const char* value) {
			static_assert(false);
		}

		template<>
		float Get(const char* value) 
		{
			if (strcmp(value, "Material.Metallic") == 0)
				return m_MaterialBuffer.metallic;
			if (strcmp(value, "Material.Smoothness") == 0)
				return m_MaterialBuffer.smoothness;
			return 0.0f;
		}
		template<>
		Color Get(const char* value) 
		{
			if (strcmp(value, "Material.AlbedoColor") == 0) 
				return Color::FromGLM(m_MaterialBuffer.color);
			return Color();
		}
		template<>
		Texture2D* Get(const char* value)
		{
			if (strcmp(value, "Material.AlbedoMap") == 0)
				return m_Textures[0].Raw();
			if (strcmp(value, "Material.NormalMap") == 0)
				return m_Textures[1].Raw();
			return nullptr;
		}
		template<typename T>
		void Set(const char* key, T value) {
			static_assert(false);
		}
		template<>
		void Set(const char* key, float value)
		{
			if (strcmp(key, "Material.Metallic") == 0) {
				m_MaterialBuffer.metallic = value;
				return;
			}
			if (strcmp(key, "Material.Smoothness") == 0) {
				m_MaterialBuffer.smoothness = value;
				return;
			}
		}
		template<>
		void Set(const char* key, Color color) 
		{
			if (strcmp(key, "Material.AlbedoColor") == 0) {
				m_MaterialBuffer.color = color;
			}
		}
		template<>
		void Set(const char* key, Ref<Texture2D> texture)
		{
			if (strcmp(key, "Material.AlbedoMap") == 0) {
				m_Textures[0] = texture;
			}
			if (strcmp(key, "Material.NormalMap") == 0) {
				m_Textures[1] = texture;
			}
		}

	public:
		static Ref<Material> Create();

	private:

		std::vector<Ref<Texture2D>> m_Textures;

		MaterialBuffer m_MaterialBuffer;
		Ref<GraphicsPipeline> m_Pipeline;
	};
}