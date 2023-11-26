
#include "HazardEditor.h"
#include "Hazard/Core/EntryPoint.h"

#include "HazardRendererCore.h"
#include "Project/ProjectManager.h"
#include "GUIManager.h"
#include "GUI/Debug/Console.h"
#include "HazardScript.h"
#include "Editor/EditorWorldManager.h"

#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "EditorScripting/EditorScriptGlue.h"

#include "EditorAssetManager.h"
#include "spdlog/fmt/fmt.h"

#include "MessageFlags.h"

using namespace Hazard;
using namespace HazardScript;

static std::string GetBuildType()
{
#ifdef HZR_DEBUG
	return "Debug";
#elif HZR_DIST
	return "Distribution";
#elif HZR_RELEASE
	return "Release";
#else
	return "Unknown";
#endif
}


void HazardEditorApplication::PreInit()
{
	std::string workingDir = CommandLineArgs::Get<std::string>("wdir");
	if (!workingDir.empty())
	{
		std::filesystem::current_path(workingDir);
		HZR_INFO("Working directory: {0} ", std::filesystem::current_path().string());
	}
    
    auto projectPath = CommandLineArgs::Get<std::string>("hprj");
	HazardProject project = PushModule<ProjectManager>().LoadProjectFromFile(projectPath);

	std::vector<const char*> icons = { "res/Icons/logo.png", "res/Icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.AppName = "Hazard Editor";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
#ifdef HZR_RELEASE
	appInfo.Logging = false;
#else
	appInfo.Logging = true;
#endif

	RenderContextCreateInfo renderContextInfo = {};
	renderContextInfo.Renderer = RenderAPI::Auto;
	renderContextInfo.VSync = CommandLineArgs::Get<bool>("VSync");
	renderContextInfo.Title = "Hazard Editor | " + RenderAPIToString(renderContextInfo.Renderer) + " | " + GetBuildType();
	renderContextInfo.Width = 1920;
	renderContextInfo.Height = 1080;

	RendererCreateInfo rendererInfo = {};

	EntityComponentCreateInfo entity = {};
	entity.StartupFile = "world.hpack";

	std::string dllFile = project.GetInfo().ProjectName + ".dll";
	std::filesystem::path appAssemblyPath = project.GetInfo().ProjectPath / "Library" / "Scripts" / "Binaries" / dllFile;

	ScriptEngineCreateInfo scriptEngine = {};
	scriptEngine.CoreAssemblyPath = "../HazardScripting/bin/Debug/net7.0/HazardScripting.dll";
	scriptEngine.AppAssemblyPath = appAssemblyPath.string();
	scriptEngine.CoralDirectory = "../HazardScripting/bin/Debug/net7.0/";	

	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RenderContextInfo = &renderContextInfo;
	createInfo.RendererInfo = &rendererInfo;
	createInfo.ScriptEngineInfo = &scriptEngine;
	createInfo.EntityComponent = &entity;

	CreateApplicationStack(&createInfo);

	EditorAssetManager::Init();

	auto& engine = GetModule<Hazard::ScriptEngine>();
	Ref<ScriptAssembly> assembly = engine.GetLoadedAssembly("HazardScripting");
	engine.RegisterScriptGlueFor<Editor::Bindings::EditorScriptGlue>(assembly);
}
void HazardEditorApplication::Init()
{
	EditorAssetManager::LoadEditorAssets();

	Editor::EditorWorldManager::Init();
	PushModule<GUIManager>();

	auto& window = GetModule<RenderContextManager>().GetWindow();
	auto& scriptEngine = GetModule<ScriptEngine>();
	window.SetDebugCallback([](const RenderMessage& message) {

		auto& manager = Application::GetModule<GUIManager>();
		auto console = manager.GetPanelManager().GetRenderable<UI::Console>();
		if (!console) return;

		uint32_t messageFlags = GetMessageFlagsFromSeverity(message.Severity);
		console->AddMessage({ message.Description, message.StackTrace, messageFlags });

		std::cout << fmt::format("RenderMessage: {}\n - {}", message.Description, message.StackTrace) << std::endl;
	});

	scriptEngine.SetDebugCallback([](ScriptMessage message) {
		auto& manager = Application::GetModule<GUIManager>();
		auto console = manager.GetPanelManager().GetRenderable<UI::Console>();
		if (!console) return;

		uint32_t messageFlags = GetMessageFlagsFromSeverity(message.Severity);
		console->AddMessage({ message.Message, message.StackTrace, messageFlags });
	});
	scriptEngine.ReloadAssemblies();

	m_ScriptManager.Init();
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
	return hnew HazardEditorApplication();
}
