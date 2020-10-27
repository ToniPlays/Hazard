#pragma once
#include <hzrpch.h>
#include "AssetManager.h"
#include "Hazard/Modules/Renderer/Textures/Texture2D.h"
#include "Hazard/Modules/Renderer/Textures/RenderTexture.h"


namespace Hazard {

	std::vector<TextureData*> AssetManager::textures;
	std::vector<ShaderData*> AssetManager::shaders;


	//Get asset
	template<typename T>
	T* AssetManager::GetAsset(const std::string& file)
	{
		HZR_ASSERT(false, "Undefined asset type: " + file);
		return nullptr;
	}
	template<>
	Texture2D* AssetManager::GetAsset(const std::string& file) {
		return nullptr;
	}
	template<>
	Shader* AssetManager::GetAsset(const std::string& file) {
		for (ShaderData* data : shaders) {
			for (Shader* shader : data->shaders) {
				if (shader->file == file) return shader;
			}
		}
		return nullptr;
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
	std::vector<ShaderData*> AssetManager::GetAssets()
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
		ShaderData* data = GetShaderNode(ShaderType::Vertex);
		data->shaders.push_back(shader);
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
	ShaderData* AssetManager::GetShaderNode(ShaderType type) {
		for (ShaderData* data : shaders) {
			if (data->type == type) return data;
		}

		ShaderData* newData = new ShaderData(type);
		shaders.push_back(newData);
		return newData;
	}
}