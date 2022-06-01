#pragma once

#include <hzrpch.h>
#include "EditorWindow.h"
#include "GUI/Window/Toolbar.h"
#include "GUI/Window/MenuBar.h"
#include "Backend/Core/Window.h"
#include "Backend/Core/Events.h"
#include "Platform/EditorGuiPlatform.h"
#include "Event.h"

namespace WindowElement {

	using namespace HazardRenderer;

	class EditorView : public Module::Module {
	public:
		EditorView();
		~EditorView();

		void Init() override;
		void Update() override;
		void Render() override;
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
					HZR_INFO("Removed Renderable {0}", typeid(T).name());
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

	public:
		static EditorView& GetInstance() { return *s_Instance; }
		static EditorGuiPlatform& GetContext() { return *s_Instance->m_Renderer; }

	private:
		void BeginFrame();
		void EndFrame();

		//void InitImGuiPlatform(HazardRenderer::Window& window);
		bool OnWindowFocus(WindowFocusEvent& e);

	private:
		static EditorView* s_Instance;

	private:
		std::vector<RenderableElement*> m_Elements = std::vector<RenderableElement*>();
		MenuBar m_MenuBar;
		Toolbar m_Toolbar;
		EditorGuiPlatform* m_Renderer = nullptr;
		//Rendering::RenderContext* m_Context = nullptr;
	};
}
