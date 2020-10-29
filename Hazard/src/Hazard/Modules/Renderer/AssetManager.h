#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Modules/Renderer/Textures/Texture.h"
#include "Hazard/Modules/Renderer/Shaders/Shader.h"

enum class TextureType {
	Texture = 0,
	Texture2D,
	Cubemap,
	RenderTexture
};
enum class ShaderType {
	Vertex = 0,
	Geometry,
	Fragment,
	Compute
};

namespace Hazard {

	struct TextureData {
		TextureType type;
		std::vector<Texture*> textures;

		TextureData(TextureType type) {
			this->type = type;
		}
	};
	struct ShaderData {
		ShaderType type;
		std::vector<Shader*> shaders;

		ShaderData(ShaderType type) {
			this->type = type;
		}
	};

	class HAZARD_API AssetManager {
		public:
			template<typename T>
			static T* GetAsset(const std::string& file);
			template<typename T>
			static void AddAsset(T* asset);

			template<typename T>
			static std::vector<T*> GetAssets();
	private:
		static TextureData* GetTextureNode(TextureType type);
		static std::vector<TextureData*> textures;
		static std::vector<Shader*> shaders;
	};
}