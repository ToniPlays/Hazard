#pragma once

namespace WindowElement {
	class EditorTabWindow {

	public:
		virtual void Init() = 0;
		virtual void Render() = 0;
	};
}