#if 0
#define HZR_CUSTOM_ENTRY

#include "UtilityCore.h"
#include "File/File.h"
#include "HazardRenderer.h"

using namespace HazardUtility;
using namespace HazardRenderer;

int main() {

	static bool running = true;

	HazardRendererAppInfo appInfo = {};
	appInfo.AppName = "Hello Triangle";
	appInfo.BuildVersion = "0.0.1a";
	appInfo.MessageCallback = [](RenderMessage message) {
		std::cout << message.Message << std::endl;
	};
	appInfo.EventCallback = [](Event& e) {
		EventDispatcher dispatcher(e);
		if (e.GetEventType() == EventType::WindowClose) {
			running = false;
		}
	};

	HazardRendererCreateInfo createInfo = {};

	createInfo.Renderer = RenderAPI::OpenGL;
	createInfo.Width = 1280;
	createInfo.Height = 720;
	createInfo.VSync = true;
	createInfo.Logging = true;
	createInfo.Decorated = true;
	createInfo.Color = Color("#222222");
	createInfo.AppInfo = &appInfo;

	Window* window = Window::Create(&createInfo);
	window->Show();
	
	while (running)
	{
		window->BeginFrame();
		


		window->Present();
	}
	return 0;
}
#endif