#pragma once

#include <hzrpch.h>
#include "Hazard/RenderContext/Pipeline/RenderPass.h"
#include "Hazard/RenderContext/Pipeline/Buffers.h"
#include "Hazard/RenderContext/Pipeline/Pipeline.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "EditorCamera.h"

using namespace Hazard::Rendering;

namespace Editor {
	class Grid 
	{
	public:
		Grid() = default;
		~Grid();

		void Render(const Editor::EditorCamera& camera);
		void Invalidate(const Ref<RenderPass> renderPass);

	private:
		Ref<Pipeline> m_Pipeline;
	};
}