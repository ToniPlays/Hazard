
#include <hzrpch.h>
#include "Application.h"
#include "HazardLoop.h"
#include "Hazard/Input/InputManager.h"
#include "Hazard/RenderContext/RenderContextManager.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/ECS/WorldHandler.h"

#ifdef HZR_PLATFORM_WINDOWS
#include <Windows.h>
#include <Psapi.h>
#endif

namespace Hazard 
{
	void Application::CreateApplicationStack(HazardCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(info->AppInfo, "[Hazard]: ApplicationCreateInfo required");

		m_JobSystem = CreateScope<JobSystem>(info->AppInfo->MaxWorkers);

		if (info->ScriptEngineInfo)
			PushModule<ScriptEngine>(info->ScriptEngineInfo);

		if (info->EntityComponent)
			PushModule<WorldHandler>(info->EntityComponent);

		if (info->RenderContextInfo)
			PushModule<RenderContextManager>(info->AppInfo, info->RenderContextInfo);

		if (info->RendererInfo)
			PushModule<RenderEngine>(info->RendererInfo);

		PushModule<InputManager>();
	}
	void Application::Quit()
	{
		HazardLoop::GetCurrent().m_ShouldClose = true;
	}
}

