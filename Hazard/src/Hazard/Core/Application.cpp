#pragma once

#include <hzrpch.h>
#include "Application.h"
#include "Hazard/RenderContext/RenderContext.h"
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
		auto& context = GetModule<Rendering::RenderContext>();
		context.GetWindow().SetWindowTitle(title);
	}
	void Application::CreateApplicationStack(HazardCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		if (info->appInfo == nullptr)
			HZR_THROW("[Hazard]: ApplicationCreateInfo required");

#ifndef HZR_RELEASE
			PushModule<Logging::Logger>();
#endif // HZR_RELEASE
		

		if (info->renderContextInfo != nullptr) 
		{
			PushModule<Rendering::RenderContext>().InitContext(info->renderContextInfo, info->appInfo);
		}

		if (info->rendererInfo != nullptr) 
		{
			if (info->renderContextInfo == nullptr) 
				HZR_THROW("[Hazard Renderer]: Using renderer requires RenderContextCreateInfo");

			PushModule<Rendering::RenderEngine>().InitRenderer(info->rendererInfo);
		}

		if (info->audioEngine) 
		{
		
			PushModule<Audio::AudioEngine>().InitAudio(info->audioEngine);
		}

		if (info->scriptEngineInfo != nullptr) 
		{
			PushModule<Scripting::ScriptEngineManager>().InitEngines(info->scriptEngineInfo);
		}

		if (info->entityComponent != nullptr) {
			ECS::WorldHandler& handler = PushModule<ECS::WorldHandler>();
			handler.LoadWorld(info->entityComponent->startupFile);
		}
	}
	void Application::Quit()
	{
		Core::HazardLoop::GetCurrent().Shutdown();
	}
	void Application::UpdateData()
	{
		PROCESS_MEMORY_COUNTERS_EX pmc;
		GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
		s_Data.memoryUsage = pmc.PrivateUsage / 1048576.0f;
	}
}
