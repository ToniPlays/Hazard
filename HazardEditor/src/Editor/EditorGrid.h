#pragma once

#include <hzrpch.h>
#include "EditorCamera.h"


namespace Editor {
	class Grid 
	{
	public:
		Grid() = default;
		~Grid();

		void Render(const Editor::EditorCamera& camera);
		//void Invalidate(const Ref<RenderPass> renderPass);

	private:
		//Ref<Pipeline> m_Pipeline;
	};
}