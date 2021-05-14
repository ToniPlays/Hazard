#pragma once

#include <hzrpch.h>
#include "RenderUtils.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering {

	RenderAPI RenderUtils::s_Api;
	Ref<Texture2D> RenderUtils::s_WhiteTexture;
	std::unordered_map<AssetType, std::vector<RefCount*>> RenderUtils::s_Assets;

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
	template<typename T>
	Ref<T>& RenderUtils::Get() {
		static_assert(false, "RenderUtils::Get()");
	}
	template<>
	Ref<Texture2D>& RenderUtils::Get() {
		return s_WhiteTexture;
	}
#pragma region Rendering

	template<>
	VertexArray* RenderUtils::CreateRaw<VertexArray>() {

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLVertexArray();
		case RenderAPI::Vulkan:		return new Vulkan::VKVertexArray();
		}
		HZR_THROW(std::string("Failed to create VertexArray for ")+ RenderContext::APIToString(s_Api));
	}
	template<>
	VertexBuffer* RenderUtils::CreateRaw<VertexBuffer>(uint32_t size) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLVertexBuffer(size);
		case RenderAPI::Vulkan:		return new Vulkan::VKVertexBuffer(size);
		default:
			HZR_THROW(std::string("Failed to create VertexBuffer for ") + RenderContext::APIToString(s_Api));
		}
	}
	template<>
	IndexBuffer* RenderUtils::CreateRaw<IndexBuffer>() {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLIndexBuffer();
		case RenderAPI::Vulkan:		return new Vulkan::VKIndexBuffer();
		default:
			HZR_THROW(std::string("Failed to create IndexBuffer for ") + RenderContext::APIToString(s_Api));
		}
		
	}
	template<>
	Ref<Shader> RenderUtils::Create<Shader>(const char* file) {

		Shader* shader = Find<Shader>(file);
		if (shader != nullptr) {
			HZR_CORE_INFO("Shader {0} refs {1}", file, shader->GetRefCount());
			return Ref(shader);
		}

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		shader = new OpenGL::OpenGLShader(file); break;
		case RenderAPI::Vulkan:		shader = new Vulkan::VKShader(file); break;
		default:
			HZR_THROW(std::string("Failed to create Shader for ") + RenderContext::APIToString(s_Api));
		}
		Ref ref(shader);
		HZR_CORE_INFO("Created new shader {0} refs {1} ", file, ref->GetRefCount());
		s_Assets[AssetType::ShaderAsset].push_back(shader);
		return ref;
	}
	template<>
	Ref<OcclusionQuery> RenderUtils::Create<OcclusionQuery>() {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLOcclusionQuery>::Create(); break;
		default:
			HZR_THROW(std::string("Failed to create OcclusionQuery for ") + RenderContext::APIToString(s_Api));
		}
	}
#pragma endregion

