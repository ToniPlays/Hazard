#pragma once

#include "Hazard/Rendering/Textures/Texture2D.h"
#include "Hazard/Rendering/Pipeline/FrameBuffer.h"
#include "Hazard/Rendering/Textures/CubemapTexture.h"

#include "Hazard/Rendering/Pipeline/Shader.h"
#include "Hazard/RenderContext/RenderContext.h"

namespace Hazard::Rendering {

	enum AssetType { ShaderAsset = 0, TextureAsset };

	struct RenderUtilsCreateInfo 
	{
		std::string shaderSourcePath;
		std::string shaderCompilePath;
	};

	class RenderUtils {
	public:
		static void Init(RenderUtilsCreateInfo* info);
		static void SetRenderAPI(RenderAPI api) { s_Api = api; }
		static RenderAPI GetRenderAPI() { return s_Api; }

		static std::string GetShaderPath(ShaderFileType type);

		template<typename T, typename Arg>
		static T* CreateRaw(Arg arg);

		template<typename T, typename Arg>
		static Ref<T> Create(Arg arg);

		template<typename T, typename Arg, typename S>
		static Ref<T> Create(Arg arg, S sArg);
		template<typename T, typename Arg, typename S, typename C>
		static Ref<T> Create(Arg arg, S sArg, C tArg);

		template<typename T>
		static Ref<T>& Get();

		static std::vector<RefCount*> GetAssetsOf(AssetType type) {
			return s_Assets[type];
		};
		
	private:

	private:
		static RenderAPI s_Api;
		static Ref<Texture2D> s_WhiteTexture;
		static std::unordered_map<AssetType, std::vector<RefCount*>> s_Assets;
		
		static RenderUtilsCreateInfo m_Info;
	};
}