#pragma once

namespace WindowElement {

	class RenderableElement {
	public:
		RenderableElement() = default;
		virtual ~RenderableElement() = default;

		virtual void Init() {};
		virtual void OnFrameBegin() {};
		virtual void OnFrameEnd() {};
		virtual void OnMenuRender() {};
		virtual void OnRender() {};
		virtual bool OnEvent(Event& e) { return false; };
	};
}