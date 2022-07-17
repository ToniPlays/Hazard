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

	private:
		Ref<HazardRenderer::Pipeline> m_Pipeline;
	};
}