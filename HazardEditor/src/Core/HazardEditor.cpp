
#include "hzrpch.h"
#include "HazardEditor.h"
#include "Hazard/Core/EntryPoint.h"

#include "HazardRenderer.h"
#include "Project/ProjectManager.h"
#include "GUIManager.h"
#include "GUI/Debug/Console.h"
#include "HazardScript.h"
#include "Editor/EditorWorldManager.h"

#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "EditorScripting/EditorScriptGlue.h"

#include "MessageFlags.h"

using namespace Hazard;
using namespace HazardRenderer;
using namespace HazardScript;


void HazardEditorApplication::PreInit()
{

#if HZR_DEBUG
	std::cout << "Select api" << std::endl;
	std::cout << " 0 - Auto" << std::endl;
	std::cout << " 1 - OpenGL" << std::endl;
	std::cout << " 2 - Vulkan" << std::endl;
	std::cout << " 3 - Metal" << std::endl;
	uint32_t api;
	std::cin >> api;
	RenderAPI renderAPI = (RenderAPI)api;
#else
	RenderAPI renderAPI = RenderAPI::Auto;
#endif

	std::string workingDir = CommandLineArgs::Get<std::string>("wdir");
	if (!workingDir.empty()) 
	{
		std::filesystem::current_path(workingDir);
		std::cout << "Working directory: " << std::filesystem::current_path().string() << std::endl;
	}
	HazardProject& project = PushModule<ProjectManager>().LoadProjectFromFile(CommandLineArgs::Get<std::string>("hprj"));

	std::vector<const char*> icons = { "res/Icons/logo.png", "res/Icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.AppName = "Hazard Editor";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
	appInfo.Logging = true;

	RenderContextCreateInfo renderContextInfo = {};
	renderContextInfo.Renderer = renderAPI;
	renderContextInfo.VSync = false;
	renderContextInfo.Title = "Hazard Editor | " + RenderAPIToString(renderAPI);
	renderContextInfo.Width = 1920;
	renderContextInfo.Height = 1080;

	RendererCreateInfo rendererInfo = {};
	

	EntityComponentCreateInfo entity = {};
	entity.StartupFile = ProjectManager::GetProject().GetProjectData().StartupWorld;

	std::string dllFile = project.GetProjectData().ProjectName + ".dll";
	std::filesystem::path appAssemblyPath = project.GetProjectData().ProjectDirectory / "Library" / "Scripts" / "Binaries" / dllFile;

	ScriptEngineCreateInfo scriptEngine = {};
	scriptEngine.CoreAssemblyPath = "C:/dev/Hazard/HazardScripting/bin/Debug/HazardScripting.dll";
	scriptEngine.AppAssemblyPath = appAssemblyPath.string();
	scriptEngine.AssemblyPath = "C:/Program Files/Mono/lib";
	scriptEngine.ConfigPath = "C:/Program Files/Mono/etc/";

	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RenderContextInfo = &renderContextInfo;
	createInfo.RendererInfo = &rendererInfo;
	createInfo.ScriptEngineInfo = &scriptEngine;
	createInfo.EntityComponent = &entity;

	CreateApplicationStack(&createInfo);

	GetModule<ScriptEngine>().RegisterScriptGlue<Editor::EditorScriptGlue>();
}
void HazardEditorApplication::Init()
{
	Editor::EditorWorldManager::Init();
	auto& manager = PushModule<GUIManager>();
	auto& window = GetModule<RenderContextManager>().GetWindow();
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

void HazardEditorApplication::Update()
{
	HZR_PROFILE_FUNCTION();
	Editor::EditorWorldManager::Update();
}

bool HazardEditorApplication::OnEvent(Event& e)
{
	return m_ScriptManager.OnEvent(e);
}

Hazard::Application* Hazard::CreateApplication()
{
	return new HazardEditorApplication();
}