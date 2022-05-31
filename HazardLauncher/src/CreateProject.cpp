#if 0
#define HZR_CUSTOM_ENTRY

#include "UtilityCore.h"
#include "File/File.h"
#include "HazardRenderer.h"


#if _WIN32
#define PATH_PREFIX ""
#else
#define PATH_PREFIX "../../../HazardLauncher/"
#endif

struct CreateProjectInfo {
	std::string ProjectName;
	std::filesystem::path Path;
};

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

	createInfo.Renderer = RenderAPI::Vulkan;
	createInfo.Width = 1280;
	createInfo.Height = 720;
	createInfo.VSync = true;
	createInfo.Logging = true;
	createInfo.Color = { 0.1f, 0.1f, 0.125f, 1.0f };
	createInfo.AppInfo = &appInfo;


	Window* window = Window::Create(&createInfo);
	window->Show();
	
	while (running) {
		window->BeginFrame();

		window->Present();
	}

	return 0;
}
#endif