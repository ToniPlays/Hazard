
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

#include <GLFW/glfw3.h>

using namespace HazardRenderer;

namespace RenderCommandTest {

	//OpenGL: Nothing
	//Vulkan: White quad
	//Metal: Not tested

	static void Run(RenderAPI api)
	{
		float scalar = 10.0f;
		float aspectRatio = (float)1280 / (float)720;
		glm::mat4 view = glm::translate(glm::mat4(1.0f), { 0, 0, -20 });
		glm::mat4 projection = glm::ortho(-aspectRatio * scalar, aspectRatio * scalar, -scalar, scalar, -100.0f, 100.0f);

		Camera camera = {};
		camera.SetProjection(projection);
		camera.SetView(view);


		std::cout << "Running RenderCommand test" << std::endl;
		static bool running = true;

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Hello RenderCommand";
		appInfo.BuildVersion = "0.0.1a";
		appInfo.MessageCallback = [](RenderMessage message) {
			std::cout << message.Message << std::endl;
		};
		appInfo.EventCallback = [&](Event& e) {
			EventDispatcher dispatcher(e);
			if (e.GetEventType() == EventType::WindowClose) {
				running = false;
			}
			if (e.GetEventType() == EventType::KeyPressed) {
				KeyPressedEvent& k = (KeyPressedEvent&)e;
				if (k.GetKeyCode() == Key::W) {
					scalar -= 25.0f;
					glm::mat4 view = glm::translate(glm::mat4(1.0f), { 0, 0, -20 });
					glm::mat4 projection = glm::ortho(-aspectRatio * scalar, aspectRatio * scalar, -scalar, scalar, -100.0f, 100.0f);
					camera.SetProjection(projection);
					camera.SetView(view);
				}
				else if (k.GetKeyCode() == Key::S) {
					scalar += 25.0f;
					glm::mat4 view = glm::translate(glm::mat4(1.0f), { 0, 0, -20 });
					glm::mat4 projection = glm::ortho(-aspectRatio * scalar, aspectRatio * scalar, -scalar, scalar, -100.0f, 100.0f);
					camera.SetProjection(projection);
					camera.SetView(view);
				}
			}
		};

		HazardRendererAppInfo rendererApp = {};
		rendererApp.AppName = appInfo.AppName;
		rendererApp.BuildVersion = "1.0.0!";
		rendererApp.EventCallback = [&](Event& e) {

		};
		rendererApp.MessageCallback = [](RenderMessage message) {
			std::cout << message.Message << std::endl;
		};


		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = "HazardEditor";
		windowInfo.FullScreen = false;
		windowInfo.Maximized = false;
		windowInfo.Decorated = true;
		windowInfo.Width = 1920;
		windowInfo.Height = 1080;
		windowInfo.Color = Color(34, 34, 34, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &rendererApp;
		renderInfo.Renderer = RenderAPI::Vulkan;
		renderInfo.VSync = true;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;

		Window* window = Window::Create(&renderInfo);
		window->Show();


		//camera.View = view;
		//camera.Projection = projection;
		//camera.Position = { 0.0f, 0.0f, 0.0f };

		FrameBufferCreateInfo fboInfo = {};
		fboInfo.DebugName = "SwapchainTarget";
		fboInfo.SwapChainTarget = true;
		fboInfo.Attachments = { ImageFormat::RGBA, ImageFormat::Depth };
		fboInfo.AttachmentCount = 2;

		Ref<FrameBuffer> frameBuffer = FrameBuffer::Create(&fboInfo);

		RenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.DebugName = "RenderPass";
		renderPassInfo.pTargetFrameBuffer = frameBuffer;

		Ref<RenderPass> renderPass = RenderPass::Create(&renderPassInfo);

		WorldSettings settings = {};
		settings.RenderingCamera = &camera;
		settings.TargetRenderPass = renderPass;

		PipelineSpecification spec = {};
		spec.DebugName = "Pipeline";
		spec.ShaderPath = "quadInstanced.glsl";
		spec.DrawType = DrawType::Fill;
		spec.CullMode = CullMode::None;
		spec.Usage = PipelineUsage::GraphicsBit;
		spec.TargetRenderPass = renderPass;

		Ref<Pipeline> pipeline = Pipeline::Create(&spec);

		UniformBufferCreateInfo ubo = {};
		ubo.Name = "Camera";
		ubo.Binding = 0;
		ubo.Usage = BufferUsage::DynamicDraw;
		ubo.Size = sizeof(Camera);

		Ref<UniformBuffer> uniformBuffer = UniformBuffer::Create(&ubo);

		std::cout << "Selected device: " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;

		double lastTime = 0;
		double frameTime = 0;

		std::vector<glm::mat4> transforms;

		for (int32_t x = -500; x < 500; x++) {
			for (int32_t y = -500; y < 500; y++) {
				transforms.push_back(glm::translate(glm::mat4(1.0f), { (float)x * 1.25f, (float)y * 1.25f, 0 }));
			}
		}

		glm::vec4 color = Color::White;
		RenderCommand::BeginFrame();
		RenderCommand::BeginWorld(settings);
		uniformBuffer->SetData(settings.RenderingCamera, sizeof(Camera));

		for (const glm::mat4& transform : transforms)
		{
			RenderCommand::DrawQuad(pipeline, transform, color);
		}
		
		while (running)
		{
			double time = glfwGetTime();

			//Update Time
			frameTime = time - lastTime;
			lastTime = time;

			window->BeginFrame();

			RenderCommand::Flush();

			window->Present();
		}

		std::cout << "Test closed";
	}
}
