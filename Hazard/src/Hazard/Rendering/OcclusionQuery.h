#pragma once

#include "Hazard/Core/Core.h"


namespace Hazard::Rendering {

	class OcclusionQuery : public RefCount {
	public:
		OcclusionQuery() {};
		virtual ~OcclusionQuery() {};

		virtual void BeginQuery() = 0;
		virtual void EndQuery() = 0;

		virtual bool IsAvailable() const = 0;
		virtual int GetResult() = 0;
		virtual bool InUse() const = 0;
		virtual void Flush() = 0;

		virtual uint32_t GetID() const = 0;

	};
}