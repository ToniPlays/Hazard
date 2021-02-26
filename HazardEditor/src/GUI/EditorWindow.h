#pragma once

#include "RenderableElement.h"

namespace WindowElement {

	class EditorWindow : public RenderableElement {
	public:
		EditorWindow();
		EditorWindow(const char* title);
		~EditorWindow();


		void OnRender() override;
		virtual void Init() {};
		virtual void OnWindowRender() = 0;

		bool GetActive() { return active; }

		void SetActive(bool active) { this->active = active; }
	protected:
		std::string title = "Untitled";
	private:
		bool active = true;
	};
}