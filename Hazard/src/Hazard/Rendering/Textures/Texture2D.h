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

	struct Texture2DInfo 
	{
		uint32_t width = 0;
		uint32_t height = 0;
		TextureDataType datatype;
	};

	class Texture2D : public Texture {

	public:
		Texture2D(const Texture2DCreateInfo& info) : Texture(info.filename) {};
		virtual ~Texture2D() {};

		virtual void SetData(void* data, uint32_t size) = 0;
		virtual Texture2DInfo GetData() const = 0;

	};
}