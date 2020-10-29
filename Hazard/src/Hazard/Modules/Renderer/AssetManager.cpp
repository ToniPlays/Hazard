#pragma once
#include <hzrpch.h>
#include "AssetManager.h"
#include "Hazard/Modules/Renderer/Textures/Texture2D.h"
#include "Hazard/Modules/Renderer/Textures/RenderTexture.h"
#include "RendererAPI.h"


namespace Hazard {

	std::vector<TextureData*> AssetManager::textures;
	std::vector<Shader*> AssetManager::shaders;


	//Get asset
	template<typename T>
	T* AssetManager::GetAsset(const std::string& file)
	{
		HZR_ASSERT(false, "Undefined asset type: " + file);
		return nullptr;
	}
	template<>
	Texture2D* AssetManager::GetAsset(const std::string& file) {

		std::cout << file << std::endl;
		TextureData* texts = GetTextureNode(TextureType::Texture2D);
		for (auto* tex : texts->textures) {
			if (tex->GetPath() == file) return (Texture2D*)tex;
		}
		Texture2D* newTexture = RendererAPI::Create<Texture2D>(file.c_str());
		texts->textures.push_back(newTexture);
		return newTexture;
	}
	template<>
	Shader* AssetManager::GetAsset(const std::string& file) {
		for (Shader* shader : shaders) {
			if (shader->file == file) return shader;
		}
		Shader* shader = RendererAPI::Create<Shader>(file.c_str());
		shaders.push_back(shader);
		return shader;
	}


	template<typename T>
	std::vector<T*> AssetManager::GetAssets()
	{
		HZR_ASSERT(false, "Undefined asset type: " + file);
		return nullptr;
	}
	template<>
	std::vector<TextureData*> AssetManager::GetAssets()
	{
		return textures;
	}
	template<>
	std::vector<Shader*> AssetManager::GetAssets()
	{
		return shaders;
	}

	//ADD ASSET
	template<typename T>
	void AssetManager::AddAsset(T* asset) {
		HZR_ASSERT(false, "Undefined asset type: " + file);
		return nullptr;
	}
	
	template<>
	void AssetManager::AddAsset(Texture2D* texture) {
		TextureData* data = GetTextureNode(TextureType::Texture2D);
		data->textures.push_back(texture);
	}
	template<>
	void AssetManager::AddAsset(RenderTexture* texture) {
		
		TextureData* data = GetTextureNode(TextureType::RenderTexture);
		data->textures.push_back(texture);
	}
	template<>
	void AssetManager::AddAsset(Shader* shader) {
		shaders.push_back(shader);
	}
	
	TextureData* AssetManager::GetTextureNode(TextureType type)
	{
		for (TextureData* data : textures) {
			if (data->type == type) return data;
		}

		TextureData* newData = new TextureData(type);
		textures.push_back(newData);
		return newData;
	}
}