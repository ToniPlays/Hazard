#pragma once

#include "Hazard.h"
#include "EditorWindow.h"

using namespace Hazard;

namespace WindowElement {
	class EditorView : public Module::Module {
	public:
		EditorView();
		~EditorView();

		void Init();
		void Render();

		void Flush();

		template<typename T>
		void PushWindow() {
			windows.push_back(new T());
		}

	private:
		std::vector<EditorWindow*> windows;
		Rendering::RenderContext* context;
	private:
		void BeginFrame();
		void EndFrame();
	};
}