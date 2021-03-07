#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

using namespace Hazard::Rendering;

namespace WindowElement {
	class Console : public EditorWindow {
	public:
		Console();
		~Console();

		void Init() override;
		void OnWindowRender() override;
	};
}