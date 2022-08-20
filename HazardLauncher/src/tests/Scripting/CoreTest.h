#pragma once
#if 0

#include "HazardScript.h"
#include <iostream>

using namespace HazardScript;

namespace ScriptCore 
{
	static void Quit() {
		std::cout << "Application Quit" << std::endl;
	}
	static void Log(MonoObject* obj) {
		std::cout << Mono::MonoObjectToString(obj) << std::endl;
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
			Mono::Register("Hazard.InternalCalls::Application_Quit_Native", (void*)Quit);
			Mono::Register("Hazard.Console::Console_Log_Native", (void*)Log);
		};

		HazardScriptEngine* engine = HazardScriptEngine::Create(&info);
		ScriptAssembly& assembly = engine->GetAppAssembly();

		if (assembly.HasScript("Spinner")) {
			
			ScriptMetadata& script = assembly.GetScript("Spinner");

			Ref<ScriptObject> obj = script.CreateObject();
			std::cout << obj->GetFieldValue<float>("value") << std::endl;

			obj->SetFieldValue("value", 20.0f);

			float delta = 1.02f;
			void* params[] = { &delta };
			obj->Invoke("OnUpdate(single)", params);

			std::cout << obj->GetFieldValue<float>("value") << std::endl;
		}
		std::cin.get();
	}
}
#endif