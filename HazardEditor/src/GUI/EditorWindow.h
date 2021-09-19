#pragma once

#include "RenderableElement.h"
#include "imgui.h"

namespace WindowElement {

	class EditorWindow : public RenderableElement {
	public:
		EditorWindow(const char* title, ImGuiWindowFlags flags = 0);
		~EditorWindow() = default;


		void OnRender() override;
		virtual void Init() {};
		virtual void OnWindowRender() = 0;
		virtual void OnBeforeRender() {};
		virtual void OnAfterRender() {};
		virtual bool OnEvent(Event& e) override { return false; };

		bool GetActive() { return m_Active; }
		bool IsFocused() { return m_Focused; }
		bool IsHovered() { return m_Hovered; }

		std::string& GetTitle() { return m_Title; }
		void SetActive(bool active) { this->m_Active = active; }

	protected:
		std::string m_Title = "Untitled";

	private:
		bool m_Active = true;
		bool m_Focused = false;
		bool m_Hovered = false;

		ImGuiWindowFlags m_Flags;
	};
}