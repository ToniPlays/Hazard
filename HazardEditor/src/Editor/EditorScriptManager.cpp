
#include "EditorScriptManager.h"
#include "Project/ProjectManager.h"

#include "Hazard.h"
#include "File.h"
#include "Core/MessageFlags.h"
#include "Core/GUIManager.h"

namespace Editor
{
	void EditorScriptManager::GenerateProjectFiles()
	{
		auto& project = ProjectManager::GetProject().GetProjectData();
		std::filesystem::path genProjectPath = project.ProjectDirectory / "Library";
		std::string command = (genProjectPath / "Win-CreateScriptProject.bat").string();
		HZR_INFO(command);
		File::SystemCall(command);
		HZR_INFO("Generated project files");
	}
	void EditorScriptManager::CompileSource()
	{
		auto& project = ProjectManager::GetProject().GetProjectData();
		std::filesystem::path buildPath = project.ProjectDirectory / "Library";
		std::string command = (buildPath / "BuildSolution.bat").string() + " > " + buildPath.string() + "/build.hlog";
		File::SystemCall(command);
		HZR_INFO("Compiled source files");

		auto& manager = Hazard::Application::GetModule<GUIManager>();
		auto console = manager.GetPanelManager().GetRenderable<UI::Console>();
		if (!console) return;

		std::vector<UI::ConsoleMessage> messages = ParseBuildResult(File::ReadFile(buildPath / "build.hlog"));

		for (auto& message : messages)
			console->AddMessage(message);
	}
	void EditorScriptManager::ReloadAssebmly()
	{
		Hazard::Application::GetModule<Hazard::ScriptEngine>().ReloadAssemblies();
	}
	void EditorScriptManager::RecompileAndLoad()
	{
		auto console = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<UI::Console>();
		if (console->ClearOnBuild()) {
			console->Clear(true);
		}

		GenerateProjectFiles();
		CompileSource();
		ReloadAssebmly();
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
