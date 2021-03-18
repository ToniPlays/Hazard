#pragma once

#include "RenderableElement.h"
#include "imgui.h"

namespace WindowElement {

	class EditorWindow : public RenderableElement {
	public:
		EditorWindow();
		EditorWindow(const char* title);
		~EditorWindow();


		void OnRender() override;
		virtual void Init() {};
		virtual void OnWindowRender() = 0;
		virtual bool OnEvent(Event& e) override { return false; };

		bool GetActive() { return active; }
		bool IsFocused() { return focused; }
		bool IsHovered() { return hovered; }

		std::string& GetTitle() { return title; }

		void SetActive(bool active) { this->active = active; }
	protected:
		std::string title = "Untitled";
	private:
		bool focused = false;
		bool hovered = false;
		bool active = true;
	};
}