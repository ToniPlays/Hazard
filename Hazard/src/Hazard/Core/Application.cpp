
#include <hzrpch.h>
#include "Application.h"
#include "HazardLoop.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/ECS/WorldHandler.h"

#ifdef HZR_PLATFORM_WINDOWS
#include <Windows.h>
#include <Psapi.h>
#endif

namespace Hazard {

	ApplicationData Application::s_Data;

	void Application::CreateApplicationStack(HazardCreateInfo* info)
	{
		
		HZR_ASSERT(info->AppInfo, "[Hazard]: ApplicationCreateInfo required");

		if (info->RendererInfo) 
		{
			PushModule<RenderEngine>(info->RendererInfo);
		}
		if (info->ScriptEngineInfo)
			PushModule<ScriptEngine>(info->ScriptEngineInfo);
		if(info->EntityComponent)
			PushModule<WorldHandler>(info->EntityComponent);
	}
	void Application::Quit()
	{
		HazardLoop::GetCurrent().m_ShouldClose = true;
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

