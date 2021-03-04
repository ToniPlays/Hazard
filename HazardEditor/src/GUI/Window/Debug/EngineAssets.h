#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

using namespace Hazard::Rendering;
namespace WindowElement {
	class EngineAssets : public EditorWindow {
	public:
		EngineAssets();
		~EngineAssets();

		void Init() override;
		void OnWindowRender() override;
	};
}