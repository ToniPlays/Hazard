#pragma once

namespace WindowElement {

	class EditorWindow {
	public:
		EditorWindow();
		EditorWindow(const char* title);
		~EditorWindow();


		void RenderWindow();
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