#pragma once

#include "HazardRendererCore.h"
#include <Color.h>

using namespace HazardRenderer;

static Window* CreateTestWindow(const char* title, RenderAPI api, bool* running)
{
	//Window creation
	HazardRendererAppInfo appInfo = {};
	appInfo.AppName = title;
	appInfo.BuildVersion = "0.0.1a";
	appInfo.MessageCallback = [](RenderMessage message) {
		std::cout << message.Description << std::endl;
	};
	appInfo.EventCallback = [running](Event& e) {
		EventDispatcher dispatcher(e);
		if (e.GetEventType() == EventType::WindowClose) {
			*running = false;
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
	windowInfo.Extent = { 1920, 1080 };
	windowInfo.Color = Color(255, 128, 0, 255);

	HazardRendererCreateInfo renderInfo = {};
	renderInfo.pAppInfo = &rendererApp;
	renderInfo.Renderer = api;
	renderInfo.Logging = true;
	renderInfo.VSync = true;
	renderInfo.WindowCount = 1;
	renderInfo.pWindows = &windowInfo;

	return Window::Create(&renderInfo);
}