#pragma once

#include <hzrpch.h>
#include "Application.h"
#include "Hazard/RenderContext/RenderContextCommand.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Scripting/ScriptEngineManager.h"
#include "Hazard/Audio/AudioEngine.h"
#include "HazardLoop.h"
#include "Hazard/Entity/WorldHandler.h"

#include <Windows.h>
#include <Psapi.h>

namespace Hazard {

	ApplicationData Application::s_Data;

	void Application::SetTitle(const std::string& title)
	{
		SetTitle(title.c_str());
	}
	void Application::SetTitle(const char* title)
	{
		Rendering::RenderContextCommand::SetTitle(title);
	}
	void Application::CreateApplicationStack(HazardCreateInfo* info)
	{
#ifndef HZR_RELEASE
		PushModule<Logging::Logger>();
#endif // HZR_RELEASE

		if (info->AppInfo == nullptr)
			HZR_THROW("[Hazard]: ApplicationCreateInfo required");

		if (info->RenderContextInfo != nullptr) 
			PushModule<Rendering::RenderContext>(info->RenderContextInfo, info->AppInfo);

		if (info->RendererInfo != nullptr) 
		{
			HZR_CORE_ASSERT(info->RenderContextInfo, "[Hazard Renderer]: Using renderer requires RenderContextCreateInfo");
			PushModule<Rendering::RenderEngine>(info->RendererInfo, info->RenderContextInfo->Renderer);
		}

		if (info->AudioEngine) 
			PushModule<Audio::AudioEngine>(info->AudioEngine);

		if (info->ScriptEngineInfo != nullptr) 
			PushModule<Scripting::ScriptEngineManager>(info->ScriptEngineInfo);

		if (info->EntityComponent != nullptr)
			PushModule<ECS::WorldHandler>(info->EntityComponent);
	}
	void Application::Quit()
	{
		Core::HazardLoop::GetCurrent().m_ShouldClose = true;
	}
	void Application::UpdateData()
	{
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		s_Data.memoryUsage = pmc.PrivateUsage / 1048576.0f;
	}
}
