#pragma once

#include "Textures/RenderTexture.h"
#include "Hazard/Rendering/RenderContext.h"

namespace Hazard::Rendering {

	class RenderUtils {
		public:

			static void SetRenderAPI(RenderAPI api) { RenderUtils::api = api; }

			template<class T>
			static T* Create();
			template<class T, typename Arg>
			static T* Create(Arg arg);
	private:
		static RenderAPI api;
	};
}