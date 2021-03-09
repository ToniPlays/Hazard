#pragma once

#include "RenderEngine.h"
namespace Hazard::Rendering {

	class RenderCommand {
	public:
		static void Init();
		template<typename T>
		static void Submit(T element) { engine.Submit(element); }

	private:
		static RenderEngine engine;
	};
}