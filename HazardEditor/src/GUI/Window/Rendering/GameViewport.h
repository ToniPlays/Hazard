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
		~GameViewport();

		void Init() override;
		void OnWindowRender() override;

	private:
		Ref<RenderTexture> renderTexture;
		uint32_t width = 0, height = 0;
	};
}