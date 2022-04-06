#pragma once


#include "HazardScript.h"
#include <iostream>

using namespace HazardScript;

namespace ScriptCore 
{
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

		HazardScriptEngine* engine = HazardScriptEngine::Create(&info);
		

		std::cin.get();
	}
}
