
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
		.MaxWorkers = std::thread::hardware_concurrency(),
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
	std::filesystem::path dllFile = File::GetNameNoExt(projectPath.parent_path()) + ".dll";
    std::filesystem::path coreAssemblyPath = std::filesystem::current_path().parent_path() / "HazardScripting" / "bin" / "Debug" / "net8.0" / "HazardScripting.dll";
	std::filesystem::path appAssemblyPath = projectPath.parent_path() / "Library" / "Scripts" / "Binaries" / "net8.0" / dllFile;

	ScriptEngineCreateInfo scriptEngine = {
		.CoralDirectory = std::filesystem::current_path() / "res" / "Scripting" / "Core",
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

void HazardEditorApplication::BeginPlayMode(Ref<Hazard::World> world)
{
    SubmitMainThread([this, world](){
        using namespace Hazard;
        m_EditorWorld = world;
        auto& handler = GetModule<WorldHandler>();
        handler.SetWorld(World::Copy(world));
        handler.SetFlags(WorldFlags_UpdateScript | WorldFlags_Render);
        handler.OnBegin();
        
        auto renderer = Editor::EditorWorldManager::GetWorldRender();
        renderer->SetTargetWorld(handler.GetCurrentWorld());
    });
}

void HazardEditorApplication::EndPlayMode()
{
    SubmitMainThread([this](){
        using namespace Hazard;
        auto& handler = GetModule<WorldHandler>();
        handler.OnEnd();
        handler.SetWorld(m_EditorWorld);
        handler.SetFlags(WorldFlags_Render);
        m_EditorWorld = nullptr;
        
        auto renderer = Editor::EditorWorldManager::GetWorldRender();
        renderer->SetTargetWorld(handler.GetCurrentWorld());
    });
}

void HazardEditorApplication::InitJobsystemHooks()
{
	JobSystem& system = Application::Get().GetJobSystem();

	system.Hook(JobSystemHook::Message, [](Severity severity, const std::string& message) {
        return;
		switch (severity)
		{
			case Severity::Warning:
				HZR_WARN("Warn: {}", message);
				break;
			case Severity::Error:
				HZR_ERROR("Error: {}", message);
				break;
            default: break;
		}
	});

	system.Hook(JobSystemHook::Status, [](Ref<Thread> thread, ThreadStatus status) {
		switch (status)
		{
			case ThreadStatus::Failed:
				HZR_ERROR("Thread {} failed: {} ({})", thread->GetThreadID(), thread->GetCurrentJob()->GetName(), thread->GetLastError());
				break;
            default: break;
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
