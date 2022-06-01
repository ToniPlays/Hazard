#pragma once


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
			Mono::Register("Hazard.InternalCalls::Application_Quit_Native", Quit);
			Mono::Register("Hazard.Console::Console_Log_Native", Log);
		};

		HazardScriptEngine* engine = HazardScriptEngine::Create(&info);
		ScriptAssembly& assembly = engine->GetAppAssembly();

		if (assembly.HasScript("Spinner")) {
			
			Script& script = assembly.GetScript("Spinner");

			if (script.Has<AuthorAttribute>()) {
				const AuthorAttribute& attrib = script.Get<AuthorAttribute>();
				std::cout << attrib.Author << std::endl;
			}
			if (script.HasMethod("OnUpdate(single)")) {
				Method& method = script.GetMethod("OnUpdate(single)");
				if (method.Has<VisualFuncAttribute>()) {
					std::cout << "Visual func" << std::endl;
				}
			}
			if (script.HasField("value")) {
				ScriptField field = script.GetField("value");
				if (field.Has<RangeAttribute>()) {
					RangeAttribute range = field.Get<RangeAttribute>();
					std::cout << range.Min << " - " << range.Max << std::endl;
				}
			}


			ScriptObject* obj = script.CreateObject();
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
