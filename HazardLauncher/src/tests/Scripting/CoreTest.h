#pragma once


#include "HazardScript.h"
#include <iostream>

using namespace HazardScript;

namespace ScriptCore 
{
	static void Quit() {
		std::cout << "Application Quit" << std::endl;
	}

	static void Run()
	{
		std::cout << "Running script core test" << std::endl;

		HazardScriptCreateInfo info = {};
		info.CoreAssemblyPath = "C:/dev/Hazard/HazardScripting/bin/Debug/HazardScripting.dll";
		info.AppAssemblyPath = "C:/dev/HazardCraft/Assets/Scripts/Binaries/HazardCraft.dll";
		info.AssemblyPath = "C:/Program Files/Mono/lib";
		info.ConfigPath = "C:/Program Files/Mono/etc/";

		info.DebugCallback = [&](ScriptMessage message) {
			std::cout << message.Message << std::endl;
		};

		info.BindingCallback = [&]() {
			std::cout << "Reloading bindings" << std::endl;
			Mono::Register("Hazard.InternalCalls::Application_Quit_Native", Quit);
		};

		HazardScriptEngine* engine = HazardScriptEngine::Create(&info);
		ScriptAssembly& assembly = engine->GetAppAssembly();

		if (assembly.HasScript("Spinner")) {
			
			ScriptObject* obj = assembly.GetScript("Spinner").CreateObject();
			void* params[] = { 0 };
			obj->Invoke("OnUpdate", params);
		}

		std::cin.get();
	}
}
