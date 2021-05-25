#pragma once

#include "playerpch.h"
#include "HazardPlayer.h"

HazardPlayer::HazardPlayer()
{

}

HazardPlayer::~HazardPlayer()
{

}

void HazardPlayer::PreInit()
{
	HZR_PROFILE_FUNCTION();
	std::vector<std::string> icons = { "res/icons/logo.png", "res/icons/logo.png" };

	ApplicationCreateInfo appInfo;
	appInfo.appName = "Hazard Runtime Game";
	appInfo.buildVersion = HZR_BUILD_VERSION;
	appInfo.logging = true;
	appInfo.iconCount = icons.size();
	appInfo.icons = icons.data();

	RenderContexCreateInfo contextInfo;
	contextInfo.renderer = RenderAPI::OpenGL;
	contextInfo.fullScreen = true;
	contextInfo.color = { 1, 1, 1 };
	contextInfo.VSync = true;
	contextInfo.width = 1280;
	contextInfo.height = 720;

	RenderEngineCreateInfo engineInfo;
	engineInfo.maxQuadCount = 50000;
	engineInfo.samplerCount = 8;
	engineInfo.shaderSourcePath = "res/shaders";
	engineInfo.shaderCompilePath = "res/shaders";

	AudioEngineCreateInfo audioInfo;

	ScriptEngineCreateInfo scriptInfo;
	scriptInfo.appAssemblyPath = "data/HazardProject.dll";
	scriptInfo.coreAssemblyPath = "data/HazardScripting.dll";
	scriptInfo.monoDirectory = "data/mono";
	scriptInfo.enable = true;

	EntityComponentCreateInfo entityComponent;
	entityComponent.startupFile = "res/worlds/testScene.hazard";

	HazardCreateInfo createInfo;
	createInfo.appInfo = &appInfo;
	createInfo.renderContextInfo = &contextInfo;
	createInfo.rendererInfo = &engineInfo;
	createInfo.audioEngine = &audioInfo;
	createInfo.scriptEngineInfo = &scriptInfo;
	createInfo.entityComponent = &entityComponent;

	CreateApplicationStack(&createInfo);
}
void HazardPlayer::Init()
{
	Scripting::ScriptCommand::OnBeginRuntime();

}
void HazardPlayer::Update()
{
	auto[found, cam, transform] = ECS::WorldCommand::GetCurrentWorld().GetWorldCamera();

	Rendering::RenderCommand::SetFrameBuffer(nullptr);
	ECS::WorldCommand::RenderScene(Rendering::Camera(cam->GetProjection(), transform->GetTransformNoScale(),
		transform->m_Translation));
}

bool HazardPlayer::OnEvent(Event& e)
{
	return false;
}



