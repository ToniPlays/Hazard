
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "Backend/Core/AccelerationStructure/AccelerationStructure.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <Hazard/Rendering/Mesh/Mesh.h>
#include <Hazard/Rendering/Mesh/MeshFactory.h>
#include <Hazard.h>

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

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Hello RayTracing";
		appInfo.BuildVersion = "0.0.1a";
		appInfo.MessageCallback = [](RenderMessage message)
		{
			std::cout << message.Description << std::endl;
		};
		appInfo.EventCallback = [](Event& e) {
			EventDispatcher dispatcher(e);
			if (e.GetEventType() == EventType::WindowClose)
			{
				running = false;
			}
		};

		Logging::Logger logger = Logging::Logger();

		HazardRendererAppInfo rendererApp = {};
		rendererApp.AppName = appInfo.AppName;
		rendererApp.BuildVersion = "1.0.0!";
		rendererApp.EventCallback = appInfo.EventCallback;

		rendererApp.MessageCallback = [](RenderMessage message)
		{
			std::cout << message.Description << std::endl;
			std::cout << message.StackTrace << std::endl;
		};

		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = appInfo.AppName;
		windowInfo.Maximized = false;
		windowInfo.FullScreen = false;
		windowInfo.Width = 1600;
		windowInfo.Height = 900;
		windowInfo.Color = Color(32, 32, 32, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &rendererApp;
		renderInfo.Renderer = api;
		renderInfo.Logging = true;
		renderInfo.VSync = true;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;

		Window* window = Window::Create(&renderInfo);
		window->Show();

		auto& device = window->GetContext()->GetDevice();

		std::cout << "Selected device: " << device->GetDeviceName() << std::endl;
		HZR_ASSERT(device->SupportsRaytracing(), "Device does not support Raytracing");

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
		//---------------

		Image2DCreateInfo outputImage = {};
		outputImage.DebugName = "OutputImage";
		outputImage.Width = window->GetWidth();
		outputImage.Height = window->GetHeight();
		outputImage.Format = ImageFormat::RGBA;
		outputImage.Usage = ImageUsage::Storage;

		Ref<Image2D> image = Image2D::Create(&outputImage);

		AccelerationStructureCreateInfo bottomAccelInfo = {};
		bottomAccelInfo.DebugName = "BottomLevelAccelerationStructure";
		bottomAccelInfo.Level = AccelerationStructureLevel::Bottom;
		bottomAccelInfo.VertexBuffer = mesh->GetVertexBuffer();
		bottomAccelInfo.IndexBuffer = mesh->GetIndexBuffer();
		bottomAccelInfo.BoundingBox = mesh->GetBoundingBox();
		Ref<AccelerationStructure> bottomLevelAccelerationStructure = AccelerationStructure::Create(&bottomAccelInfo);

		AccelerationStructureCreateInfo topAccelInfo = {};
		topAccelInfo.DebugName = "TopLevelAccelerationStructure";
		topAccelInfo.Level = AccelerationStructureLevel::Top;
		topAccelInfo.pAccelerationStructure = bottomLevelAccelerationStructure;

		Ref<AccelerationStructure> topLevelAccelerationStructure = AccelerationStructure::Create(&topAccelInfo);
		std::vector<ShaderStageCode> shaderCode = ShaderCompiler::GetShaderBinariesFromSource("src/tests/Shaders/raygen.glsl", api);
		std::vector<ShaderStageCode> screenPassCode = ShaderCompiler::GetShaderBinariesFromSource("src/tests/Shaders/composite.glsl", api);

		PipelineSpecification pipelineSpec = {};
		pipelineSpec.DebugName = "RaygenPipeline";
		pipelineSpec.Usage = PipelineUsage::Raygen;
		pipelineSpec.ShaderCodeCount = shaderCode.size();
		pipelineSpec.pShaderCode = shaderCode.data();
		pipelineSpec.MaxRayDepth = 1;

		Ref<Pipeline> raygenPipeline = Pipeline::Create(&pipelineSpec);

		ShaderBindingTableCreateInfo bindingSpec = {};
		bindingSpec.pPipeline = raygenPipeline;

		Ref<ShaderBindingTable> bindingTable = ShaderBindingTable::Create(&bindingSpec);

		PipelineSpecification screenPassSpec = {};
		screenPassSpec.DebugName = "ScreenPass";
		screenPassSpec.Usage = PipelineUsage::GraphicsBit;
		screenPassSpec.CullMode = CullMode::None;
		screenPassSpec.DepthTest = false;
		screenPassSpec.ShaderCodeCount = screenPassCode.size();
		screenPassSpec.pShaderCode = screenPassCode.data();
		screenPassSpec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass();

		Ref<Pipeline> screenPass = Pipeline::Create(&screenPassSpec);

		struct CameraData
		{
			glm::mat4 InvView;
			glm::mat4 InvProjection;
		};

		UniformBufferCreateInfo uboInfo = {};
		uboInfo.Name = "Camera";
		uboInfo.Set = 0;
		uboInfo.Binding = 2;
		uboInfo.Size = sizeof(CameraData);
		uboInfo.Usage = BufferUsage::DynamicDraw;

		Ref<UniformBuffer> camera = UniformBuffer::Create(&uboInfo);

		while (running)
		{
			Input::Update();
			window->BeginFrame();

			auto& commandBuffer = window->GetSwapchain()->GetSwapchainBuffer();

			CameraData cameraData = {};
			float aspectRatio = (float)window->GetWidth() / (float)window->GetHeight();

			glm::vec3 Translation = { 0.0f, -1.0f, -5.0f };
			glm::quat Rotation = glm::quat({ glm::radians(0.0f), glm::radians(70.0f), glm::radians(0.0f) });
			glm::vec3 Scale = glm::vec3(1.0f);

			glm::mat4 cameraView = Math::ToTransformMatrix(Translation, Rotation, Scale);

			cameraData.InvProjection = glm::inverse(glm::perspective(glm::radians(45.0f), aspectRatio, 0.03f, 100.0f));
			cameraData.InvView = glm::inverse(cameraView);

			BufferCopyRegion region = {};
			region.Data = &cameraData;
			region.Size = sizeof(CameraData);

			camera->SetData(region);
			commandBuffer->BindUniformBuffer(camera);

			raygenPipeline->GetShader()->Set("topLevelAS", 0, topLevelAccelerationStructure);
			raygenPipeline->GetShader()->Set("image", 0, image);
			raygenPipeline->GetShader()->Set("Vertices", 0, mesh->GetVertexBuffer().As<BufferBase>());
			raygenPipeline->GetShader()->Set("Indices", 0, mesh->GetIndexBuffer().As<BufferBase>());

			TraceRaysInfo info = {};
			info.Width = outputImage.Width;
			info.Height = outputImage.Height;
			info.Depth = 1;
			info.pBindingTable = bindingTable;

			commandBuffer->BindPipeline(raygenPipeline);
			commandBuffer->TraceRays(info);

			ImageTransitionInfo imageInfo = {};
			imageInfo.Image = image;
			imageInfo.SourceLayout = ImageLayout_General;
			imageInfo.DestLayout = ImageLayout_ShaderReadOnly;

			commandBuffer->TransitionImageLayout(imageInfo);
			commandBuffer->BeginRenderPass(window->GetSwapchain()->GetRenderPass());

			screenPass->GetShader()->Set("u_Image", 0, image);

			commandBuffer->BindPipeline(screenPass);
			commandBuffer->Draw(6);

			commandBuffer->EndRenderPass();
			{
				ImageTransitionInfo imageInfo = {};
				imageInfo.Image = image;
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
