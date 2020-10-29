#pragma once

#include <hzrpch.h>
#include "RendererAPI.h"

#include "Platform/Rendering/OpenGL/Data/OpenGLVertexArray.h"
#include "Platform/Rendering/OpenGL/Data/OpenGLBuffers.h"
#include "Platform/Rendering/OpenGL/Textures/OpenGLTexture.h"
#include "Platform/Rendering/OpenGL/Textures/OpenGLRenderTexture.h"
#include "Platform/Rendering/OpenGL/OpenGLShader.h"
#include "Hazard/Modules/Renderer/AssetManager.h"

namespace Hazard {

	RenderAPI RendererAPI::renderer = RenderAPI::OpenGL;

	template<typename T>
	T* RendererAPI::Create()
	{
		HZR_ASSERT(false, "Unknown Create type");
		return nullptr;
	}
	template<>
	VertexArray* RendererAPI::Create() {
		switch (GetAPI())
		{
			case RenderAPI::OpenGL: return new OpenGLVertexArray();
		}
		return nullptr;
	}

	template<>
	IndexBuffer* RendererAPI::Create() {
		switch (GetAPI())
		{
			case RenderAPI::OpenGL: return new OpenGLIndexBuffer();
		}
		return nullptr;
	}
	template<>
	RenderTexture* RendererAPI::Create() {
		switch (GetAPI())
		{
			case RenderAPI::OpenGL: {
				RenderTexture* texture =  new OpenGLRenderTexture();
				AssetManager::AddAsset<RenderTexture>(texture);
				return texture;
			}
		}
		return nullptr;
	}

	template<typename T>
	T* RendererAPI::Create(const char* file)
	{
		HZR_ASSERT(false, "Unknown Create type");
		return nullptr;
	}

	template<>
	Shader* RendererAPI::Create(const char* file) {
		switch (GetAPI())
		{
		case RenderAPI::OpenGL: {
			Hazard::Shader* shader = AssetManager::GetAsset<Hazard::Shader>(file);
			if (shader != nullptr)
				return shader;
			shader = new OpenGLShader(file);
			AssetManager::AddAsset<Hazard::Shader>(shader);
			return shader;
			}
		}
		return nullptr;
	}
	template<>
	Texture2D* RendererAPI::Create(const char* file) {

		switch (GetAPI())
		{
		case RenderAPI::OpenGL:

			Hazard::Texture2D* texture = AssetManager::GetAsset<Hazard::Texture2D>(file);
			if (texture != nullptr)
				return texture;
			texture = new OpenGLTexture2D(file);
			AssetManager::AddAsset<Hazard::Texture2D>(texture);
			return texture;
		}
		return nullptr;
	}
	VertexBuffer* RendererAPI::VertexBuffer(uint32_t size)
	{
		switch (GetAPI())
		{
			case RenderAPI::OpenGL: return new OpenGLVertexBuffer(size);
		}
		return nullptr;
	}

	int RendererAPI::GetTypeAsInt()
	{
		switch (GetType())
		{
		case Hazard::RenderType::Points:
			return 1;
			break;
		case Hazard::RenderType::Wireframe:
			return 2;
			break;
		default:
			return 0;
			break;
		}
	}

	void RendererAPI::SetType(int t)
	{
		switch (t)
		{
		case 1:
			type = RenderType::Points;
			break;
		case 2:
			type = RenderType::Wireframe;
			break;
		default:
			type = RenderType::Default;
		}
	}
}