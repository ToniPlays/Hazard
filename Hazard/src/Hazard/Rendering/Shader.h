#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Rendering
{
	class Shader {
	public:
		virtual void Reload() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}
