
#include "EditorScriptManager.h"
#include "EditorModeManager.h"
#include "Project/ProjectManager.h"

#include "Hazard.h"
#include "File.h"
#include "Core/MessageFlags.h"
#include "Core/HazardEditor.h"

#include "Platform/OS.h"
#include <Hazard/ImGUI/GUIManager.h>

namespace Editor
{
	static void GenerateProjectFilesJob(JobInfo& info, EditorScriptManager* manager)
	{
		manager->GenerateProjectFiles();
	}
	static void CompileSourcesJob(JobInfo& info, EditorScriptManager* manager)
	{
		manager->CompileSource();
	}
	static void ReloadAssembliesJob(JobInfo& info, EditorScriptManager* manager)
	{
		manager->ReloadAssembly();
	}

	void EditorScriptManager::Init()
	{
		m_Preprocessors.push_back({ "HZR_WINDOWS", "Available on Windows platforms" });
		m_Preprocessors.push_back({ "HZR_MACOS", "Available on MacOS platforms" });
		m_Preprocessors.push_back({ "HZR_LINUX", "Available on Windows platforms" });
		m_Preprocessors.push_back({ "HZR_IOS", "Available on iOS platforms" });
		m_Preprocessors.push_back({ "HZR_ANDROID", "Available on Android platforms" });
		m_Preprocessors.push_back({ "HZR_EDITOR", "Available in the Editor" });
		m_Preprocessors.push_back({ "DEV_BUILD", "Available when Development build is enabled" });
	}

	void EditorScriptManager::GenerateProjectFiles()
	{
		auto& project = ProjectManager::GetProject();
		auto& info = project.GetInfo();
		std::filesystem::path genProjectPath = info.ProjectPath / "Project";
		std::string command = (genProjectPath / "Win-CreateScriptProject.bat").string();

		std::stringstream ss;
		auto& defines = project.GetScriptingSettings().Defines;
		for (uint32_t i = 0; i < defines.size(); i++)
		{
			ss << defines[i];
			if (i < defines.size() - 1 || m_Preprocessors.size() > 0)
				ss << " ";
		}

		for (uint32_t i = 0; i < m_Preprocessors.size(); i++)
		{
			ss << m_Preprocessors[i].Name;
			if (i < m_Preprocessors.size() - 1)
				ss << " ";
		}

		command = fmt::format("{0} --define=\"{1}\"", command, ss.str());
		HZR_INFO("Executing: {0}", command);
		OS::SysCall(command.c_str());
	}
	void EditorScriptManager::CompileSource()
	{
		auto& project = ProjectManager::GetProject().GetInfo();
		std::filesystem::path scriptPath = project.ProjectPath / "Project";
		std::filesystem::path buildPath = project.ProjectPath / "Library";
		std::string command = (scriptPath / "BuildSolution.bat").string() + " > " + buildPath.string() + "/build.hlog";
		OS::SysCall(command.c_str());
		HZR_INFO("Compiled source files");

		auto& manager = Hazard::Application::Get().GetModule<Hazard::GUIManager>();
		auto console = manager.GetRenderable<UI::Console>();
		if (!console) return;

		std::vector<UI::ConsoleMessage> messages = ParseBuildResult(File::ReadFile(buildPath / "build.hlog"));

		for (auto& message : messages)
			console->AddMessage(message);
	}
	void EditorScriptManager::ReloadAssembly()
	{
		EditorModeManager::EndPlayMode();
		Hazard::Application::Get().GetModule<Hazard::ScriptEngine>().ReloadAssemblies();
	}
	void EditorScriptManager::RecompileAndLoad()
	{
		auto console = Application::Get().GetModule<GUIManager>().GetRenderable<UI::Console>();

		if (console->ClearOnBuild())
			console->Clear(true);

		Ref<Job> generateProject = Ref<Job>::Create("Generate C# project", GenerateProjectFilesJob, this);
		Ref<Job> compileSource = Ref<Job>::Create("Compile C#", CompileSourcesJob, this);
		Ref<Job> reloadAssembly = Ref<Job>::Create("Reload", ReloadAssembliesJob, this);

		//Ref<JobGraph> graph = Ref<JobGraph>::Create("Script compilation", 3);
		//graph->GetStage(0)->QueueJobs({ generateProject });
		//graph->GetStage(1)->QueueJobs({ compileSource });
		//graph->GetStage(2)->QueueJobs({ reloadAssembly });

		//Application::Get().GetJobSystem().QueueGraph(graph);
	}
	bool EditorScriptManager::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		return dispatcher.Dispatch<WindowFocusEvent>(BIND_EVENT(EditorScriptManager::OnWindowFocusEvent));
	}
	bool EditorScriptManager::OnWindowFocusEvent(WindowFocusEvent& e)
	{
		if (e.GetFocus())
		{
			if (m_ReloadOnFocus)
			{
				RecompileAndLoad();
			}
			return true;
		}
		return false;
	}
	std::vector<UI::ConsoleMessage> EditorScriptManager::ParseBuildResult(const std::string& source)
	{
		std::vector<UI::ConsoleMessage> result;

		if (StringUtil::Contains(source, "Build succeeded"))
		{
			result.push_back({ "Build succeeded", "", MessageFlags_Debug | MessageFlags_Clearable });
		}
		else if (StringUtil::Contains(source, "Build FAILED."))
		{
			result.push_back({ "BUILD FAILED", "", MessageFlags_Fatal });
		}
		if (StringUtil::Contains(source, "(CoreCompile target) ->"))
		{
			std::string_view message = StringUtil::Between(source, "(CoreCompile target) ->", "Time Elapsed");
			result.push_back({ "Build report", message.data(), MessageFlags_Debug | MessageFlags_Clearable });
		}
		return result;
	}
}
