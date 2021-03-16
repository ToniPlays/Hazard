#pragma once

#include <glm/glm.hpp>
#include "Skybox.h"

namespace Hazard::Rendering {

	class BackgroundRenderer {
	public:
		virtual void Render(glm::mat4 transform) = 0;
		virtual glm::vec4 ClearColor() = 0;
	};

	class ColorBackground : public BackgroundRenderer {
	public:
		virtual void Render(glm::mat4 transform) override;
		virtual glm::vec4 ClearColor() override { return color; }
	private:
		glm::vec4 color;
	};

	class SkyboxBackground : public BackgroundRenderer {
	public:

		SkyboxBackground();
		SkyboxBackground(CubemapTexture* texture);
		~SkyboxBackground();

		void Render(glm::mat4 transform) override;
		glm::vec4 ClearColor() override { return glm::vec4(0, 0, 0, 1.0); }
		void SetSkyboxCubemap(CubemapTexture* texture) { skybox->SetCubemapTexture(texture); }

	private:
		Skybox* skybox;
	};
}