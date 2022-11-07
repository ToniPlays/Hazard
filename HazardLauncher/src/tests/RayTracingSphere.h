
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "Backend/Core/AccelerationStructure/AccelerationStructure.h"

#include "Hazard/Math/Time.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <Hazard/Rendering/Mesh/Mesh.h>
#include <Hazard/Rendering/Mesh/MeshFactory.h>
#include <Hazard.h>
#include <src/tests/EditorCamera.h>
#include "Hazard/RenderContext/TextureFactory.h"
#include "Hazard/RenderContext/ShaderLibrary.h"

using namespace HazardRenderer;
using namespace Hazard;

namespace RayTracingSphere
{
	//OpenGL : Test
	//Vulkan : Test
	//Metal	 : Test
	//DX12	 : Test
	//DX11	 : Test

	static void Run(RenderAPI api)
	{
#ifdef HZR_PLATFORM_MACOS
		std::filesystem::current_path("/users/ToniSimoska/Hazard/HazardLauncher");
#endif
		static bool running = true;

		Editor::EditorCamera camera(45.0f, 1.0f, 0.03f, 1000.0f);
		camera.SetSpeedMultiplier(0.25f);
#pragma region Init
		Logging::Logger logger = Logging::Logger();

		HazardRendererAppInfo rendererApp = {};
		rendererApp.AppName = "Hello RayTracing";
		rendererApp.BuildVersion = "0.0.1a";
		rendererApp.EventCallback = [&](Event& e) {
			camera.OnEvent(e);
			if (e.GetEventType() == EventType::WindowClose)
			{
				running = false;
			}
			if (e.GetEventType() == EventType::WindowResize)
			{
				auto& resize = (WindowResizeEvent&)e;
				camera.SetViewport(resize.GetWidth(), resize.GetHeight());
			}
		};

		rendererApp.MessageCallback = [](RenderMessage message)
		{
			std::cout << message.Description << std::endl;
			std::cout << message.StackTrace << std::endl;
		};

		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = rendererApp.AppName;
		windowInfo.Maximized = false;
		windowInfo.FullScreen = false;
		windowInfo.Width = 1920;
		windowInfo.Height = 1080;
		windowInfo.Color = Color(32, 32, 32, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &rendererApp;
		renderInfo.Renderer = api;
		renderInfo.Logging = true;
		renderInfo.VSync = false;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;
#pragma endregion
		Window* window = Window::Create(&renderInfo);
		window->Show();

		auto& device = window->GetContext()->GetDevice();

		std::cout << "Selected device: " << device->GetDeviceName() << std::endl;
		HZR_ASSERT(device->SupportsRaytracing(), "Device does not support Raytracing");

#pragma region Mesh
		MeshFactory factory = {};
		factory.SetOptimization(MeshLoaderFlags_DefaultFlags);
		MeshData meshData = factory.LoadMeshFromSource("src/tests/Meshes/c8_corvette_colored.fbx");

		MeshCreateInfo meshInfo = {};
		meshInfo.Usage = BufferUsage::BLAS;
		meshInfo.BoundingBox = meshData.BoundingBox;
		meshInfo.VertexCount = meshData.Vertices.size() * sizeof(Vertex3D);
		meshInfo.pVertices = meshData.Vertices.data();
		meshInfo.IndexCount = meshData.Indices.size() * sizeof(uint32_t);
		meshInfo.pIndices = meshData.Indices.data();

		Ref<Mesh> mesh = Ref<Mesh>::Create(&meshInfo);
#pragma endregion
#pragma region RayTracing
		Image2DCreateInfo outputImageSpec = {};
		outputImageSpec.DebugName = "OutputImage";
		outputImageSpec.Width = window->GetWidth();
		outputImageSpec.Height = window->GetHeight();
		outputImageSpec.Format = ImageFormat::RGBA;
		outputImageSpec.Usage = ImageUsage::Storage;

		Ref<Image2D> outputImage = Image2D::Create(&outputImageSpec);

		AccelerationStructureCreateInfo bottomAccelInfo = {};
		bottomAccelInfo.DebugName = "BottomLevelAccelerationStructure";
		bottomAccelInfo.Level = AccelerationStructureLevel::Bottom;
		bottomAccelInfo.VertexBuffer = mesh->GetVertexBuffer();
		bottomAccelInfo.IndexBuffer = mesh->GetIndexBuffer();
		bottomAccelInfo.BoundingBox = mesh->GetBoundingBox();

		Ref<BottomLevelAS> bottomLevelAccelerationStructure = BottomLevelAS::Create(&bottomAccelInfo);

		glm::mat4 transform = Math::ToTransformMatrix({ 0, 0, -5.0f }, glm::quat({ 0.0f, glm::radians(60.0f), 0.0f }));

		AccelerationStructureCreateInfo topAccelInfo = {};
		topAccelInfo.DebugName = "TopLevelAccelerationStructure";
		topAccelInfo.Level = AccelerationStructureLevel::Top;
		topAccelInfo.BottomLevelASCount = 1;
		topAccelInfo.pBottomLevelAS = bottomLevelAccelerationStructure.Raw();

		Ref<TopLevelAS> topLevelAccelerationStructure = TopLevelAS::Create(&topAccelInfo);

		std::vector<ShaderStageCode> shaderCode = ShaderCompiler::GetShaderBinariesFromSource("src/tests/Shaders/raygen.glsl", api);
		std::vector<ShaderStageCode> screenPassCode = ShaderCompiler::GetShaderBinariesFromSource("src/tests/Shaders/composite.glsl", api);

		PipelineSpecification pipelineSpec = {};
		pipelineSpec.DebugName = "RaygenPipeline";
		pipelineSpec.Usage = PipelineUsage::Raygen;
		pipelineSpec.ShaderCodeCount = shaderCode.size();
		pipelineSpec.pShaderCode = shaderCode.data();
		pipelineSpec.MaxRayDepth = 2;

		Ref<Pipeline> raygenPipeline = Pipeline::Create(&pipelineSpec);

		ShaderBindingTableCreateInfo bindingSpec = {};
		bindingSpec.pPipeline = raygenPipeline;

		Ref<ShaderBindingTable> bindingTable = ShaderBindingTable::Create(&bindingSpec);
#pragma endregion
#pragma region ScreenPass
		PipelineSpecification screenPassSpec = {};
		screenPassSpec.DebugName = "ScreenPass";
		screenPassSpec.Usage = PipelineUsage::GraphicsBit;
		screenPassSpec.CullMode = CullMode::None;
		screenPassSpec.DepthTest = false;
		screenPassSpec.ShaderCodeCount = screenPassCode.size();
		screenPassSpec.pShaderCode = screenPassCode.data();
		screenPassSpec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass();

		Ref<Pipeline> screenPass = Pipeline::Create(&screenPassSpec);
#pragma endregion

		struct CameraData
		{
			glm::mat4 InvProjection;
			glm::mat4 InvView;
		};

		UniformBufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Set = 0;
		uboInfo.Binding = 2;
		uboInfo.Size = sizeof(CameraData);
		uboInfo.Usage = BufferUsage::DynamicDraw;

		Ref<UniformBuffer> cameraUBO = UniformBuffer::Create(&uboInfo);
#pragma region EnvironmentMap

		TextureHeader& header = TextureFactory::LoadTextureFromSourceFile("src/tests/Textures/pink_sunrise_4k.hdr", true);

		Image2DCreateInfo envInfo = {};
		envInfo.DebugName = "EnvMap";
		envInfo.Format = ImageFormat::RGBA;
		envInfo.Usage = ImageUsage::Texture;
		envInfo.Width = header.Width;
		envInfo.Height = header.Height;
		envInfo.Data = header.ImageData;

		Ref<Image2D> environmentMapImage = Image2D::Create(&envInfo);

		Ref<RenderCommandBuffer> computeBuffer = RenderCommandBuffer::Create("RadianceMap compute", 1, true);
		Ref<RenderCommandBuffer> graphicsBuffer = RenderCommandBuffer::Create("Transition", 1);

		CubemapTextureCreateInfo radianceInfo = {};
		radianceInfo.DebugName = "RadianceMap " + environmentMapImage->GetDebugName();
		radianceInfo.Usage = ImageUsage::Texture;
		radianceInfo.Format = ImageFormat::RGBA;
		radianceInfo.Width = 4096;
		radianceInfo.Height = 4096;

		Ref<CubemapTexture> radianceMap = CubemapTexture::Create(&radianceInfo);

		ImageTransitionInfo generalTransition = {};
		generalTransition.Cubemap = radianceMap;
		generalTransition.SourceLayout = ImageLayout_ShaderReadOnly;
		generalTransition.DestLayout = ImageLayout_General;

		std::vector<ShaderStageCode> computeCode = ShaderCompiler::GetShaderBinariesFromSource("src/tests/Shaders/EquirectangularToCubeMap.glsl", api);

		PipelineSpecification computeSpec = {};
		computeSpec.DebugName = "EquirectangularToCubemap";
		computeSpec.Usage = PipelineUsage::ComputeBit;
		computeSpec.ShaderCodeCount = computeCode.size();
		computeSpec.pShaderCode = computeCode.data();

		Ref<Pipeline> computePipeline = Pipeline::Create(&computeSpec);

		auto& shader = computePipeline->GetShader();
		shader->Set("o_CubeMap", 0, radianceMap);
		shader->Set("u_EquirectangularTexture", 0, environmentMapImage);

		DispatchComputeInfo computeInfo = {};
		computeInfo.GroupSize = { radianceInfo.Width / 32, radianceInfo.Height / 32, 6 };
		computeInfo.Pipeline = computePipeline;
		computeInfo.WaitForCompletion = true;

		computeBuffer->Begin();
		computeBuffer->DispatchCompute(computeInfo);
		computeBuffer->End();
		computeBuffer->Submit();

#pragma endregion

		while (running)
		{
			Time::Update(glfwGetTime());
			Input::Update();

			camera.OnUpdate();

			window->BeginFrame();

			auto& commandBuffer = window->GetSwapchain()->GetSwapchainBuffer();

			CameraData cameraData = {};
			cameraData.InvProjection = glm::inverse(camera.GetProjection());
			cameraData.InvView = glm::inverse(camera.GetView());

			BufferCopyRegion copyRegion = {};
			copyRegion.Data = &cameraData;
			copyRegion.Size = sizeof(CameraData);

			cameraUBO->SetData(copyRegion);
			commandBuffer->BindUniformBuffer(cameraUBO);

			raygenPipeline->GetShader()->Set("TLAS", 0, topLevelAccelerationStructure.As<AccelerationStructure>());
			raygenPipeline->GetShader()->Set("outputImage", 0, outputImage);
			raygenPipeline->GetShader()->Set("Vertices", 0, mesh->GetVertexBuffer().As<BufferBase>());
			raygenPipeline->GetShader()->Set("Indices", 0, mesh->GetIndexBuffer().As<BufferBase>());
			raygenPipeline->GetShader()->Set("u_EnvironmentMap", 0, radianceMap);

			TraceRaysInfo info = {};
			info.Width = outputImageSpec.Width;
			info.Height = outputImageSpec.Height;
			info.Depth = 1;
			info.pBindingTable = bindingTable;

			commandBuffer->BindPipeline(raygenPipeline);
			commandBuffer->TraceRays(info);
			{
				ImageTransitionInfo imageInfo = {};
				imageInfo.Image = outputImage;
				imageInfo.SourceLayout = ImageLayout_General;
				imageInfo.DestLayout = ImageLayout_ShaderReadOnly;

				commandBuffer->TransitionImageLayout(imageInfo);
			}
			commandBuffer->BeginRenderPass(window->GetSwapchain()->GetRenderPass());

			screenPass->GetShader()->Set("u_Image", 0, outputImage);
			commandBuffer->BindPipeline(screenPass);
			commandBuffer->Draw(6);

			commandBuffer->EndRenderPass();
			{
				ImageTransitionInfo imageInfo = {};
				imageInfo.Image = outputImage;
				imageInfo.SourceLayout = ImageLayout_ShaderReadOnly;
				imageInfo.DestLayout = ImageLayout_General;

				commandBuffer->TransitionImageLayout(imageInfo);
			}

			Renderer::WaitAndRender();
			window->Present();
		}

		std::cout << "Test closed";
	}
}
