#pragma once

#include <hzrpch.h>
#include "ScriptEngine.h"
#include "Hazard/File/File.h"


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>


namespace Hazard::Scripting {


	struct MonoData {
		MonoDomain* mono_domain;
		MonoAssembly* core_assembly;
		MonoImage* core_assembly_image;

		MonoAssembly* app_assembly;
		MonoImage* app_assembly_image;
	};

	MonoData data;
	#pragma region ASSEMBLY

	static MonoAssembly* LoadAssemblyFromFile(const char* path)
	{
		std::vector<char> data = File::File::ReadBinaryFile(path);
		MonoImageOpenStatus status;

		MonoImage* image = mono_image_open_from_data_full(data.data(), data.size(), 1, &status, 0);
		MonoAssembly* loaded = mono_assembly_load_from_full(image, path, &status, 0);
		mono_image_close(image);
		return loaded;
	}
	static MonoAssembly* LoadAssembly(const std::string& path)
	{
		MonoAssembly* assembly = LoadAssemblyFromFile(path.c_str());
		return assembly;
	}
	static MonoImage* GetAssemblyImage(MonoAssembly* assembly) {
		HZR_CORE_ASSERT((assembly != nullptr), "Cannot read nullptr MonoAssembly image");
		MonoImage* image = mono_assembly_get_image(assembly);
		return image;
	}

#pragma endregion

	ScriptEngine::ScriptEngine() : Module("ScriptEngine")
	{

	}
	ScriptEngine::~ScriptEngine()
	{

	}
	void ScriptEngine::InitAssembly(const char* assemblyPath)
	{
		HZR_CORE_INFO("AssemblyPath {0}", assemblyPath);
		this->assemblyPath = assemblyPath;
		MonoInit();
		LoadRuntimeAssembly(assemblyPath);
	}
	void ScriptEngine::LoadRuntimeAssembly(const char* path)
	{
		MonoDomain* domain = nullptr;
		bool cleanUp = false;

		if (data.mono_domain != nullptr) 
		{
			domain = mono_domain_create_appdomain("Hazard-runtime", nullptr);
			mono_domain_set(domain, false);
			cleanUp = true;
		}
		HZR_CORE_WARN("Loading runtime assembly");
		data.core_assembly = LoadAssembly("res/ScriptEngine.dll");
		
		data.core_assembly_image = GetAssemblyImage(data.core_assembly);

		auto appAssembly = LoadAssembly(path);
		auto appAssemblyImage = GetAssemblyImage(appAssembly);

		if (cleanUp) {
			mono_domain_unload(data.mono_domain);
			data.mono_domain = domain;
		}

		data.app_assembly = appAssembly;
		data.app_assembly_image = appAssemblyImage;

		HZR_CORE_INFO("Loaded successfully");
	}
	void ScriptEngine::ReloadAssembly(const char* assemblyPath)
	{
		LoadRuntimeAssembly(assemblyPath);

		HZR_CORE_INFO("Reloading C# assembly");
	}
	void ScriptEngine::Init()
	{
		
	}
	void ScriptEngine::Close()
	{
		MonoShutdown();
	}
	void ScriptEngine::MonoInit()
	{
		mono_set_dirs("C:/Program Files/Mono/lib", "C:/Program Files/Mono/etc");
		mono_set_assemblies_path("C:/dev/Hazard//vendor/mono/lib");
		MonoDomain* domain = mono_jit_init("Hazard");

		char* name = (char*)"Hazard-runtime";
		data.mono_domain = mono_domain_create_appdomain(name, nullptr);
	}
	void ScriptEngine::MonoShutdown()
	{
		std::cout << "Mono shutdown" << std::endl;
		if(data.mono_domain != nullptr)
			mono_jit_cleanup(data.mono_domain);
	}
	
}