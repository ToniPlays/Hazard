
#include "EditorScriptManager.h"
#include "EditorModeManager.h"
#include "Project/ProjectManager.h"

#include "Hazard.h"
#include "File.h"
#include "Core/MessageFlags.h"
#include "Core/GUIManager.h"

#include "Core/HazardEditor.h"

namespace Editor
{
	static void GenerateProjectFilesJob(Ref<Job> job, EditorScriptManager* manager)
	{
		manager->GenerateProjectFiles();
	}
	static void CompileSourcesJob(Ref<Job> job, EditorScriptManager* manager)
	{
		manager->CompileSource();
	}
	static void ReloadAssembliesJob(Ref<Job> job, EditorScriptManager* manager)
	{
		std::this_thread::sleep_for(500ms);
		manager->ReloadAssembly();
	}

	void EditorScriptManager::GenerateProjectFiles()
	{
		auto& project = ProjectManager::GetProject();
		auto& info = project.GetInfo();
		std::filesystem::path genProjectPath = info.ProjectPath / "Project";
		std::string command = (genProjectPath / "Win-CreateScriptProject.bat").string();

		std::stringstream ss;
		for (auto& define : project.GetScriptingSettings().Defines)
			ss << define << " ";

		command = fmt::format("{0} --define=\"{1}\"", command, ss.str());
		HZR_INFO("Executing: {0}", command);
		File::SystemCall(command);
	}
	void EditorScriptManager::CompileSource()
	{
		auto& project = ProjectManager::GetProject().GetInfo();
		std::filesystem::path scriptPath = project.ProjectPath / "Project";
		std::filesystem::path buildPath = project.ProjectPath / "Library";
		std::string command = (scriptPath / "BuildSolution.bat").string() + " > " + buildPath.string() + "/build.hlog";
		File::SystemCall(command);
		HZR_INFO("Compiled source files");

		auto& manager = Hazard::Application::GetModule<GUIManager>();
		auto console = manager.GetPanelManager().GetRenderable<UI::Console>();
		if (!console) return;

		std::vector<UI::ConsoleMessage> messages = ParseBuildResult(File::ReadFile(buildPath / "build.hlog"));

		for (auto& message : messages)
			console->AddMessage(message);
	}
	void EditorScriptManager::ReloadAssembly()
	{
		EditorModeManager::EndPlayMode();
		Hazard::Application::GetModule<Hazard::ScriptEngine>().ReloadAssemblies();
	}
	void EditorScriptManager::RecompileAndLoad()
	{
		auto console = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::Console>();

		if (console->ClearOnBuild())
			console->Clear(true);

		Ref<Job> generateProject = Ref<Job>::Create(GenerateProjectFilesJob, this);
		Ref<Job> compileSource = Ref<Job>::Create(CompileSourcesJob, this);
		Ref<Job> reloadAssembly = Ref<Job>::Create(ReloadAssembliesJob, this);

		Ref<JobGraph> graph = Ref<JobGraph>::Create("Script compilation", 3);
		graph->GetStage(0)->QueueJobs({ generateProject }, "Generate C# project");
		graph->GetStage(1)->QueueJobs({ compileSource }, "Compile C#");
		graph->GetStage(2)->QueueJobs({ reloadAssembly }, "Reload");

		Application::Get().GetJobSystem().QueueGraph<bool>(graph);
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

		if (StringUtil::Contains(source, "Build succeeded")) {
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
