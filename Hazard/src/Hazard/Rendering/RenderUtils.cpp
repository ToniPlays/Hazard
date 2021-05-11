#pragma once

#include <hzrpch.h>
#include "RenderUtils.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"

namespace Hazard::Rendering {

	RenderAPI RenderUtils::s_Api;

	template<typename T>
	T* RenderUtils::CreateRaw()
	{
		static_assert(false, "RenderUtils::CreateRaw()");
	}
	template<typename T, typename Arg>
	T* RenderUtils::CreateRaw(Arg args)
	{
		static_assert(false, "RenderUtils::CreateRaw(Arg)");
	}

	template<typename T>
	Ref<T> RenderUtils::Create()
	{
		static_assert(false, "RenderUtils::Create()");
	}
	template<typename T, typename Arg>
	Ref<T> RenderUtils::Create(Arg args)
	{
		static_assert(false, "RenderUtils::Create(Arg)");
	}
	template<typename T, typename F, typename Arg>
	Ref<T> RenderUtils::Create(F file, Arg name)
	{
		static_assert(false, "RenderUtils::Create(F, Arg)");
	}

#pragma region Rendering

	template<>
	VertexArray* RenderUtils::CreateRaw<VertexArray>() {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLVertexArray();
		}
		return nullptr;
	}
	template<>
	VertexBuffer* RenderUtils::CreateRaw<VertexBuffer>(uint32_t size) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLVertexBuffer(size);
		}
		return nullptr;
	}
	template<>
	IndexBuffer* RenderUtils::CreateRaw<IndexBuffer>() {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLIndexBuffer();
		}
		return nullptr;
	}
	template<>
	Ref<Shader> RenderUtils::Create<Shader>(const char* file) {

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLShader>::Create(file);
		}
	}
	template<>
	Ref<OcclusionQuery> RenderUtils::Create<OcclusionQuery>() {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLOcclusionQuery>::Create();
		}
		return nullptr;
	}
#pragma endregion

#pragma region Textures
	template<>
	Ref<RenderTexture> RenderUtils::Create<RenderTexture>(TextureSpecs specs) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLRenderTexture>::Create(specs);
		}
	}

	template<>
	Ref<Texture2D> RenderUtils::Create<Texture2D>(const char* path) {

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLTexture2D>::Create(path, TextureSpecs());
		}
	}
	template<>
	Ref<Texture2D> RenderUtils::Create<Texture2D>(const char* name, TextureSpecs params) {

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLTexture2D>::Create(name, params);
		}
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>() {


		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLCubemapTexture>::Create(TextureSpecs());
		}
		return nullptr;
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>(TextureSpecs specs) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLCubemapTexture>::Create(specs);
		}
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>(std::vector<std::string> faces) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLCubemapTexture>::Create(faces);
		}
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>(const char* name, const char* extension) {

		HZR_CORE_INFO("Loading cubemap {0}, {1}", name, extension);

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLCubemapTexture>::Create(name, extension);
		}
	}
	template<>
	Ref<EnvinronmentMap> RenderUtils::Create<EnvinronmentMap>(const char* file) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLEnvironmentMap>::Create(file, TextureSpecs());
		}
	}
	template<>
	Ref<EnvinronmentMap> RenderUtils::Create<EnvinronmentMap>(const char* file, TextureSpecs specs) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLEnvironmentMap>::Create(file, specs);
		}
	}
#pragma endregion

	//Initialize white texture for batch rendering
	void RenderUtils::Init()
	{
		TextureSpecs params;
		params.width = 1;
		params.height = 1;
		params.diskImage = false;

		Ref<Texture2D> texture = Create<Texture2D>("White", params);
		uint32_t data = 0xFFFFFFFF;
		texture->SetData(&data, sizeof(uint32_t));


	}
}