#pragma once

#include "Hazard/Rendering/Textures/EnvironmentMap.h"
#include "Hazard/Rendering/Buffers/VertexArray.h"
#include "Hazard/Rendering/Textures/Texture2D.h"

namespace Hazard::Rendering::OpenGL {

	class OpenGLEnvironmentMap : public EnvinronmentMap {
	public:
		OpenGLEnvironmentMap(const char* file, TextureSpecs& specs);
		~OpenGLEnvironmentMap();

		void SetTexture(int side, const std::string& file) override;
		void SetIrradiance(float irradiance) override;

		uint32_t GetWidth() const override { return 0; }
		uint32_t GetHeight() const override { return 0; }

		void Bind(uint32_t slot = 0) const override;
		void Unbind(uint32_t slot) const override;

		void GenerateIrradiance() override;
		void GeneratePreFilter() override;

		TextureSpecs& GetSpec() override { return m_Spec; }
		uint32_t GetID() const override { return m_rendererID; }

	private:
		void GenerateCubemap();

	private:

		TextureSpecs m_Spec;
		uint32_t m_rendererID;

		Texture2D* m_RawTexture;

		CubemapTexture* m_Cubemap = nullptr;
	};
}