#pragma once

#include <hzrpch.h>
#include "RenderUtils.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering {

	RenderAPI RenderUtils::s_Api;
	Ref<Texture2D> RenderUtils::s_WhiteTexture;
	std::unordered_map<AssetType, std::vector<RefCount*>> RenderUtils::s_Assets;

	template<typename T, typename Arg>
	T* RenderUtils::CreateRaw(Arg args)
	{
		HZR_THROW("Failed to create raw: " + std::string(typeid(T).name()));
	}
	template<typename T, typename Arg>
	Ref<T> RenderUtils::Create(Arg args)
	{
		HZR_THROW("Failed to create: " + std::string(typeid(T).name()));
	}
	template<typename T>
	Ref<T>& RenderUtils::Get() {
		HZR_THROW("Failed to get: " + std::string(typeid(T).name()));
	}
	template<>
	Ref<Texture2D>& RenderUtils::Get() {
		return s_WhiteTexture;
	}
	template<typename T, typename Arg>
	T* RenderUtils::Find(Arg args) {
		HZR_THROW("Failed to find: " + std::string(typeid(T).name()));
	}

#pragma region Rendering

	template<>
	VertexArray* RenderUtils::CreateRaw<VertexArray>(VertexArrayCreateInfo info) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLVertexArray(info);
		case RenderAPI::Vulkan:		return new Vulkan::VKVertexArray(info);
		}
		//HZR_THROW(std::string("Failed to create VertexArray for ")+ RenderContext::APIToString(s_Api));
	}
	template<>
	Ref<Shader> RenderUtils::Create<Shader>(ShaderCreateInfo info) {

		Shader* shader = Find<Shader>(info.filename.c_str());
		if (shader != nullptr) {
			return Ref(shader);
		}

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		shader = new OpenGL::OpenGLShader(info); break;
		case RenderAPI::Vulkan:		shader = new Vulkan::VKShader(info); break;
		}

		s_Assets[AssetType::ShaderAsset].push_back(shader);
		return Ref(shader);
	}
	template<>
	Ref<OcclusionQuery> RenderUtils::Create<OcclusionQuery>(const char* dontUse) {

		HZR_THROW("Just don't use me");

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLOcclusionQuery>::Create(); break;
		}
		HZR_THROW(std::string("Failed to create OcclusionQuery for ") + RenderContext::APIToString(s_Api));
	}
#pragma endregion

#pragma region Textures
	template<>
	Ref<RenderTexture> RenderUtils::Create<RenderTexture>(RenderTextureCreateInfo info) {

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLRenderTexture>::Create(info); break;
		//case RenderAPI::Vulkan:		return Ref<Vulkan::VKRenderTexture>::Create(info); break;
		}
		//HZR_THROW(std::string("Failed to create RenderTexture for ") + RenderContext::APIToString(s_Api));
	}

	template<>
	Ref<Texture2D> RenderUtils::Create<Texture2D>(Texture2DCreateInfo info) {

		Texture* texture = Find<Texture>(info.filename.c_str());
		if (texture != nullptr) {
			return Ref((Texture2D*)texture);
		}

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLTexture2D(info);	break;
		//case RenderAPI::Vulkan:		texture = new Vulkan::VKTexture2D(info);		break;
		}

		s_Assets[AssetType::TextureAsset].push_back(texture);
		return Ref(texture);
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>(CubemapCreateInfo info) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLCubemapTexture>::Create(info);
		}
	}
#pragma endregion

	//Initialize white texture for batch rendering
	void RenderUtils::Init()
	{
		s_Assets = std::unordered_map<AssetType, std::vector<RefCount*>>();
		uint32_t data = 0xFFFFFFFF;

		Texture2DCreateInfo createInfo = {};
		createInfo.width = 1;
		createInfo.height = 1;
		createInfo.data = &data;

		s_WhiteTexture = Create<Texture2D>(createInfo);
	}
}