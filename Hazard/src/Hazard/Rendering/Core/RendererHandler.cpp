#pragma once

#include <hzrpch.h>
#include "RendererHandler.h"


namespace Hazard {

	Renderer RendererHandler::renderer;

	void RendererHandler::Init(Renderer renderer)
	{
		RendererHandler::renderer = renderer;
	}
	template<typename T, typename... Args>
	T* RendererHandler::Create(Args... args) {
		static_assert(false);
	}

	template<>
	VertexArray* RendererHandler::Create() {
		switch (renderer)
		{
		case Renderer::OpenGL:		return new OpenGLVertexArray();
		}
		return nullptr;
	}
	template<>
	IndexBuffer* RendererHandler::Create() {
		switch (renderer)
		{
		case Renderer::OpenGL:		return new OpenGLIndexBuffer();
		}
		return nullptr;
	}

	template<>
	RenderTexture* RendererHandler::Create() {
		switch (renderer)
		{
		case Renderer::OpenGL:		return new OpenGLRenderTexture();
		}
		return nullptr;
	}
	
	template<>
	VertexBuffer* RendererHandler::Create(int size) {
		switch (renderer)
		{
		case Renderer::OpenGL:		return new OpenGLVertexBuffer((uint32_t)size);
		}
		return nullptr;
	}
	template<>
	Shader* RendererHandler::Create(const char* file) {
		switch (renderer)
		{
		case Renderer::OpenGL:		return new OpenGLShader(file);
		}
		return nullptr;
	}
}