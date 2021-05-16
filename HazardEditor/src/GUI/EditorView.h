#pragma once

#include "Hazard.h"
#include "EditorWindow.h"
#include "GUI/Window/Toolbar.h"
#include "GUI/Window/MenuBar.h"

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
			m_Elements.push_back(w);
			w->Init();
			return *w;
		} 
		template<typename T>
		T* GetRenderable() {

			for (RenderableElement* e : m_Elements) {
				if (dynamic_cast<T*>(e))
					return (T*)e;
			}
			return nullptr;
		}
		template<typename T>
		void RemoveRenderable() {

			for (size_t i = 0; i < m_Elements.size(); i++) {
				RenderableElement* e = m_Elements[i];
				if (dynamic_cast<T*>(e)) {
					m_Elements.erase(m_Elements.begin() + i);
					return;
				}
			}
		}
		template<typename T>
		void SetLayerActive(bool active) 
		{
			GetRenderable<T>()->SetActive(active);
		}
		template<typename T>
		static void SetWindowFocus() {
			EditorWindow& window = static_cast<EditorWindow&>(*s_Instance->GetRenderable<T>());
			ImGui::SetWindowFocus(window.GetTitle().c_str());
		}

	public:
		static EditorView& GetInstance() { return *s_Instance; }

	private:
		void BeginFrame();
		void EndFrame();

		void InitImGuiPlatform(Rendering::Window& window);

	private:
		static EditorView* s_Instance;

	private:
		std::vector<RenderableElement*> m_Elements = std::vector<RenderableElement*>();
		Rendering::RenderContext* m_Context;

		MenuBar m_MenuBar;
		Toolbar m_Toolbar;
		RenderAPI m_Api;

	};
}