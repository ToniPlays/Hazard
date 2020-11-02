#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard {
	class HAZARD_API CubemapTexture {
	public:

		CubemapTexture(const char* refname) : name(refname)  {};
		virtual ~CubemapTexture() {};

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Revalidate(std::vector<std::string> sources) = 0;

		virtual uint32_t GetID() const = 0;

	protected:
		const char* name;
	};
}