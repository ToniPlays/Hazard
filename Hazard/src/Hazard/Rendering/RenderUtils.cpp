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
	Texture2D* RenderUtils::whiteTexture;

	
	template<class T>
	T* RenderUtils::Create()
	{

	}
	template<class T, typename Arg>
	T* RenderUtils::Create(Arg args)
	{

	}
	template<>
	RenderTexture* RenderUtils::Create<RenderTexture>() {
		switch (api)
		{
		case RenderAPI::OpenGL:		return new OpenGLRenderTexture();
		}
		return nullptr;
	}
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
	template<>
	Texture2D* RenderUtils::Create<Texture2D>(glm::vec2 size) {
		switch (api)
		{
		case RenderAPI::OpenGL:		return new OpenGLTexture2D(size);
		}
		return nullptr;
	}
	template<>
	Texture2D* RenderUtils::Create<Texture2D>(const char* path) {
		switch (api)
		{
		case RenderAPI::OpenGL:		return new OpenGLTexture2D(path);
		}
		return nullptr;
	}

	//Initialize white texture for batch rendering
	void RenderUtils::Init()
	{
		whiteTexture = Create<Texture2D>(glm::vec2{ 1, 1 });
		uint32_t data = 0xFFFFFFFF;
		whiteTexture->SetData(&data, sizeof(uint32_t));
		HZR_CORE_INFO("White texture id {0}", whiteTexture->GetID());
	}
}