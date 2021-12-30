#pragma once
#include <hzrpch.h>
#include "GUI/EditorWindow.h"
#include "Hazard/Rendering/WorldRenderer.h"
#include "Editor/EditorCamera.h"

using namespace Hazard;
using namespace Hazard::Rendering;

namespace WindowElement {

	class GameViewport : public EditorWindow {
	public:
		GameViewport();
		~GameViewport() = default;

		void Init() override;
		void OnUpdate() override;
		void OnWindowRender() override;

	private:

		Camera m_Camera;
		bool m_Resize = false;
		Ref<WorldRenderer> m_Renderer;
		uint32_t m_Width = 0, m_Height = 0;
	};
}