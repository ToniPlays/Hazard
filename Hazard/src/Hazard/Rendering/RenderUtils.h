#pragma once

#include "Hazard/Rendering/Textures/RenderTexture.h"
#include "Hazard/Rendering/Textures/CubemapTexture.h"
#include "Hazard/Rendering/Textures/EnvironmentMap.h"

#include "Hazard/Rendering/Shader/Shader.h"
#include "Hazard/RenderContext/RenderContext.h"

namespace Hazard::Rendering {

	class RenderUtils {
	public:
		static void Init();
		static void Flush();
		static void UnloadPending();

		static void SetRenderAPI(RenderAPI api) { s_Api = api; }

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

		static std::vector<Shader*> GetShaders() { return s_Shaders; }
		static std::vector<Texture*> GetTextures() { return s_Textures; }
	private:
		static RenderAPI s_Api;
		static std::vector<Shader*> s_Shaders;
		static std::vector<Texture*> s_Textures;

	};
}