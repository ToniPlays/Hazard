#pragma once

#include "Hazard/Rendering/Textures/Texture2D.h"
#include "Hazard/Rendering/Textures/RenderTexture.h"
#include "Hazard/Rendering/Textures/CubemapTexture.h"
#include "Hazard/Rendering/Textures/EnvironmentMap.h"

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

		template<typename T, typename Arg>
		static T* CreateRaw(Arg arg);

		template<typename T, typename Arg>
		static Ref<T> Create(Arg arg);

		template<typename T>
		static Ref<T>& Get();

		template<typename T>
		static void RemoveAsset(T* asset) {
			static_assert(false);
		};

		template<>
		static void RemoveAsset(Shader* shader) {
			
			std::vector<RefCount*> refs = s_Assets[AssetType::ShaderAsset];
			auto it = std::find(refs.begin(), refs.end(), shader);

			HZR_CORE_INFO(refs.size());
			if (it != refs.end()) {
				refs.erase(it);
			}
			HZR_CORE_INFO(refs.size());
			s_Assets[AssetType::ShaderAsset] = refs;
		}

		template<>
		static void RemoveAsset(Texture* texture) {

			std::vector<RefCount*> refs = s_Assets[AssetType::TextureAsset];
			auto it = std::find(refs.begin(), refs.end(), texture);

			HZR_CORE_INFO(refs.size());
			if (it != refs.end()) {
				HZR_CORE_ERROR("Removed {0}", texture->GetFile());
				refs.erase(it);
			}
			HZR_CORE_INFO(refs.size());
			s_Assets[AssetType::TextureAsset] = refs;
		}

		static std::vector<RefCount*> GetAssetsOf(AssetType type) {
			return s_Assets[type];
		};
		
	private:

		template<typename T, typename Arg>
		static T* Find(Arg args);

		template<>
		static Texture* Find(const char* file) {
			std::vector<RefCount*> textures = s_Assets[AssetType::TextureAsset];

			for (RefCount* ref : textures) {
				Texture* texture = (Texture*)ref;
				if (strcmp(texture->GetFile().c_str(), file) == 0)
					return texture;
			}
			return nullptr;
		}

	private:
		static RenderAPI s_Api;
		static Ref<Texture2D> s_WhiteTexture;
		static std::unordered_map<AssetType, std::vector<RefCount*>> s_Assets;
		
		static RenderUtilsCreateInfo m_Info;
	};
}