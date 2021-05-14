#pragma once

#include <glm/glm.hpp>
#include "Skybox.h"

namespace Hazard::Rendering {

	class BackgroundRenderer {
	public:
		BackgroundRenderer() {};
		virtual ~BackgroundRenderer() {};

		virtual float GetGamma() { return 1.0f; }
		virtual void SetCubemap(Ref<CubemapTexture>& texture) {};
		virtual void Render(glm::mat4 view, glm::mat4 projection) {};
		
		Color m_Color;
	};
	
	class ColorBackgroundRenderer : public BackgroundRenderer
	{
	public:
		ColorBackgroundRenderer() {}
		void Render(glm::mat4 view, glm::mat4 projection) override;
	};
	class SkyboxBackgroundRenderer : public BackgroundRenderer
	{
	public:
		SkyboxBackgroundRenderer();
		~SkyboxBackgroundRenderer();

		float GetGamma() { return m_Skybox->GetGamma(); }
		Ref<CubemapTexture> GetTexture() { return m_Skybox->GetCubemapTexture(); };
		Ref<Skybox> GetSkybox() { return m_Skybox; };
		Ref<Texture2D> GetRaw();

		void Render(glm::mat4 view, glm::mat4 projection) override;
		void SetCubemap(Ref<CubemapTexture>& texture) override;
	private:
		Ref<Skybox> m_Skybox;
	};
}