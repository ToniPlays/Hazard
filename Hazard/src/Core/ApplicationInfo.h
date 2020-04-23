#pragma once
#include "Core.h"
#include "Rendering/RenderAPI/GraphicsContext.h"
#include <iostream>

namespace Hazard {
	class HAZARD_API ApplicationInfo {
	public:

		ApplicationInfo(std::string _title) : title(_title) {
			renderer = Renderer();
		}

		std::string title;


		struct Renderer {

			std::string renderer = "Unknown";
			std::string graphicProcessor = "Unknown";
		};
		Renderer renderer;
	};
}