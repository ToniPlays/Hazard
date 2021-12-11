#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

#include "Editor/EditorCamera.h"

using namespace Hazard;
using namespace Hazard::Rendering;

namespace WindowElement {

	class GameViewport : public EditorWindow {
	public:
		GameViewport();
		~GameViewport() = default;

		void Init() override;
		void OnWindowRender() override;

	private:
		Ref<WorldRenderer> m_Renderer;
		uint32_t m_Width = 0, m_Height = 0;
	};
}