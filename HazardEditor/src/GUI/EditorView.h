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

		void OnEvent(Event& e);

		void Close() override;

		template<typename T>
		T& PushRenderable() {
			T* w = new T();
			elements.push_back(w);
			w->Init();
			HZR_INFO("Added renderable " + std::string(typeid(T).name()));
			return *w;
		} 
	public:
		static EditorView& GetInstance() { return *instance; }
	private:
		std::vector<RenderableElement*> elements;
		Rendering::RenderContext* context;

	private:
		static EditorView* instance;

	private:
		void BeginFrame();
		void EndFrame();
	};
}