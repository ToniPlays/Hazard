
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


void HazardEditorApplication::PreInit()
{
	std::string workingDir = CommandLineArgs::Get<std::string>("wdir");
	if (!workingDir.empty())
	{
		std::filesystem::current_path(workingDir);
		HZR_INFO("Working directory: {0} ", std::filesystem::current_path().string());
	}

	std::filesystem::path projectPath = CommandLineArgs::Get<std::filesystem::path>("hprj");

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

	std::string dllFile = File::GetName(projectPath) + ".dll";
	std::filesystem::path coreAssemblyPath = projectPath / "Library" / "Scripts" / "Binaries" / "HazardScripting.dll";
	std::filesystem::path appAssemblyPath = projectPath / "Library" / "Scripts" / "Binaries" / dllFile;

	ScriptEngineCreateInfo scriptEngine = {};
	scriptEngine.CoreAssemblyPath = coreAssemblyPath.string();
	scriptEngine.AppAssemblyPath = appAssemblyPath.string();
	scriptEngine.CoralDirectory = (projectPath.parent_path() / "Library" / "Scripts" / "Binaries").string();

	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RenderContextInfo = &renderContextInfo;
	createInfo.RendererInfo = &rendererInfo;
	createInfo.ScriptEngineInfo = &scriptEngine;
	createInfo.EntityComponent = &entity;

	CreateApplicationStack(&createInfo);
	InitDefaultHooks();

	HazardProject project = PushModule<ProjectManager>().LoadProjectFromFile(projectPath);
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

	auto& scriptEngine = GetModule<ScriptEngine>();
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

void HazardEditorApplication::InitDefaultHooks()
{
	JobSystem& system = Application::Get().GetJobSystem();
	system.Hook(JobSystem::Failure, [](Ref<JobGraph> graph) {
		HZR_ERROR("Job graph {} has failed at {}", graph->GetName(), graph->GetCurrentStageInfo().Name);
	});

	system.Hook(JobSystem::Message, [](Severity severity, const std::string& message) {
		if (severity == Severity::Error)
			HZR_ERROR("Message: {}", message);
	});

	system.Hook(JobSystem::Status, [](Ref<Thread> thread, ThreadStatus status) {
		switch (status)
		{
			case ThreadStatus::Failed:
			{
				HZR_ERROR("Thread {} failed: {}", thread->GetThreadID(), thread->GetCurrentJob()->GetName());
			}
			default: break;
		}
	});
}

Hazard::Application* Hazard::CreateApplication()
{
	return hnew HazardEditorApplication();
}
