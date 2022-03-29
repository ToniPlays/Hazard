
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

using namespace HazardUtility;
using namespace HazardRenderer;

namespace RenderCommandTest {

	//OpenGL: Nothing
	//Vulkan: White quad
	//Metal: Not tested

	static void Run(RenderAPI api)
	{
		float scalar = 450.0f;
		float aspectRatio = (float)1280 / (float)720;
		glm::mat4 view = glm::translate(glm::mat4(1.0f), { 0, 0, -20 });
		glm::mat4 projection = glm::ortho(-aspectRatio * scalar, aspectRatio * scalar, -scalar, scalar, -100.0f, 100.0f);

		Camera camera = {};
		camera.ViewProjection = projection * view;


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
					camera.ViewProjection = projection * view;
				}
				else if (k.GetKeyCode() == Key::S) {
					scalar += 25.0f;
					glm::mat4 view = glm::translate(glm::mat4(1.0f), { 0, 0, -20 });
					glm::mat4 projection = glm::ortho(-aspectRatio * scalar, aspectRatio * scalar, -scalar, scalar, -100.0f, 100.0f);
					camera.ViewProjection = projection * view;
				}
			}
		};

		HazardRendererCreateInfo createInfo = {};
		createInfo.Renderer = api;
		createInfo.Width = 1280;
		createInfo.Height = 720;
		createInfo.VSync = true;
		createInfo.Logging = true;
		createInfo.Color = { 0.1f, 0.1f, 0.125f, 1.0f };
		createInfo.AppInfo = &appInfo;

		Window* window = Window::Create(&createInfo);
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
		spec.TargetRenderPass = renderPass;
		spec.Usage = PipelineUsage::GraphicsBit;

		Ref<Pipeline> pipeline = Pipeline::Create(&spec);

		UniformBufferCreateInfo ubo = {};
		ubo.Name = "Camera";
		ubo.Binding = 0;
		ubo.Usage = BufferUsage::DynamicDraw;
		ubo.Size = sizeof(Camera);

		Ref<UniformBuffer> uniformBuffer = UniformBuffer::Create(&ubo);

		std::cout << "Selected device: " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;

		while (running)
		{
			window->BeginFrame();
			RenderCommand::BeginFrame();
			RenderCommand::BeginWorld(settings);

			for (int32_t x = -160; x < 160; x++) {
				for (int32_t y = -160; y < 160; y++) {
					RenderCommand::DrawQuad(pipeline, glm::translate(glm::mat4(1.0f), { (float)x * 1.25f, (float)y * 1.25f, 0 }), Color::White);
				}
			}

			uniformBuffer->SetData(settings.RenderingCamera, sizeof(Camera));
			RenderCommand::Flush();
			window->Present();
		}

		std::cout << "Test closed";
	}
}
