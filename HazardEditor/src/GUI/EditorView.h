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
		void PushRenderable() {
			T* w = new T();
			elements.push_back(w);
			w->Init();
			HZR_INFO("Added renderable " + std::string(typeid(T).name()));
		} 

	private:
		std::vector<RenderableElement*> elements;
		Rendering::RenderContext* context;
	private:
		void BeginFrame();
		void EndFrame();
	};
}