#pragma region Textures
	template<>
	Ref<RenderTexture> RenderUtils::Create<RenderTexture>(TextureSpecs specs) {

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLRenderTexture>::Create(specs); break;
		case RenderAPI::Vulkan:		return Ref<Vulkan::VKRenderTexture>::Create(specs); break;
		default:
			HZR_THROW(std::string("Failed to create RenderTexture for ") + RenderContext::APIToString(s_Api));
		}
	}

	template<>
	Ref<Texture2D> RenderUtils::Create<Texture2D>(const char* path) {

		Texture* texture = Find<Texture>(path);
		if (texture != nullptr) {
			return Ref((Texture2D*)texture);
		}

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLTexture2D(path, TextureSpecs());	break;
		case RenderAPI::Vulkan:		texture = new Vulkan::VKTexture2D(path, TextureSpecs());		break;
		default:
			HZR_THROW(std::string("Failed to create Texture2D for ") + RenderContext::APIToString(s_Api));
		}

		HZR_CORE_INFO("Created new Texture2D {0}", path);
		s_Assets[AssetType::TextureAsset].push_back(texture);
		return Ref(texture);
	}
	template<>
	Ref<Texture2D> RenderUtils::Create<Texture2D>(const char* name, TextureSpecs params) {

		Texture* texture = Find<Texture>(name);
		if (texture != nullptr) {
			return Ref((Texture2D*)texture);
		}

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLTexture2D(name, params);		break;
		case RenderAPI::Vulkan:		texture = new Vulkan::VKTexture2D(name, TextureSpecs());	break;
		default:
			HZR_THROW(std::string("Failed to create Texture2D for ") + RenderContext::APIToString(s_Api));
		}

		HZR_CORE_INFO("Created new Texture2D {0}", name);
		s_Assets[AssetType::TextureAsset].push_back(texture);

		return Ref(texture);
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>() {

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLCubemapTexture>::Create(TextureSpecs());
		default:
			HZR_THROW(std::string("Failed to create CubemapTexture for ") + RenderContext::APIToString(s_Api));
		}
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>(TextureSpecs specs) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLCubemapTexture>::Create(specs);
		default:
			HZR_THROW(std::string("Failed to create CubemapTexture for ") + RenderContext::APIToString(s_Api));
		}
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>(std::vector<std::string> faces) {
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLCubemapTexture>::Create(faces);
		default:
			HZR_THROW(std::string("Failed to create CubemapTexture for ") + RenderContext::APIToString(s_Api));
		}
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>(const char* name, const char* extension) {

		Ref<Texture> ref = Find<Texture>(name);
		if (ref) {
			return static_cast<Ref<CubemapTexture>>(ref);
		}

		CubemapTexture* texture;
		HZR_CORE_INFO("Loading cubemap {0}, {1}", name, extension);

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLCubemapTexture(name, extension);  break;
		default:
			HZR_THROW(std::string("Failed to create CubemapTexture for ") + RenderContext::APIToString(s_Api));
		}

		HZR_CORE_INFO("Created new Texture2D {0}", name);
		s_Assets[AssetType::TextureAsset].push_back(texture);

		return ref;
	}
	template<>
	Ref<EnvinronmentMap> RenderUtils::Create<EnvinronmentMap>(const char* file) {

		Ref<Texture> ref = Find<Texture>(file);
		if (ref) {
			return static_cast<Ref<EnvinronmentMap>>(ref);
		}
		EnvinronmentMap* texture;
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLEnvironmentMap(file, TextureSpecs());  break;
		default:
			HZR_THROW(std::string("Failed to create EnvinronmentMap for ") + RenderContext::APIToString(s_Api));
		}

		HZR_CORE_INFO("Created new Texture2D {0}", file);
		s_Assets[AssetType::TextureAsset].push_back(texture);

		return Ref(texture);
	}
	template<>
	Ref<EnvinronmentMap> RenderUtils::Create<EnvinronmentMap>(const char* file, TextureSpecs specs) {

		Ref<Texture> ref = Find<Texture>(file);
		if (ref) {
			return static_cast<Ref<EnvinronmentMap>>(ref);
		}

		EnvinronmentMap* texture;
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLEnvironmentMap(file, specs);  break;
		default:
			HZR_THROW(std::string("Failed to create EnvinronmentMap for ") + RenderContext::APIToString(s_Api));
		}

		HZR_CORE_INFO("Created new Texture2D {0}", file);
		s_Assets[AssetType::TextureAsset].push_back(texture);

		return Ref(texture);

	}
#pragma endregion

	//Initialize white texture for batch rendering
	void RenderUtils::Init()
	{
		s_Assets = std::unordered_map<AssetType, std::vector<RefCount*>>();

		TextureSpecs params;
		params.width = 1;
		params.height = 1;
		params.diskImage = false;

		s_WhiteTexture = Create<Texture2D>("White", params);
		uint32_t data = 0xFFFFFFFF;
		s_WhiteTexture->SetData(&data, sizeof(uint32_t));
	}
	template<typename T, typename Arg>
	T* RenderUtils::Find(Arg args) {
		static_assert(false, "RenderUtils::Find(Args)");
	}
	
}