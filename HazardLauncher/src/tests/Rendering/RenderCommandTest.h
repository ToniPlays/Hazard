
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
		uint32_t size = 10;

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

			if (e.GetEventType() == EventType::WindowClose) {
				running = false;
			}
			if (e.GetEventType() == EventType::MouseScrolled) {
				MouseScrolledEvent& k = (MouseScrolledEvent&)e;
				scalar -= k.GetYOffset() * 5.0f;
				glm::mat4 view = glm::translate(glm::mat4(1.0f), { 0, 0, -20 });
				glm::mat4 projection = glm::ortho(-aspectRatio * scalar, aspectRatio * scalar, -scalar, scalar, -100.0f, 100.0f);
				camera.SetProjection(projection);
				camera.SetView(view);
			}

		};

		HazardRendererAppInfo rendererApp = {};
		rendererApp.AppName = appInfo.AppName;
		rendererApp.BuildVersion = "1.0.0!";
		rendererApp.EventCallback = appInfo.EventCallback;

		rendererApp.MessageCallback = [](RenderMessage message) {
			std::cout << message.Message << std::endl;
		};


		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = "RenderCommandTest | Quads: " + std::to_string(size * size);
		windowInfo.FullScreen = false;
		windowInfo.Maximized = false;
		windowInfo.Decorated = true;
		windowInfo.Width = 1920;
		windowInfo.Height = 1080;
		windowInfo.Color = Color(34, 34, 34, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &rendererApp;
		renderInfo.Renderer = api;
		renderInfo.VSync = true;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;

		Window* window = Window::Create(&renderInfo);
		window->Show();

		WorldSettings settings = {};
		settings.RenderingCamera = &camera;
		settings.TargetRenderPass = window->GetSwapchain()->GetRenderPass();

		PipelineSpecification spec = {};
		spec.DebugName = "Pipeline";
		spec.ShaderPath = "quadInstanced.glsl";
		spec.DrawType = DrawType::Fill;
		spec.Usage = PipelineUsage::GraphicsBit;
		spec.TargetRenderPass = settings.TargetRenderPass;

		Ref<Pipeline> pipeline = Pipeline::Create(&spec);

		UniformBufferCreateInfo ubo = {};
		ubo.Name = "Camera";
		ubo.Binding = 0;
		ubo.Usage = BufferUsage::DynamicDraw;
		ubo.Size = sizeof(Camera);

		Ref<UniformBuffer> uniformBuffer = UniformBuffer::Create(&ubo);

		std::cout << "Selected device: " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;

		RenderCommand::BeginFrame();
		RenderCommand::BeginWorld(settings);

		for (int32_t x = 0; x < size; x++) {
			for (int32_t y = 0; y < size; y++) {
				glm::vec4 color = { (float)x / (float)size, (float)y / (float)size, 0.0f, 1.0f };
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), { (float)(x - size / 2.0f) * 1.0f, (float)(y - size / 2.0f) * 1.0f, 0 });
				RenderCommand::DrawQuad(pipeline, transform, color);
			}
		}

		double startTime = 0;
		std::string title = window->GetWindowInfo().Title;

		while (running)
		{
			uniformBuffer->SetData(&settings.RenderingCamera->GetViewPprojection(), sizeof(Camera));
			double time = glfwGetTime();
			window->SetWindowTitle(title + " frame time " + std::to_string((time - startTime)));
			window->SetWindowTitle(window->GetWindowInfo().Title + " fps " + std::to_string((1.0f / (time - startTime))));
			startTime = time;

			window->BeginFrame();

			RenderCommand::Flush();

			window->Present();
		}

		std::cout << "Test closed";
	}
}
