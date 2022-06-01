
#include <hzrpch.h>
#include "Application.h"
#include "HazardLoop.h"

#ifdef HZR_PLATFORM_WINDOWS
#include <Windows.h>
#include <Psapi.h>
#endif

namespace Hazard {

	ApplicationData Application::s_Data;

	void Application::SetTitle(const std::string& title)
	{
		SetTitle(title.c_str());
	}
	void Application::SetTitle(const char* title)
	{
		//Rendering::RenderContextCommand::SetTitle(title);
	}
	void Application::CreateApplicationStack(HazardCreateInfo* info)
	{
		
#ifndef HZR_RELEASE
		PushModule<Logging::Logger>();
#endif // HZR_RELEASE
		/*

		if (info->AppInfo == nullptr)
			HZR_THROW("[Hazard]: ApplicationCreateInfo required");

		if (info->EntityComponent != nullptr)
			PushModule<ECS::WorldHandler>(info->EntityComponent);

		if (info->RenderContextInfo != nullptr)
			PushModule<Rendering::RenderContext>(info->RenderContextInfo, info->AppInfo);

		if (info->RendererInfo != nullptr)
		{
			HZR_CORE_ASSERT(info->RenderContextInfo, "[Hazard Renderer]: Using renderer requires RenderContextCreateInfo");
			PushModule<Rendering::RenderEngine>(info->RendererInfo);
		}
		if (info->ScriptEngineInfo != nullptr)
			PushModule<Scripting::ScriptEngineManager>(info->ScriptEngineInfo);

		if (info->AudioEngine)
			PushModule<Audio::AudioEngine>(info->AudioEngine);
		*/
	}
	void Application::Quit()
	{
#ifdef HZR_PLATFORM_WINDOWS
		Core::HazardLoop::GetCurrent().m_ShouldClose = true;
#endif
	}
	void Application::UpdateData()
	{
#ifdef HZR_PLATFORM_WINDOWS
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		s_Data.MemoryUsage = pmc.PrivateUsage / 1048576.0f;
#else
		// TODO: Implement on other platforms using RLimit etc.
		s_Data.MemoryUsage = 0.0f;
#endif
	}
}

