#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Textures/Texture2D.h"
#include "glm/glm.hpp"

namespace Hazard::Rendering::Vulkan {

	class VKTexture2D : public Texture2D {
	public:

		VKTexture2D(const Texture2DCreateInfo& info);
		~VKTexture2D();

		void Bind(uint32_t slot) const override;
		void Unbind(uint32_t slot) const override;

		virtual uint32_t GetWidth() const { return 0; }
		virtual uint32_t GetHeight() const { return 0; }
		virtual void SetData(void* data, uint32_t size) override;

		uint32_t GetID() const { return m_TextureID; };
		Texture2DInfo GetData() const { return m_Info; }

	private:
		Texture2DInfo m_Info;
		uint32_t m_TextureID;
	};
}