#pragma once
#include "Hazard/Core/Core.h"
#include "Texture.h"

namespace Hazard::Rendering 
{
	struct Texture2DCreateInfo 
	{
		std::filesystem::path filePath;
	};

	enum TextureDataType {Auto = 0, RGB, RGBA, HRD, HDRA };

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind(uint32_t slot = 0) const = 0;

		static Texture2D* Create(Texture2DCreateInfo* info);
	};
}
