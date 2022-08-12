
#include "EditorScriptManager.h"
#include "Project/ProjectManager.h"

#include "Hazard.h"
#include "File.h"
#include "Core/MessageFlags.h"
#include "Core/GUIManager.h"
#include "GUI/Debug/Console.h"

namespace Editor
{
	void EditorScriptManager::GenerateProjectFiles()
	{
		auto& project = ProjectManager::GetProject().GetProjectData();
		std::filesystem::path genProjectPath = project.ProjectDirectory / "Library";
		std::string command = (genProjectPath / "Win-CreateScriptProject.bat").string() + " > " + genProjectPath.string() + "/build.hlog";
		File::SystemCall(command);
	}
	void EditorScriptManager::CompileSource()
	{
		auto& project = ProjectManager::GetProject().GetProjectData();
		std::filesystem::path buildPath = project.ProjectDirectory / "Library";
		std::string command = (buildPath / "BuildSolution.bat").string() + " > " + buildPath.string() + "/build.hlog";
		File::SystemCall(command);

		auto& manager = Hazard::Application::GetModule<GUIManager>();
		auto console = manager.GetPanelManager().GetRenderable<UI::Console>();
		if (!console) return;

		console->AddMessage({ "Build result", File::ReadFile(buildPath / "build.hlog"), MessageFlags_Info });
	}
	void EditorScriptManager::ReloadAssebmly()
	{
		Hazard::Application::GetModule<Hazard::ScriptEngine>().ReloadAssemblies();
	}
	void EditorScriptManager::RecompileAndLoad()
	{
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
}
