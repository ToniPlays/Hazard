#pragma once

#include <hzrpch.h>
#include "EditorCamera.h"
#include "HazardRendererCore.h"

namespace Editor {
	class Grid 
	{
	public:
		Grid() = default;
		~Grid();

		void Render(const Editor::EditorCamera& camera);
		bool IsVisible() { return m_ShowGrid; }
		void SetVisible(bool visible) { m_ShowGrid = visible; }

	private:
		Ref<HazardRenderer::GPUBuffer> m_GridUniformBuffer;
		Ref<HazardRenderer::Pipeline> m_Pipeline = nullptr;
		bool m_ShowGrid = true;
	};
}
