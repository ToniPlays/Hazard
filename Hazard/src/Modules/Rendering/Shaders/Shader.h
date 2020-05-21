#pragma once

#include "Core/Core.h"
namespace Hazard {

	class HAZARD_API Shader {
	public:

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		static Shader* Create(const std::string& vertex, const std::string& fragment);
	protected:
		uint32_t program;
	};
}