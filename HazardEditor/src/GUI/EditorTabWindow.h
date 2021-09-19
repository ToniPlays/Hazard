#pragma once

#include "RenderableElement.h"

namespace WindowElement {
	class EditorTabWindow : public RenderableElement {

	public:
		EditorTabWindow(const std::string& title) : m_Title(title) {}

		void OnRender() override;
		virtual void Render() = 0;
		virtual bool OnEvent(Event& e) = 0;

		const std::vector<RenderableElement*>& GetElements() { return m_Elements; }

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
		void SetLayerActive(bool active)
		{
			GetRenderable<T>()->SetActive(active);
		}

		template<typename T>
		static void SetWindowFocus() {
			EditorWindow& window = static_cast<EditorWindow&>(*s_Instance->GetRenderable<T>());
			ImGui::SetWindowFocus(window.GetTitle().c_str());
		}

	protected:
		bool m_Open = true;

	private:
		std::vector<RenderableElement*> m_Elements = std::vector<RenderableElement*>();
		std::string m_Title;
	};
}