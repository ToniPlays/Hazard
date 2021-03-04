#pragma once

#include <hzrpch.h>
#include "RenderUtils.h"

#include "Platform/Rendering/OpenGL/Texture/OpenGLTexture2D.h"
#include "Platform/Rendering/OpenGL/Texture/OpenGLRenderTexture.h"
#include "Platform/Rendering/OpenGL/Buffer/OpenGLVertexArray.h"
#include "Platform/Rendering/OpenGL/Buffer/OpenGLBuffer.h"
#include "Platform/Rendering/OpenGL/Shader/OpenGLShader.h"

namespace Hazard::Rendering {

	RenderAPI RenderUtils::api;
	std::vector<Texture*> RenderUtils::textures = std::vector<Texture*>();

	template<typename T>
	T* RenderUtils::Create()
	{
		static_assert(false);
	}
	template<class T, typename Arg>
	T* RenderUtils::Create(Arg args)
	{
		static_assert(false);
	}
	template<class T, typename Arg, typename C>
	T* RenderUtils::Create(Arg args, C name)
	{
		static_assert(false);
	}
	template<typename T, typename Arg>
	T* RenderUtils::Find(Arg args) {
		static_assert(false);
	}

#pragma region Find textures 

	template<>
	Texture* RenderUtils::Find(uint32_t textureID) {
		for (Texture* texture : textures) {
			if (texture->GetID() == textureID) 
				return texture;
		}
		return nullptr;
	}
	template<>
	Texture* RenderUtils::Find(const char* file) {
		for (Texture* texture : textures) {
			if (std::string(texture->GetFile()) == std::string(file)) 
				return texture;
		}
		return nullptr;
	}

#pragma endregion

#pragma region Rendering

	template<>
	VertexArray* RenderUtils::Create<VertexArray>() {
		switch (api)
		{
		case RenderAPI::OpenGL:		return new OpenGLVertexArray();
		}
		return nullptr;
	}
	template<>
	VertexBuffer* RenderUtils::Create<VertexBuffer>(uint32_t size) {
		switch (api)
		{
		case RenderAPI::OpenGL:		return new OpenGLVertexBuffer(size);
		}
		return nullptr;
	}
	template<>
	IndexBuffer* RenderUtils::Create<IndexBuffer>() {
		switch (api)
		{
		case RenderAPI::OpenGL:		return new OpenGLIndexBuffer();
		}
		return nullptr;
	}
	template<>
	Shader* RenderUtils::Create<Shader>(const char* file) {
		switch (api)
		{
		case RenderAPI::OpenGL:		return new OpenGLShader(file);
		}
		return nullptr;
	}
#pragma endregion

#pragma region Textures
	template<>
	RenderTexture* RenderUtils::Create<RenderTexture>() {
		switch (api)
		{
		case RenderAPI::OpenGL:		return new OpenGLRenderTexture();
		}
		return nullptr;
	}

	template<>
	Texture2D* RenderUtils::Create<Texture2D>(glm::vec2 size, const char* name) {

		Texture2D* texture = (Texture2D*)Find<Texture>(name);

		if (texture != nullptr)
			return texture;

		switch (api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGLTexture2D(size, name); break;
		}
		textures.push_back(texture);
		return texture;
	}
	template<>
	Texture2D* RenderUtils::Create<Texture2D>(const char* path) {

		Texture2D* texture = (Texture2D*)Find<Texture>(path);

		if (texture != nullptr) 
			return texture;

		switch (api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGLTexture2D(path); break;
		}
		textures.push_back(texture);
		return texture;
	}
#pragma endregion

	//Initialize white texture for batch rendering
	void RenderUtils::Init()
	{
		Texture2D* texture = Create<Texture2D>(glm::vec2{ 1, 1 }, "White");
		uint32_t data = 0xFFFFFFFF;
		texture->SetData(&data, sizeof(uint32_t));
	}
	Texture* RenderUtils::GetTexture(uint32_t textureID)
	{
		return Find<Texture>(textureID);
	}
	Texture* RenderUtils::GetFromTextures(uint32_t index) {
		if (index > textures.size() - 1) 
			index = 0;
		return textures[index];
	}
	uint32_t RenderUtils::GetTextureStackIndex(Texture* texture) {
		for (int i = 0; i < textures.size(); i++) {
			if (textures[i] == texture) 
				return i;
		}

		return 0;
	}
}