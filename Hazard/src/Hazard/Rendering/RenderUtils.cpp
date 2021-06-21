#pragma once

#include <hzrpch.h>
#include "RenderUtils.h"
#include "Pipeline/GraphicsPipeline.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering {

	RenderAPI RenderUtils::s_Api;
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
	Ref<GraphicsPipeline> RenderUtils::Create<GraphicsPipeline>(GraphicsPipelineCreateInfo info) {

		if (Vault::Has<GraphicsPipeline>(info.shaderPipelineName.c_str())) {
			return Ref(Vault::Get<GraphicsPipeline>(info.shaderPipelineName.c_str()));
		}

		GraphicsPipeline* pipeline = nullptr;

		switch(s_Api)
		{
		case RenderAPI::OpenGL:		pipeline = new OpenGL::OpenGLGraphicsPipeline(info);
		//case RenderAPI::Vulkan:		return new Vulkan::VulkanGraphicsPipeline(info);
		}
		Vault::Add(info.shaderPipelineName, pipeline);
		return pipeline;
	}
	template<>
	Ref<Shader> RenderUtils::Create<Shader>(const char* name, uint32_t stageCount, PipelineShaderStage* stages) {

		if (Vault::Has<Shader>(name)) {
			return Ref(Vault::Get<Shader>(name));
		}

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		return Ref<OpenGL::OpenGLShader>::Create(name, stageCount, stages);
		//case RenderAPI::Vulkan:		return Ref<Vulkan::VulkanShader>::Create(name, stageCount, stages); break;
		}
		HZR_THROW("Failed to find shader");
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

		if (Vault::Has<Texture2D>(info.filename.c_str())) {
			return Vault::Get<Texture2D>(info.filename.c_str());
		}

		Texture2D* texture = nullptr;
		HZR_CORE_WARN("Loading texture {0}", info.filename);

		switch (s_Api)
		{
		case RenderAPI::OpenGL:		texture = new OpenGL::OpenGLTexture2D(info);	break;
		//case RenderAPI::Vulkan:		texture = new Vulkan::VKTexture2D(info);	break;
		}
		HZR_CORE_ASSERT(texture, "Failed to get texture");
		Vault::Add(info.filename, texture);
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

		Ref<Texture2D> texture = Create<Texture2D>(createInfo);
		texture->IncRefCount();
		Vault::Add(createInfo.filename, texture.Raw());

		data = 0xFFFF0000;

		Texture2DCreateInfo normalCreateInfo = {};
		normalCreateInfo.filename = "DefaultNormalMap";
		normalCreateInfo.loadFromFile = false;
		normalCreateInfo.width = 1;
		normalCreateInfo.height = 1;
		normalCreateInfo.data = &data;

		Ref<Texture2D> normalTexture = Create<Texture2D>(normalCreateInfo);
		normalTexture->IncRefCount();
		Vault::Add(normalCreateInfo.filename, normalTexture.Raw());


		InputAssembly assembly = {};
		assembly.topology = InputTopology::TriangleList;

		PipelineRasterizer rasterizer;
		rasterizer.cullFace = CullFace::BackFace;
		rasterizer.depthFunc = DepthFunc::Less;

		PipelineViewport viewport = {};
		viewport.offset = { 0, 0 };
		viewport.size = { 1920, 1080 };

		PipelineShaderStage stages[2];

		stages[0].shaderFileName = "pbr_vert.glsl";
		stages[0].stage = ShaderType::VertexShader;
		stages[0].fileType = ShaderFileType::Source;

		stages[1].shaderFileName = "pbr_frag.glsl";
		stages[1].stage = ShaderType::FragmentShader;
		stages[1].fileType = ShaderFileType::Source;


		GraphicsPipelineCreateInfo pipelineInfo;
		pipelineInfo.shaderPipelineName = "DefaultMeshShader";
		pipelineInfo.inputAssembly = &assembly;
		pipelineInfo.rasterizer = &rasterizer;
		pipelineInfo.viewport = &viewport;
		pipelineInfo.stageCount = 2;
		pipelineInfo.stages = stages;
		Ref<GraphicsPipeline> pipeline = Create<GraphicsPipeline>(pipelineInfo);


		//Handle somehow, causes memoryleak possibly
		Material* mat = new Material();
		mat->SetPipeline(pipeline);
	}
	std::string RenderUtils::GetShaderPath(ShaderFileType type)
	{
		if (type == ShaderFileType::Binary) 
			return m_Info.shaderCompilePath;
		return m_Info.shaderSourcePath;
	}
}