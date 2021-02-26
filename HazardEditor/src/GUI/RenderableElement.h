#pragma once

namespace WindowElement {
	class RenderableElement {
	public:
		RenderableElement() {};
		virtual ~RenderableElement() {};

		virtual void Init() {};
		virtual void OnMenuRender() {};
		virtual void OnRender() {};
	};
}