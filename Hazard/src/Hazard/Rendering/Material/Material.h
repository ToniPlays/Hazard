#pragma once

#include "../RenderUtils.h"

namespace Hazard::Rendering {

	struct MaterialCreateInfo 
	{

	};


	class Material : public RefCount {
	public:
		Material(GraphicsPipeline* pipeline);
		~Material();

		GraphicsPipeline* GetPipeline() { return m_Pipeline; }
		std::vector<Ref<Texture2D>> GetTextures() { return m_Textures; }
		void SetTextures(const std::vector<Ref<Texture2D>>& textures) { this->m_Textures = textures; };

	public:
		static Ref<Material> Create();

	private:
		std::vector<Ref<Texture2D>> m_Textures;
		GraphicsPipeline* m_Pipeline;
	};
}