#pragma once

#include "Hazard/Assets/Asset.h"

namespace Hazard::Rendering 
{
	class Texture : public Asset {
	public:
		Texture() = default;

		virtual void* GetID() const = 0;
	};
}