
#include "hzrpch.h"
#include "HazardEditor.h"
#include "Editor/EditorCamera.h"
#include "HazardRenderer.h"
#include "Project/ProjectManager.h"
#include "GUIManager.h"
#include "GUI/Debug/Console.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "HazardScript.h"
#include "EditorScripting/EditorScriptGlue.h"

using namespace Hazard;
using namespace HazardRenderer;
using namespace HazardScript;

static uint32_t GetMessageFlagsFromSeverity(const Severity& severity)
{
	switch (severity)
	{
	case Severity::Debug:		return MessageFlags_Debug | MessageFlags_Clearable;
	case Severity::Info:		return MessageFlags_Info | MessageFlags_Clearable;
	case Severity::Trace:		return MessageFlags_Info | MessageFlags_Clearable;
	case Severity::Warning:		return MessageFlags_Warning | MessageFlags_Clearable;
	case Severity::Error:		return MessageFlags_Error | MessageFlags_Clearable;
	case Severity::Critical:	return MessageFlags_Fatal;
	}
	return 0;
}


void EditorApplication::PreInit()
{
	RenderAPI renderAPI = RenderAPI::OpenGL;

	std::string workingDir = CommandLineArgs::Get<std::string>("wdir");
	if (!workingDir.empty()) {
		std::filesystem::current_path(workingDir);
		std::cout << "Working directory: " << std::filesystem::current_path().string() << std::endl;
	}
	HazardProject& project = PushModule<ProjectManager>().LoadProjectFromFile(CommandLineArgs::Get<std::string>("hprj"));
	

	std::vector<const char*> icons = { "res/Icons/logo.png", "res/Icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.AppName = "Hazard Editor";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
	appInfo.Logging = true;

	HazardRendererAppInfo rendererApp = {};
	rendererApp.AppName = appInfo.AppName;
	rendererApp.BuildVersion = HZR_BUILD_VERSION;
	rendererApp.IconCount = 2;
	rendererApp.pIcons = icons.data();
	rendererApp.EventCallback = [&](Event& e) {
		HazardLoop::GetCurrent().OnEvent(e);
	};
	rendererApp.MessageCallback;

	HazardWindowCreateInfo windowInfo = {};
	windowInfo.Title = "HazardEditor | " + RenderAPIToString(renderAPI);
	windowInfo.FullScreen = false;
	windowInfo.Maximized = false;
	windowInfo.HasTitlebar = true;

#ifdef HZR_PLATFORM_MACOS
	windowInfo.Width = 2880;
	windowInfo.Height = 1600;
#else
	windowInfo.Width = 1920;
	windowInfo.Height = 1080;
#endif
	windowInfo.Color = Color(34, 34, 34, 255);

	HazardRendererCreateInfo renderInfo = {};
	renderInfo.pAppInfo = &rendererApp;
	renderInfo.Renderer = renderAPI;
	renderInfo.VSync = CommandLineArgs::Get<bool>("VSync");
	renderInfo.WindowCount = 1;
	renderInfo.pWindows = &windowInfo;

	EntityComponentCreateInfo entity = {};
	entity.StartupFile = ProjectManager::GetProject().GetProjectData().StartupWorld;

	ScriptEngineCreateInfo scriptEngine = {};

	std::string dllFile = project.GetProjectData().ProjectName + ".dll";
	std::filesystem::path appAssemblyPath = project.GetProjectData().ProjectDirectory / "Library" / "Scripts" / "Binaries" / dllFile;
	std::cout << "App assembly at " << appAssemblyPath << std::endl;

	scriptEngine.CoreAssemblyPath = "C:/dev/Hazard/HazardScripting/bin/Debug/HazardScripting.dll";
	scriptEngine.AppAssemblyPath = appAssemblyPath.string();
	scriptEngine.AssemblyPath = "C:/Program Files/Mono/lib";
	scriptEngine.ConfigPath = "C:/Program Files/Mono/etc/";

	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RendererInfo = &renderInfo;
	createInfo.EntityComponent = &entity;
	createInfo.ScriptEngineInfo = &scriptEngine;

	HZR_WARN("Initializing");
	CreateApplicationStack(&createInfo);

	GetModule<ScriptEngine>().RegisterScriptGlue<Editor::EditorScriptGlue>();
}
void EditorApplication::Init()
{
	auto& manager = PushModule<GUIManager>();
	auto& window = GetModule<RenderEngine>().GetWindow();
	auto& scriptEngine = GetModule<ScriptEngine>();

	window.SetDebugCallback([](RenderMessage message) {
		auto manager = Application::GetModule<GUIManager>();
		auto console = manager.GetPanelManager().GetRenderable<UI::Console>();
		if (!console) return;

		uint32_t messageFlags = GetMessageFlagsFromSeverity(message.Severity);
		console->AddMessage({ message.Description, message.StackTrace, messageFlags });
		});

	scriptEngine.SetDebugCallback([](ScriptMessage message) {
		auto& manager = Application::GetModule<GUIManager>();
		auto console = manager.GetPanelManager().GetRenderable<UI::Console>();
		if (!console) return;

		uint32_t messageFlags = GetMessageFlagsFromSeverity(message.Severity);
		console->AddMessage({ message.Message, message.StackTrace, messageFlags });
		});
	scriptEngine.ReloadAssemblies();
}

bool EditorApplication::OnEvent(Event& e)
{
	return false;
}
