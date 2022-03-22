#include "HazardRenderer.h"

int main() 
{
	using namespace HazardRenderer;

	try {

		std::vector<const char*> icons = { "res/world.png", "res/world.png" };

		HazardRendererAppInfo appInfo = {};
		appInfo.AppName = "Example triangle";
		appInfo.BuildVersion = 1;
		appInfo.Icons = icons.data();
		appInfo.IconCount = 2;

		HazardRendererCreateInfo createInfo = {};
		createInfo.Renderer = RenderAPI::Vulkan;
		createInfo.Width = 1280;
		createInfo.Height = 720;
		createInfo.Maximized = false;
		createInfo.FullScreen = false;
		createInfo.VSync = false;
		createInfo.Color = { 0.2f, 0.2f, 0.2f, 1.0f };
		createInfo.AppInfo = &appInfo;

		Window* window = Window::Create(&createInfo);
		window->Show();

		while (true)
		{
			window->BeginFrame();



			window->Present();
		}
	}
	catch (std::exception& e) 
	{
		std::cout << e.what() << std::endl;
	}

	return 0;
}