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
		virtual void Render() {};
		virtual void SetGamma(float gamma) {};
		virtual std::string GetFile() { return ""; }
		
		Color m_Color;
	};
	
	class ColorBackgroundRenderer : public BackgroundRenderer
	{
	public:
		ColorBackgroundRenderer() {}
	};
	class SkyboxBackgroundRenderer : public BackgroundRenderer
	{
	public:
		SkyboxBackgroundRenderer(const std::string& file);
		~SkyboxBackgroundRenderer();

		float GetGamma() { return m_Skybox->GetGamma(); }
		void SetGamma(float gamma) override { m_Skybox->SetGamma(gamma); }
		Ref<CubemapTexture> GetTexture() { return m_Skybox->GetCubemapTexture(); };
		Ref<Skybox> GetSkybox() { return m_Skybox; };
		Ref<Texture2D> GetRaw();
		std::string GetFile() override { return GetTexture()->GetData().file; }


		void Render() override;
		void SetCubemap(Ref<CubemapTexture>& texture) override;
	private:
		Ref<Skybox> m_Skybox;
	};
}