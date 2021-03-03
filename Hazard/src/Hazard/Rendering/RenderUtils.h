#pragma once

#include "Textures/RenderTexture.h"
#include "Hazard/Rendering/RenderContext.h"

namespace Hazard::Rendering {

	struct RenderStats {
		uint32_t drawCalls = 0;
		uint32_t quads = 0;
		uint32_t vertices = 0;
		uint32_t indices = 0;
	};

	class RenderUtils {
		public:
			static void Init();
			static void SetRenderAPI(RenderAPI api) { RenderUtils::api = api; }

			template<class T>
			static T* Create();

			template<class T, typename Arg>
			static T* Create(Arg arg);

			static Texture2D* GetWhiteTexture() { return whiteTexture; }

	private:
		static RenderAPI api;
		static Texture2D* whiteTexture;
	};
}