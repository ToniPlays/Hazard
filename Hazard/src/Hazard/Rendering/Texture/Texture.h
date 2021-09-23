#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard::Rendering 
{

	enum TextureDataType {Auto = 0, RGB, RGBA, HRD, HDRA };

	class Texture 
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() = 0;
		virtual uint32_t GetHeight() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void* GetID() const = 0;
	};
}
