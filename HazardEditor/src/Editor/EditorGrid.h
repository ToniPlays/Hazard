#pragma once

#include "Hazard.h"
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
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<Pipeline> m_Pipeline;
	};
}