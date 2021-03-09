#pragma once

#include "RenderEngine.h"
namespace Hazard::Rendering {

	class RenderCommand {
	public:
		static void Init();
	private:
		RenderEngine engine;
	};
}