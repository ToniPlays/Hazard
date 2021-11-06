#pragma once

#include "Hazard.h"

namespace WindowElement 
{

	class RenderableElement {
	public:
		RenderableElement() = default;
		virtual ~RenderableElement() = default;

		virtual void Init() {};
		virtual void OnFrameBegin() {};
		virtual void OnFrameEnd() {};
		virtual void OnMenuRender() {};
		virtual void OnRender() {};
		virtual bool OnEvent(Hazard::Event& e) { return false; };
	};
}