#pragma once

#include "Hazard/Assets/Asset.h"

namespace Hazard::Rendering {
	struct MeshAsset : public Asset 
	{
		friend class MeshLoader;
	public:
		float GetScaleFactor() { return m_ScaleFactor; }
		UUID GetSourceHandle() { return m_MeshSourceHandle; }
	private:
		float m_ScaleFactor = 1.0f;
		UUID m_MeshSourceHandle = INVALID_ASSET_HANDLE;
	};
}