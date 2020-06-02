#pragma once

#include <hzrpch.h>
#include "RendererAPI.h"

#include "Platform/Rendering/OpenGL/Data/OpenGLVertexArray.h"
#include "Platform/Rendering/OpenGL/Data/OpenGLBuffers.h"
#include "Platform/Rendering/OpenGL/Textures/OpenGLTexture.h"
#include "Platform/Rendering/OpenGL/OpenGLShader.h"
#include "Hazard/Modules/Rendering/TextureManager.h"

namespace Hazard {

	RenderAPI RendererAPI::renderer = RenderAPI::OpenGL;

	Shader* RendererAPI::Shader(const std::string& file)
	{
		switch (GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGLShader(file);
		}
		return nullptr;
	}

	VertexArray* RendererAPI::VertexArray()
	{
		switch (GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGLVertexArray();
		}
		return nullptr;
	}

	VertexBuffer* RendererAPI::VertexBuffer(ShaderDataType type, const std::string& name)
	{
		switch (GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGLVertexBuffer(type, name);
		}
		return nullptr;
	}

	Hazard::IndexBuffer* RendererAPI::IndexBuffer()
	{
		switch (GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGLIndexBuffer();
		}
		return nullptr;
	}

	Texture2D* RendererAPI::Texture2D(const std::string& file)
	{
		switch (GetAPI())
		{
		case RenderAPI::OpenGL: 

			Hazard::Texture2D* texture = (Hazard::Texture2D*)TextureManager::GetTexture(T2D, file);

			if (texture != nullptr) 
				return texture;

			texture = new OpenGLTexture2D(file);
			TextureManager::AddTexture(T2D, texture);
			return texture;
		}
		return nullptr;
	}

	void RendererAPI::GetType(int &selected)
	{
		switch (type)
		{
		case Hazard::RenderType::Points:
			selected = 1;
			break;
		default:
			selected = 0;
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
		default:
			type = RenderType::Default;
		}
	}
}