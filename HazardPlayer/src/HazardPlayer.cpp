#pragma once

#include "playerpch.h"
#include "HazardPlayer.h"

void HazardPlayer::PreInit()
{
	HZR_PROFILE_FUNCTION();
	std::vector<std::string> icons = { "res/icons/logo.png", "res/icons/logo.png" };

	ApplicationCreateInfo appInfo = {};
	appInfo.AppName = "Hazard Runtime Game";
	appInfo.BuildVersion = HZR_BUILD_VERSION;
	appInfo.Logging = true;
	appInfo.IconCount = icons.size();
	appInfo.Icons = icons.data();

	RenderContexCreateInfo contextInfo = {};
	contextInfo.Renderer = RenderAPI::OpenGL;
	contextInfo.FullScreen = false;
	contextInfo.Maximized = true;
	contextInfo.Color = Color::FromHex("#323232");
	contextInfo.VSync = true;
	contextInfo.Width = 1280;
	contextInfo.Height = 720;

	RenderEngineCreateInfo engineInfo = {};
	engineInfo.MaxQuadCount = 25000;
	engineInfo.SamplerCount = 8;
	engineInfo.ShaderSourcePath = "res/shaders";
	engineInfo.ShaderCompilePath = "res/shaders";

	AudioEngineCreateInfo audioInfo = {};

	ScriptEngineCreateInfo scriptInfo = {};
	scriptInfo.AppAssemblyPath = "data/HazardProject.dll";
	scriptInfo.CoreAssemblyPath = "data/HazardScripting.dll";
	scriptInfo.MonoDirectory = "data/mono";
	scriptInfo.Enable = true;

	EntityComponentCreateInfo entityComponent = {};
	entityComponent.StartupFile = "C:/dev/HazardProject/assets/world.hazard";

	HazardCreateInfo createInfo = {};
	createInfo.AppInfo = &appInfo;
	createInfo.RenderContextInfo = &contextInfo;
	createInfo.RendererInfo = &engineInfo;
	createInfo.AudioEngine = &audioInfo;
	createInfo.ScriptEngineInfo = &scriptInfo;
	createInfo.EntityComponent = &entityComponent;

	CreateApplicationStack(&createInfo);
}
void HazardPlayer::Init()
{
	Scripting::ScriptCommand::OnBeginRuntime();
}
void HazardPlayer::Update()
{

}

bool HazardPlayer::OnEvent(Event& e)
{
	return false;
}



