#pragma once

namespace WindowElement {

	class RenderableElement {
	public:
		RenderableElement() {};
		virtual ~RenderableElement() {};

		virtual void Init() {};
		virtual void OnFrameBegin() {};
		virtual void OnFrameEnd() {};
		virtual void OnMenuRender() {};
		virtual void OnRender() {};
		virtual bool OnEvent(Event& e) { return false; };
	};
}