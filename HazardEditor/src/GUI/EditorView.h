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
		bool OnEvent(Event& e);

		void Close() override;

		template<typename T>
		T& PushRenderable() {
			T* w = new T();
			elements.push_back(w);
			w->Init();
			return *w;
		} 
		template<typename T>
		T& GetRenderable() {
			for (RenderableElement* e : elements) {
				if (dynamic_cast<T*>(e))
					return (T&)*e;
			}
		}
		template<typename T>
		void SetLayerActive(bool active) 
		{
			GetRenderable<T>().SetActive(active);
		}
		template<typename T>
		static void SetWindowFocus() {
			EditorWindow& window = static_cast<EditorWindow&>(instance->GetRenderable<T>());
			ImGui::SetWindowFocus(window.GetTitle().c_str());
		}

	public:
		static EditorView& GetInstance() { return *instance; }
	private:
		std::vector<RenderableElement*> elements = std::vector<RenderableElement*>();
		Rendering::RenderContext* context;

	private:
		static EditorView* instance;

	private:
		void BeginFrame();
		void EndFrame();
	};
}