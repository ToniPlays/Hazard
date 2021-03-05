#pragma once

#include "Textures/RenderTexture.h"
#include "Hazard/Rendering/RenderContext.h"

namespace Hazard::Rendering {

	struct RenderStats {
		uint32_t drawCalls = 0;
		uint32_t quads = 0;
		uint32_t vertices = 0;
		uint32_t indices = 0;
		uint32_t textures = 0;
	};

	class RenderUtils {
		public:
			static void Init();
			static void Flush();

			static void SetRenderAPI(RenderAPI api) { RenderUtils::api = api; }

			template<typename T>
			static T* Create();

			template<typename T, typename Arg>
			static T* Create(Arg arg);

			template<typename T, typename Arg, typename C>
			static T* Create(Arg arg, C name);

			template<typename T, typename Arg>
			static T* Find(Arg args);

			static Texture* GetTexture(uint32_t textureID);
			static Texture* GetFromTextures(uint32_t index);
			static uint32_t GetTextureStackIndex(Texture* texture);
			static void UnloadTexture(Texture* texture);

			static std::vector<Texture*> GetTextures() { return textures; }
	private:
		static RenderAPI api;
		static std::vector<Texture*> textures;

	};
}