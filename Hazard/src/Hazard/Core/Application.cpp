#pragma once

#include <hzrpch.h>
#include "Application.h"
#include "Hazard/RenderContext/RenderContext.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Scripting/ScriptEngineManager.h"
#include "Hazard/Audio/AudioEngine.h"
#include "Hazard/Entity/WorldCommand.h"
#include "HazardLoop.h"
#include "Hazard/Entity/WorldHandler.h"

namespace Hazard {

	void Application::SetTitle(const std::string& title)
	{
		SetTitle(title.c_str());
	}
	void Application::SetTitle(const char* title)
	{
		auto* context = GetModule<Rendering::RenderContext>();
		context->GetWindow().SetWindowTitle(title);
	}
	void Application::CreateApplicationStack(HazardCreateInfo* info)
	{
		HZR_PROFILE_FUNCTION();
		if (info->appInfo == nullptr)
			HZR_THROW("[Hazard]: ApplicationCreateInfo required");

#ifndef HZR_RELEASE
			PushModule<Logging::Logger>();
#endif // HZR_RELEASE
		

		if (info->renderContextInfo != nullptr) {
			PushModule<Rendering::RenderContext>().InitContext(info->renderContextInfo, info->appInfo);
		}
		if (info->rendererInfo != nullptr) {
			if (info->renderContextInfo == nullptr) HZR_THROW("[Hazard Renderer]: Using renderer requires RenderContextCreateInfo");
			PushModule<Rendering::RenderEngine>().InitRenderer(info->rendererInfo);
		}
		if (info->audioEngine) {
		
			PushModule<Audio::AudioEngine>().InitAudio(info->audioEngine);
		}
		if (info->scriptEngineInfo != nullptr) {
			PushModule<Scripting::ScriptEngineManager>().InitEngines(info->scriptEngineInfo);
		}
		if (info->entityComponent != nullptr) {
			ECS::WorldHandler& handler = PushModule<ECS::WorldHandler>();
			if (info->entityComponent->startupFile != "") {
				handler.LoadScene(info->entityComponent->startupFile);
			}
		}
	}
	void Application::Quit()
	{
		Core::HazardLoop::GetCurrent().Shutdown();
	}
}
