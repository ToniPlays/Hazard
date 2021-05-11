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

		static void SetRenderAPI(RenderAPI api) { s_Api = api; }

		template<typename T>
		static T* CreateRaw();

		template<typename T, typename Arg>
		static T* CreateRaw(Arg arg);

		template<typename T>
		static Ref<T> Create();

		template<typename T, typename Arg>
		static Ref<T> Create(Arg arg);

		template<typename T, typename F, typename Arg>
		static Ref<T> Create(F file, Arg args);

	private:
		static RenderAPI s_Api;
	};
}