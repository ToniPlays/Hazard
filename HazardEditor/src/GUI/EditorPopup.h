#pragma once

#include "RenderableElement.h"
#include "Event.h"
#include "imgui.h"

namespace WindowElement {

	class EditorPopup : public RenderableElement {
	public:

		EditorPopup(const char* title);
		~EditorPopup() = default;

		virtual void Init() {};
		virtual void OnWindowRender() = 0;
		virtual bool OnEvent(Event& e) { return false; }
		void OnRender();

	protected:
		ImVec2 m_Size;
		bool open = true;
	private:
		const char* m_Title;
	};
}