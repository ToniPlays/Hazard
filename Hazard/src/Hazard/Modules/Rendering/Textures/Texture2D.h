#pragma once

#include "Hazard/Core/Core.h"
#include "Texture.h"

namespace Hazard {
	class HAZARD_API Texture2D : public Texture {
	public:

		virtual ~Texture2D() {};

	protected:
		std::string path;
	};
}