#pragma once

#include <hzrpch.h>
#include "RenderUtils.h"
#include "Pipeline/GraphicsPipeline.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering {

	RenderAPI RenderUtils::s_Api;
	Ref<Texture2D> RenderUtils::s_WhiteTexture;
	std::unordered_map<AssetType, std::vector<RefCount*>> RenderUtils::s_Assets;
	RenderUtilsCreateInfo RenderUtils::m_Info;


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
	template<typename T, typename Arg, typename S>
	Ref<T> RenderUtils::Create(Arg args, S sArgs)
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
		case RenderAPI::Vulkan:		return new Vulkan::VulkanVertexArray(info);
		}
		//HZR_THROW(std::string("Failed to create VertexArray for ")+ RenderContext::APIToString(s_Api));
	}
	template<>
	GraphicsPipeline* RenderUtils::CreateRaw<GraphicsPipeline>(GraphicsPipelineCreateInfo info) {

		switch(s_Api)
		{
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLGraphicsPipeline(info);
		//case RenderAPI::Vulkan:		return new Vulkan::VulkanGraphicsPipeline(info);
		}
	}
	template<>
	Ref<UniformBuffer> RenderUtils::Create<UniformBuffer>(UniformBufferCreateInfo info) {

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLUniformBuffer>::Create(info); break;
		case RenderAPI::Vulkan:		return Ref<Vulkan::VulkanUniformBuffer>::Create(info); break;
		}
	}
	template<>
	Ref<OcclusionQuery> RenderUtils::Create<OcclusionQuery>(const char* dontUse) {

		HZR_THROW("Just don't use me");

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLOcclusionQuery>::Create(); break;
		}
		HZR_THROW("Failed to create OcclusionQuery for ", RenderContext::APIToString(s_Api));
	}
#pragma endregion

#pragma region Textures
	template<>
	Ref<FrameBuffer> RenderUtils::Create<FrameBuffer>(FrameBufferCreateInfo info) {

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLFrameBuffer>::Create(info); break;
		//case RenderAPI::Vulkan:		return Ref<Vulkan::VKRenderTexture>::Create(info); break;
		}
		//HZR_THROW(std::string("Failed to create RenderTexture for ") + RenderContext::APIToString(s_Api));
	}

	template<>
	Ref<Texture2D> RenderUtils::Create<Texture2D>(Texture2DCreateInfo info) {
		HZR_PROFILE_FUNCTION();

		if (Vault::Has<Texture2D>(info.filename)) {
			return Vault::Get<Texture2D>(info.filename);
		}
		Texture2D* texture;
		HZR_CORE_WARN("Loading texture {0}", info.filename);

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLTexture2D(info);	break;
		//case RenderAPI::Vulkan:		texture = new Vulkan::VKTexture2D(info);	break;
		}
		HZR_CORE_ASSERT(texture, "Failed to get texture");
		Vault::Add(info.filename, (RefCount*)texture);
		return Ref(texture);
	}
	template<>
	Ref<CubemapTexture> RenderUtils::Create<CubemapTexture>(CubemapCreateInfo info)
	{
		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLCubemapTexture>::Create(info);
		}
	}
#pragma endregion

	//Initialize white texture for batch rendering
	void RenderUtils::Init(RenderUtilsCreateInfo* info)
	{
		m_Info.shaderCompilePath = info->shaderCompilePath;
		m_Info.shaderSourcePath = info->shaderSourcePath;
		s_Assets = std::unordered_map<AssetType, std::vector<RefCount*>>();
		uint32_t data = 0xFFFFFFFF;

		Texture2DCreateInfo createInfo = {};
		createInfo.filename = "White";
		createInfo.loadFromFile = false;
		createInfo.width = 1;
		createInfo.height = 1;
		createInfo.data = &data;

		s_WhiteTexture = Create<Texture2D>(createInfo);
	}
}