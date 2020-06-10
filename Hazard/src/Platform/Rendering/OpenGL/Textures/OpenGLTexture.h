#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Modules/Renderer/Textures/Texture2D.h"
#include <iostream>

namespace Hazard {

	class HAZARD_API OpenGLTexture2D : public Texture2D {
	public:

		OpenGLTexture2D(std::string file);
		~OpenGLTexture2D();
		
		void Bind(uint32_t slot) const override;
		void Unbind(uint32_t slot) const override;

		uint32_t GetID() const { return textureID; };

	private:
		uint32_t textureID;
	};
}