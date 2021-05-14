#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Textures/Texture2D.h"
#include "glm/glm.hpp"

namespace Hazard::Rendering::Vulkan {

	class VKTexture2D : public Texture2D {
	public:

		VKTexture2D(const char* file, TextureSpecs params);
		~VKTexture2D();

		void Bind(uint32_t slot) const override;
		void Unbind(uint32_t slot) const override;

		virtual uint32_t GetWidth() const { return m_Spec.width; }
		virtual uint32_t GetHeight() const { return m_Spec.height; }
		virtual void SetData(void* data, uint32_t size) override;

		uint32_t GetID() const { return m_TextureID; };
		TextureSpecs& GetSpec() { return m_Spec; };

	private:

		TextureSpecs m_Spec;
		uint32_t m_TextureID;
	};
}