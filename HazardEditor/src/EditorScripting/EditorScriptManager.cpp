#include "EditorScriptManager.h"
#include "Platform/OS.h"
#include "Project/ProjectManager.h"
#include "Hazard.h"
#include "Hazard/ImGUI/GUIManager.h"
#include "Editor/GUI/Debug/Console.h"
#include "Core/MessageFlags.h"

void EditorScriptManager::GenerateScriptProjects()
{
    //Run Premake scripts right here
    auto premakePath = std::filesystem::current_path().parent_path() / "vendor" / "premake" / "bin" / "premake5";
    if(!File::Exists(premakePath))
    {
        HZR_CORE_WARN("Premake not found, cannot generate projects");
        return;
    }
    
    auto& manager = Hazard::Application::Get().GetModule<ProjectManager>();
    auto path = manager.GetCurrentProject().GetProjectDirectory();
    
    auto cmd = fmt::format("vs2022 --file={}/premake5.lua", path.string());
    
    OS::BackgroundProcess(premakePath.string().c_str(), cmd.c_str());
    
}
void EditorScriptManager::RecompileScripts()
{
    using namespace Hazard;
    auto& project = Application::Get().GetModule<ProjectManager>().GetCurrentProject();
    auto result = OS::BackgroundProcess("/usr/local/share/dotnet/dotnet", fmt::format("build {0}/{1}.sln", project.GetProjectDirectory().string(),  File::GetName(project.GetProjectDirectory())).c_str());
    
    UI::Console& console = Application::Get().GetModule<GUIManager>().GetExistingOrNew<UI::Console>();
    console.AddMessage({ "Build report", result, MessageFlags_Info | MessageFlags_Clearable });
    
    Application::Get().GetModule<ScriptEngine>().ReloadAssemblies();
}
