
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

#include "Backend/Core/AccelerationStructure/AccelerationStructure.h"

#include "vendor/stb_image.h"
#include <glm/glm/ext/matrix_clip_space.hpp>
#include <glm/glm/gtc/type_ptr.hpp>

using namespace HazardRenderer;

namespace AccelerationStructureTest
{

	//OpenGL : Definitely a big nono
	//Vulkan : Working
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
		appInfo.AppName = "Hello Acceleration structures";
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
		windowInfo.FullScreen = false;
		windowInfo.Maximized = false;
		windowInfo.Width = 1280;
		windowInfo.Height = 720;
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

		//---------------
		float vertices[] =
		{
			-0.5f, -0.5f,-0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f,-0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};
		uint32_t indices[] = {
			0, 1, 2, 2, 3, 0
		};

		BufferLayout layout = { { "a_Position",			ShaderDataType::Float3 },
								{ "a_Color",			ShaderDataType::Float4 },
								{ "a_TextureCoords",	ShaderDataType::Float2 }
		};

		VertexBufferCreateInfo vbo = {};
		vbo.DebugName = "QuadVBO";
		vbo.Usage = BufferUsage::BLAS;
		vbo.Layout = &layout;
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;

		IndexBufferCreateInfo ibo = {};
		ibo.DebugName = "QuadIBO";
		ibo.Usage = BufferUsage::BLAS;
		ibo.Size = sizeof(indices);
		ibo.Data = indices;

		BoundingBox boundingBox = {};
		boundingBox.Encapsulate({ -0.5f, -0.5f, -0.5f });
		boundingBox.Encapsulate({  0.5f, -0.5f,  0.5f });
		boundingBox.Encapsulate({  0.5f,  0.5f,  0.5f });
		boundingBox.Encapsulate({ -0.5f,  0.5f, -0.5f });

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);

		Image2DCreateInfo outputImage = {};
		outputImage.DebugName = "OutputImage";
		outputImage.Width = 1920;
		outputImage.Height = 1080;
		outputImage.Format = ImageFormat::RGBA;
		outputImage.Usage = ImageUsage::Storage;

		Ref<Image2D> image = Image2D::Create(&outputImage);

		AccelerationStructureCreateInfo bottomAccelInfo = {};
		bottomAccelInfo.DebugName = "BottomLevelAccelerationStructure";
		bottomAccelInfo.Level = AccelerationStructureLevel::Bottom;
		bottomAccelInfo.VertexBuffer = vertexBuffer;
		bottomAccelInfo.IndexBuffer = indexBuffer;
		bottomAccelInfo.BoundingBox = boundingBox;

		Ref<BottomLevelAS> bottomLevelAccelerationStructure = BottomLevelAS::Create(&bottomAccelInfo);

		glm::mat4x3 transform = glm::mat4x3(glm::mat4(1.0));
		glm::mat4x3 transform1 = glm::mat4x3(glm::translate(glm::mat4(1.0f), { 0.0f, -1.0f, 1.0f }));


		std::vector<TransformMatrixAS> transforms(2);
		transforms[0] = *(TransformMatrixAS*)&transform;
		transforms[1] = *(TransformMatrixAS*)&transform1;

		BufferCopyRegion region = {};
		region.Data = transforms.data();
		region.Size = sizeof(TransformMatrixAS) * transforms.size();

		bottomLevelAccelerationStructure->PushTransforms(region);

		AccelerationStructureCreateInfo topAccelInfo = {};
		topAccelInfo.DebugName = "TopLevelAccelerationStructure";
		topAccelInfo.Level = AccelerationStructureLevel::Top;
		topAccelInfo.pBottomLevelAS = bottomLevelAccelerationStructure.Raw();
		topAccelInfo.BottomLevelASCount = 1;

		Ref<TopLevelAS> topLevelAccelerationStructure = TopLevelAS::Create(&topAccelInfo);

		std::vector<ShaderStageCode> shaderCode = ShaderCompiler::GetShaderBinariesFromSource("src/tests/Shaders/raygenBasic.glsl", api);
		std::vector<ShaderStageCode> screenPassCode = ShaderCompiler::GetShaderBinariesFromSource("src/tests/Shaders/composite.glsl", api);

		PipelineSpecification pipelineSpec = {};
		pipelineSpec.DebugName = "RaygenPipeline";
		pipelineSpec.Usage = PipelineUsage::Raygen;
		pipelineSpec.ShaderCodeCount = shaderCode.size();
		pipelineSpec.pShaderCode = shaderCode.data();

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
			cameraData.InvProjection = glm::inverse(glm::perspective(glm::radians(60.0f), aspectRatio, 0.03f, 100.0f));
			cameraData.InvView = glm::translate(glm::mat4(1.0f), { 0.0f, 0.0f, 5.0f });

			BufferCopyRegion region = {};
			region.Data = &cameraData;
			region.Size = sizeof(CameraData);

			camera->SetData(region);
			commandBuffer->BindUniformBuffer(camera);

			raygenPipeline->GetShader()->Set("topLevelAS", 0, topLevelAccelerationStructure.As<AccelerationStructure>());
			raygenPipeline->GetShader()->Set("image", 0, image);

			TraceRaysInfo rayInfo = {};
			rayInfo.Width = outputImage.Width;
			rayInfo.Height = outputImage.Height;
			rayInfo.Depth = 1;
			rayInfo.pBindingTable = bindingTable;

			commandBuffer->BindPipeline(raygenPipeline);
			commandBuffer->TraceRays(rayInfo);

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
