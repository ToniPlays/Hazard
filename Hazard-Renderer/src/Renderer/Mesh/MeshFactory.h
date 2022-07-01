#pragma once

#include "MeshCreateInfo.h"

namespace HazardRenderer 
{
	class MeshFactory {
	public:
		MeshFactory() = default;
		~MeshFactory() = default;

		void SetOptimization(uint32_t flags);
		void LoadMesh(const std::string& file);

	private:
		uint32_t m_MeshFlags = 0;
	};
}