
#include "HazardEditor.h"
#include "Hazard/Core/EntryPoint.h"

#include "HazardScript.h"
#include "HazardRendererCore.h"
#include "Project/ProjectManager.h"
#include "Editor/EditorWorldManager.h"

#include "Hazard/RenderContext/RenderContextManager.h"
#include "EditorScripting/EditorScriptGlue.h"

#include "EditorAssetManager.h"
#include "spdlog/fmt/fmt.h"
#include <Hazard/ImGUI/GUIManager.h>

#include "Editor/GUI/MainMenuBar.h"
#include "Editor/GUI/AllPanels.h"

using namespace Hazard;
using namespace HazardScript;

void HazardEditorApplication::PreInit()
{
	std::string workingDir = CommandLineArgs::Get<std::string>("wdir");
	if (!workingDir.empty())
	{
		std::filesystem::current_path(workingDir);
		HZR_INFO("Working directory: {0} ", std::filesystem::current_path().string());
	}

	std::vector<const char*> icons = { "res/Icons/logo.png", "res/Icons/logo.png" };

	ApplicationCreateInfo appInfo = {
		.AppName = "Hazard Editor",
		.BuildVersion = HZR_BUILD_VERSION,
		.MaxWorkers = 24,
	};

#ifdef HZR_RELEASE
	appInfo.Logging = false;
#else
	appInfo.Logging = true;
#endif

	RenderContextCreateInfo renderContextInfo = {
		.Renderer = RenderAPI::Auto,
		.Title = fmt::format("Hazard Editor | {} | {}", RenderAPIToString(RenderAPI::Auto), GetBuildType()),
		.Width = 1920,
		.Height = 1080,
		.VSync = CommandLineArgs::Get<bool>("VSync"),
	};

	RendererCreateInfo rendererInfo = {};
	EntityComponentCreateInfo entity = {};

	std::filesystem::path projectPath = CommandLineArgs::Get<std::filesystem::path>("hprj");
	std::filesystem::path dllFile = File::GetName(projectPath) + ".dll";
	std::filesystem::path coreAssemblyPath = projectPath / "Library" / "Scripts" / "Binaries" / "HazardScripting.dll";
	std::filesystem::path appAssemblyPath = projectPath / "Library" / "Scripts" / "Binaries" / dllFile;

	ScriptEngineCreateInfo scriptEngine = {
		.CoralDirectory = projectPath.parent_path() / "Library" / "Scripts" / "Binaries",
		.CoreAssemblyPath = coreAssemblyPath,
		.AppAssemblyPath = appAssemblyPath
	};

	GuiCreateInfo guiInfo = {
		.Title = "Editor",
	};

	HazardCreateInfo createInfo = {
		.AppInfo = &appInfo,
		.RenderContextInfo = &renderContextInfo,
		.RendererInfo = &rendererInfo,
		.ScriptEngineInfo = &scriptEngine,
		.EntityComponent = &entity,
		.GuiInfo = &guiInfo,
	};


	CreateApplicationStack(&createInfo);
	InitJobsystemHooks();
	EditorAssetManager::Init();

	//TODO: Move
	PushModule<ProjectManager>().LoadProjectFromFile(projectPath);
}

void HazardEditorApplication::Init()
{
	EditorAssetManager::LoadEditorAssets();
	Editor::EditorWorldManager::Init();
	EditorAssetManager::PostInit();

	auto& scriptEngine = GetModule<ScriptEngine>();
	scriptEngine.ReloadAssemblies();

	InitializeGUIPanels();
}

void HazardEditorApplication::Update()
{
	HZR_PROFILE_FUNCTION();
	Editor::EditorWorldManager::Update();
}

bool HazardEditorApplication::OnEvent(Event& e)
{
	return false;
}

void HazardEditorApplication::InitJobsystemHooks()
{
	JobSystem& system = Application::Get().GetJobSystem();
	system.Hook(JobSystem::Failure, [](Ref<JobGraph> graph) {
		HZR_ERROR("Job graph {} has failed at {}", graph->GetName(), graph->GetCurrentStageInfo().Name);
	});

	system.Hook(JobSystem::Message, [](Severity severity, const std::string& message) {
		switch (severity)
		{
			case Severity::Error:
				HZR_ERROR("Error: {}", message);
				break;
		}
	});

	system.Hook(JobSystem::Status, [](Ref<Thread> thread, ThreadStatus status) {
		switch (status)
		{
			case ThreadStatus::Failed:
				HZR_ERROR("Thread {} failed: {}", thread->GetThreadID(), thread->GetCurrentJob()->GetName());
				break;
		}
	});
}

void HazardEditorApplication::InitializeGUIPanels()
{
	auto& manager = GetModule<GUIManager>();
	manager.SetMenubar<UI::MainMenuBar>();
	manager.New<UI::Console>();
	manager.New<UI::AssetPanel>();
	manager.New<UI::GameViewport>();
	manager.New<UI::Viewport>();
	manager.New<UI::Hierarchy>();
	manager.New<UI::Properties>();
	manager.New<UI::ProgressOverlay>();
}

Hazard::Application* Hazard::CreateApplication()
{
	return hnew HazardEditorApplication();
}
