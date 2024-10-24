
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"

#include "vendor/stb_image.h"

using namespace HazardRenderer;

namespace TexturedQuad {

	//OpenGL : Working
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
		appInfo.AppName = "Hello Textured quad";
		appInfo.BuildVersion = "0.0.1a";
		appInfo.MessageCallback = [](RenderMessage message) {
			std::cout << message.Description << std::endl;
		};
		appInfo.EventCallback = [](Event& e) {
			EventDispatcher dispatcher(e);
			if (e.GetEventType() == EventType::WindowClose) {
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
		windowInfo.Color = Color(255, 128, 0, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &rendererApp;
		renderInfo.Renderer = api;
		renderInfo.Logging = true;
		renderInfo.VSync = true;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;

		Window* window = Window::Create(&renderInfo);
		window->Show();

		//---------------
		std::cout << "Selected device: " << window->GetContext()->GetDevice()->GetDeviceName() << std::endl;
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
		};
		uint32_t indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		BufferLayout layout = { { "a_Position",			ShaderDataType::Float3 },
								{ "a_Color",			ShaderDataType::Float4 },
								{ "a_TextureCoords",	ShaderDataType::Float2 }
		};

		std::vector<ShaderStageCode> code = ShaderCompiler::GetShaderBinariesFromSource("src/tests/Shaders/texturedQuad.glsl", api);

		stbi_set_flip_vertically_on_load(true);

		int w, h, channels;
		stbi_uc* data = stbi_load("src/tests/Textures/csharp.png", &w, &h, &channels, 4);

		VertexBufferCreateInfo vbo = {};
		vbo.DebugName = "TriangleVBO";
		vbo.Usage = BufferUsage::StaticDraw;
		vbo.Layout = &layout;
		vbo.Size = sizeof(vertices);
		vbo.Data = &vertices;

		IndexBufferCreateInfo ibo = {};
		ibo.DebugName = "TriangleIBO";
		ibo.Usage = BufferUsage::StaticDraw;
		ibo.Size = sizeof(indices);
		ibo.Data = indices;

		PipelineSpecification spec = {};
		spec.DebugName = "Pipeline";
		spec.Usage = PipelineUsage::GraphicsBit;
		spec.DrawType = DrawType::Fill;
		spec.CullMode = CullMode::None;
		spec.pTargetRenderPass = window->GetSwapchain()->GetRenderPass().Raw();
		spec.DepthTest = false;
		spec.pBufferLayout = &layout;
		spec.ShaderCodeCount = code.size();
		spec.pShaderCode = code.data();

		Image2DCreateInfo imageInfo = {};
		imageInfo.DebugName = "Image2D";
		imageInfo.Width = w;
		imageInfo.Height = h;
		imageInfo.Format = ImageFormat::RGBA;
		imageInfo.Data = Buffer(data, w * h * 4);
		imageInfo.Usage = ImageUsage::Texture;

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vbo);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&ibo);
		Ref<Pipeline> pipeline = Pipeline::Create(&spec);
		Ref<Image2D> image = Image2D::Create(&imageInfo);



		while (running)
		{
			Input::Update();
			window->BeginFrame();

			auto commandBuffer = window->GetSwapchain()->GetSwapchainBuffer();
			commandBuffer->BeginRenderPass(window->GetSwapchain()->GetRenderPass());
			commandBuffer->BindVertexBuffer(vertexBuffer);
			pipeline->GetShader()->Set("u_Texture", 0, image);

			commandBuffer->BindPipeline(pipeline);
			commandBuffer->Draw(indexBuffer->GetCount(), indexBuffer);

			commandBuffer->EndRenderPass();
			Renderer::WaitAndRender();
			window->Present();
		}

		std::cout << "Test closed";
	}
}
