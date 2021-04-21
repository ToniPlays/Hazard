#pragma once

#include "Hazard/Rendering/OcclusionQuery.h"

namespace Hazard::Rendering::OpenGL {

	class OpenGLOcclusionQuery : public OcclusionQuery {
	public:
		OpenGLOcclusionQuery();
		~OpenGLOcclusionQuery() override;

		void BeginQuery() override;
		void EndQuery() override;

		bool IsAvailable() const override;
		bool InUse() const override { return m_InUse; };
		int GetResult() override;
		void Flush() override;

		uint32_t GetID() const override;
	private:
		uint32_t m_QueryID;
		bool m_InUse = false;
	};
}