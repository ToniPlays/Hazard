
#include "HazardRenderer.h"
#include "Event.h"
#include "Color.h"
#include "MathCore.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "GLFW/glfw3.h"
#include <glad/glad.h>

using namespace HazardRenderer;

namespace SceneTest {

	//OpenGL:
	//Vulkan:
	//Metal:

	static void Run(RenderAPI api)
	{
		static bool running = true;

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Hello 3D world";
		appInfo.BuildVersion = "0.1.0a";
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
		rendererApp.BuildVersion = "1.0.0";
		rendererApp.EventCallback = appInfo.EventCallback;
		rendererApp.MessageCallback = [](RenderMessage message) {
			std::cout << message.Description << std::endl;
		};

		HazardWindowCreateInfo windowInfo = {};
		windowInfo.Title = appInfo.AppName;
		windowInfo.FullScreen = false;
		windowInfo.Maximized = false;
		windowInfo.Width = 1280;
		windowInfo.Height = 720;
		windowInfo.Color = Color(34, 34, 34, 255);

		HazardRendererCreateInfo renderInfo = {};
		renderInfo.pAppInfo = &rendererApp;
		renderInfo.Renderer = api;
		renderInfo.VSync = false;
		renderInfo.WindowCount = 1;
		renderInfo.pWindows = &windowInfo;

		Window* window = Window::Create(&renderInfo);
		window->Show();

			
		std::cout << "Selected device: " << window->GetContext()->GetDevice().GetDeviceName() << std::endl;

		while (running)
		{
			window->BeginFrame();



			window->Present();
		}
		window->Close();
		std::cout << "Test closed";
	}
}
