#pragma once

#include "Hazard/Core/Core.h"
#include "Texture.h"

namespace Hazard::Rendering {

	struct Texture2DCreateInfo
	{
		std::string filename = "";
		uint32_t width, height;
		TextureDataType datatype;
		void* data = nullptr;
	};

	class Texture2D : public Texture {

	public:
		virtual ~Texture2D() {};

		virtual void SetData(void* data, uint32_t size) = 0;
	};
}