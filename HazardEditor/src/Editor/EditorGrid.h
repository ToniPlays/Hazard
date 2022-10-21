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
		void Invalidate(Ref<HazardRenderer::RenderPass> renderPass);
		bool IsVisible() { return m_ShowGrid; }
		void SetVisible(bool visible) { m_ShowGrid = visible; }

	private:
		Ref<HazardRenderer::UniformBuffer> m_GridUniformBuffer;
		Ref<HazardRenderer::Pipeline> m_Pipeline;
		bool m_ShowGrid = true;
	};
}