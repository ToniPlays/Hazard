
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
#include "Backend/Core/ShaderCompiler.h"
#include "Hazard/RenderContext/TextureFactory.h"
#include "Hazard/Rendering/Mesh/MeshFactory.h"

#include "EditorAssetManager.h"

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

#if HZR_DEBUG && false
	std::cout << "Select api" << std::endl;
	std::cout << " 0 - Auto" << std::endl;
	std::cout << " 1 - OpenGL" << std::endl;
	std::cout << " 2 - Vulkan" << std::endl;
	std::cout << " 3 - Metal" << std::endl;
	uint32_t api;
	std::cin >> api;
	RenderAPI renderAPI = (RenderAPI)api;
#else
	RenderAPI renderAPI = RenderAPI::Vulkan;
#endif

	std::string workingDir = CommandLineArgs::Get<std::string>("wdir");
	if (!workingDir.empty())
	{
		std::filesystem::current_path(workingDir);
		HZR_INFO("Working directory: {0} ", std::filesystem::current_path().string());
	}
	HazardProject& project = PushModule<ProjectManager>().LoadProjectFromFile(CommandLineArgs::Get<std::string>("hproj"));

	std::vector<const char*> icons = { "res/Icons/logo.png", "res/Icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.AppName = "Hazard Editor";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
	appInfo.Logging = true;

	RenderContextCreateInfo renderContextInfo = {};
	renderContextInfo.Renderer = renderAPI;
	renderContextInfo.VSync = CommandLineArgs::Get<bool>("VSync");
	renderContextInfo.Title = "Hazard Editor | " + RenderAPIToString(renderAPI) + " | " + GetBuildType();
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


	EditorAssetManager::ImportAssets();
	ShaderCompiler::SetCacheLocation("Library/Shaders");
	TextureFactory::SetCacheLocation("Library/Textures");
	MeshFactory::SetCacheLocation("Library/Meshes/");

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
		switch (message.Severity)
		{
		case Severity::Debug:		HZR_INFO("Debug: {0} -> {1}",		message.Description, message.StackTrace); break;
		case Severity::Trace:		HZR_TRACE("Trace: {0} -> {1}",		message.Description, message.StackTrace); break;
		case Severity::Info:		HZR_INFO("Info: {0} -> {1}",		message.Description, message.StackTrace); break;
		case Severity::Warning:		HZR_WARN("Warning: {0} -> {1}",		message.Description, message.StackTrace); break;
		case Severity::Error:		HZR_ERROR("Error: {0} -> {1}",		message.Description, message.StackTrace); break;
		case Severity::Critical:	HZR_ERROR("Critical: {0} -> {1}",	message.Description, message.StackTrace); break;
		}

		auto& manager = Application::GetModule<GUIManager>();
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

	EditorAssetManager::Init();
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