#pragma once

#include <glm/glm.hpp>
#include "Skybox.h"

namespace Hazard::Rendering {

	class BackgroundRenderer {
	public:
		BackgroundRenderer() {};
		virtual ~BackgroundRenderer() {};

		virtual float GetGamma() { return 1.0f; }
		virtual Texture* GetTexture() { return nullptr; };
		virtual void SetCubemap(CubemapTexture* texture) {};

		virtual void Render(glm::mat4 view, glm::mat4 projection) {};
		
		Color m_Color;
	};
	
	class ColorBackgroundRenderer : public BackgroundRenderer
	{
	public:
		ColorBackgroundRenderer() {};
		void Render(glm::mat4 view, glm::mat4 projection) override;
	};
	class SkyboxBackgroundRenderer : public BackgroundRenderer
	{
	public:
		SkyboxBackgroundRenderer();

		float GetGamma() { return m_Skybox->GetGamma(); }
		Texture* GetTexture() { return m_Skybox->GetCubemapTexture(); };
		Skybox* GetSkybox() { return m_Skybox; };
		Texture2D* GetRaw();

		void Render(glm::mat4 view, glm::mat4 projection) override;
		void SetCubemap(CubemapTexture* texture) override;
	private:
		Skybox* m_Skybox = nullptr;
	};
}