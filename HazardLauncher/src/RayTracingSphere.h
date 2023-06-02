
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "Backend/Core/RTCore/AccelerationStructure.h"

#include "Hazard/Math/Time.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <Hazard/Rendering/Mesh/Mesh.h>
#include <Hazard/Rendering/Mesh/MeshFactory.h>
#include <Hazard.h>
#include <EditorCamera.h>
#include "Hazard/RenderContext/TextureFactory.h"
#include "Hazard/RenderContext/ShaderLibrary.h"

#include <GLFW/glfw3.h>

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
		windowInfo.Extent = { 1920, 1080 };
		windowInfo.Color = Color(32, 32, 32, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &rendererApp;
		renderInfo.Renderer = api;
		renderInfo.Logging = true;
		renderInfo.VSync = true;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;
#pragma endregion
		Window* window = Window::Create(&renderInfo);
		window->Show();

		auto device = window->GetContext()->GetDevice();

		std::cout << "Selected device: " << device->GetDeviceName() << std::endl;
		HZR_ASSERT(device->SupportsRaytracing(), "Device does not support Raytracing");

#pragma region Mesh
		MeshFactory factory = {};
		factory.SetOptimization(MeshLoaderFlags_DefaultFlags);

		MeshData corvetteData = factory.LoadMeshFromSource("assets/models/c8_corvette_colored.fbx");

		MeshCreateInfo corvetteInfo = {};
		corvetteInfo.DebugName = "Corvette";
		corvetteInfo.BoundingBox = corvetteData.BoundingBox;
		corvetteInfo.VertexCount = corvetteData.Vertices.size() * sizeof(Vertex3D);
		corvetteInfo.pVertices = corvetteData.Vertices.data();
		corvetteInfo.IndexCount = corvetteData.Indices.size() * sizeof(uint32_t);
		corvetteInfo.pIndices = corvetteData.Indices.data();
		corvetteInfo.UsageFlags = BUFFER_USAGE_BOTTOM_LEVEL_ACCELERATION_STRUCTURE;

		Ref<Mesh> corvette = Ref<Mesh>::Create(&corvetteInfo);

		MeshData cubeData = factory.LoadMeshFromSource("assets/models/cube.fbx");

		MeshCreateInfo cubeInfo = {};
		cubeInfo.DebugName = "Cube";
		cubeInfo.BoundingBox = cubeData.BoundingBox;
		cubeInfo.VertexCount = cubeData.Vertices.size() * sizeof(Vertex3D);
		cubeInfo.pVertices = cubeData.Vertices.data();
		cubeInfo.IndexCount = cubeData.Indices.size() * sizeof(uint32_t);
		cubeInfo.pIndices = cubeData.Indices.data();
		cubeInfo.UsageFlags = BUFFER_USAGE_BOTTOM_LEVEL_ACCELERATION_STRUCTURE;

		Ref<Mesh> cube = Ref<Mesh>::Create(&cubeInfo);


#pragma endregion
#pragma region OutputImage
		Image2DCreateInfo outputImageSpec = {};
		outputImageSpec.DebugName = "OutputImage";
		outputImageSpec.Extent.Width = window->GetWidth();
		outputImageSpec.Extent.Height = window->GetHeight();
		outputImageSpec.Format = ImageFormat::RGBA;
		outputImageSpec.Usage = ImageUsage::Storage;

		Ref<Image2D> outputImage = Image2D::Create(&outputImageSpec);
#pragma endregion
#pragma region RayTracing
#pragma region BLAS

		AccelerationStructureGeometry carGeometry = {};
		carGeometry.VertexBuffer = corvette->GetVertexBuffer();
		carGeometry.IndexBuffer = corvette->GetIndexBuffer();
		carGeometry.BoundingBox = corvette->GetBoundingBox();
		carGeometry.Transform = glm::mat4(1.0f);

		AccelerationStructureGeometry cubeGeometry = {};
		cubeGeometry.VertexBuffer = cube->GetVertexBuffer();
		cubeGeometry.IndexBuffer = cube->GetIndexBuffer();
		cubeGeometry.BoundingBox = cube->GetBoundingBox();
		cubeGeometry.Transform = glm::translate(glm::mat4(1.0f), { 0.0f, 1.0f, 6.0f });

		std::vector<AccelerationStructureGeometry> geometries
		{
			carGeometry, cubeGeometry
		};

		AccelerationStructureCreateInfo bottomAccelInfo = {};
		bottomAccelInfo.DebugName = "BottomLevelAccelerationStructure";
		bottomAccelInfo.Level = AccelerationStructureLevel::Bottom;
		bottomAccelInfo.GeometryCount = geometries.size();
		bottomAccelInfo.pGeometries = geometries.data();

		Ref<BottomLevelAS> bottomLevelAccelerationStructure = BottomLevelAS::Create(&bottomAccelInfo);

		AccelerationStructureInstance instance1 = {};
		instance1.CustomIndex = 0;
		instance1.pBottomLevelAS = bottomLevelAccelerationStructure;
		instance1.Transform = glm::translate(glm::mat4(1.0f), { -4.0f, 0.0f, 0.0f });

		AccelerationStructureInstance instance2 = {};
		instance2.CustomIndex = 1;
		instance2.pBottomLevelAS = bottomLevelAccelerationStructure;
		instance2.Transform = glm::translate(glm::mat4(1.0f), { 4.0f, 0.0f, 0.0f });

		AccelerationStructureInstance instance3 = {};
		instance3.CustomIndex = 2;
		instance3.pBottomLevelAS = bottomLevelAccelerationStructure;

		std::vector<AccelerationStructureInstance> instances = {
			instance1, instance2, instance3
		};
#pragma endregion
#pragma region TLAS
		AccelerationStructureCreateInfo topAccelInfo = {};
		topAccelInfo.DebugName = "TopLevelAccelerationStructure";
		topAccelInfo.Level = AccelerationStructureLevel::Top;
		topAccelInfo.InstanceCount = instances.size();
		topAccelInfo.pInstances = instances.data();

		Ref<TopLevelAS> topLevelAccelerationStructure = TopLevelAS::Create(&topAccelInfo);
#pragma endregion
#pragma region Raygen
		std::vector<ShaderStageCode> shaderCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/raygen.glsl", api);
		std::vector<ShaderStageCode> screenPassCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/composite.glsl", api);

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
#pragma region EnvironmentMap

		TextureHeader header = TextureFactory::LoadTextureFromSourceFile("assets/Textures/pink_sunrise_4k.hdr", true);

		Image2DCreateInfo envInfo = {};
		envInfo.DebugName = "EnvMap";
		envInfo.Format = ImageFormat::RGBA;
		envInfo.Usage = ImageUsage::Texture;
		envInfo.Extent.Width = header.Width;
		envInfo.Extent.Height = header.Height;
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

		std::vector<ShaderStageCode> computeCode = ShaderCompiler::GetShaderBinariesFromSource("assets/Shaders/EquirectangularToCubeMap.glsl", api);

		PipelineSpecification computeSpec = {};
		computeSpec.DebugName = "EquirectangularToCubemap";
		computeSpec.Usage = PipelineUsage::ComputeBit;
		computeSpec.ShaderCodeCount = computeCode.size();
		computeSpec.pShaderCode = computeCode.data();

		Ref<Pipeline> computePipeline = Pipeline::Create(&computeSpec);

		auto shader = computePipeline->GetShader();
		shader->Set("o_CubeMap", 0, radianceMap);
		shader->Set("u_EquirectangularTexture", 0, environmentMapImage);

		DispatchComputeInfo computeInfo = {};
		computeInfo.GroupSize = { radianceInfo.Width / 32, radianceInfo.Height / 32, 6 };
		computeInfo.WaitForCompletion = true;

		computeBuffer->Begin();
        computeBuffer->SetPipeline(computePipeline);
		computeBuffer->DispatchCompute(computeInfo);
		computeBuffer->End();
		computeBuffer->Submit();

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

		Ref<UniformBuffer> cameraUBO = UniformBuffer::Create(&uboInfo);

		while (running)
		{
			Time::Update(glfwGetTime());
			Input::Update();

			camera.OnUpdate();

			window->BeginFrame();

			auto commandBuffer = window->GetSwapchain()->GetSwapchainBuffer();

			CameraData cameraData = {};
			cameraData.InvProjection = glm::inverse(camera.GetProjection());
			cameraData.InvView = glm::inverse(camera.GetView());

			BufferCopyRegion copyRegion = {};
			copyRegion.Data = &cameraData;
			copyRegion.Size = sizeof(CameraData);

			cameraUBO->SetData(copyRegion);
			commandBuffer->SetUniformBuffers(&cameraUBO, 1);

			raygenPipeline->GetShader()->Set("TLAS", 0, topLevelAccelerationStructure.As<AccelerationStructure>());
			raygenPipeline->GetShader()->Set("outputImage", 0, outputImage);
			raygenPipeline->GetShader()->Set("Vertices", 0, corvette->GetVertexBuffer().As<BufferBase>());
			raygenPipeline->GetShader()->Set("Indices", 0, corvette->GetIndexBuffer().As<BufferBase>());
			raygenPipeline->GetShader()->Set("u_EnvironmentMap", 0, radianceMap);

			TraceRaysInfo info = {};
			info.Extent = outputImageSpec.Extent;
			info.pBindingTable = bindingTable;

			commandBuffer->SetPipeline(raygenPipeline);
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
			commandBuffer->SetPipeline(screenPass);
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
	}
}
