
#include "HazardScriptEngine.h"

#include "Buffer.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include <mono/metadata/mono-gc.h>

using namespace HazardUtility;

namespace HazardScript 
{
	HazardScriptEngine* HazardScriptEngine::Create(HazardScriptCreateInfo* info)
	{
		return new HazardScriptEngine(info);
	}

	void HazardScriptEngine::SendDebugMessage(ScriptMessage message)
	{
		if (!s_Instance->m_DebugCallback) return;
		s_Instance->m_DebugCallback(message);
	}

	HazardScriptEngine::HazardScriptEngine(HazardScriptCreateInfo* info) 
	{
		s_Instance = this;

		m_MonoData.CoreAssemblyPath = info->CoreAssemblyPath;
		m_MonoData.AppAssemblyPath = info->AppAssemblyPath;
		m_MonoData.MonoAssemblyDir = info->AssemblyPath;
		m_MonoData.MonoConfigDir = info->ConfigPath;

		m_DebugCallback = info->DebugCallback;
		SendDebugMessage({ Severity::Info, "Debug enabled" });

		InitializeMono();
	}
	void HazardScriptEngine::RegisterInternalCall(const std::string& signature, void* function) 
	{
		mono_add_internal_call(signature.c_str(), function);
	}


	void HazardScriptEngine::InitializeMono()
	{
		mono_set_dirs(m_MonoData.MonoAssemblyDir.string().c_str(), m_MonoData.MonoConfigDir.string().c_str());

		MonoDomain* domain = mono_jit_init("HazardScriptCore");
		m_MonoData.Domain = mono_domain_create_appdomain("HazardScriptCore", nullptr);

		LoadCoreAssebly();
		LoadRuntimeAssembly();

		SendDebugMessage({ Severity::Info, "Assemblies loaded" });
	}
	void HazardScriptEngine::LoadCoreAssebly()
	{
		MonoDomain* domain = nullptr;
		bool cleanup = false;

		if (m_MonoData.Domain) {
			domain = mono_domain_create_appdomain("HazardScriptCore", nullptr);
			mono_domain_set(domain, false);
			cleanup = true;
		}

		m_MonoData.CoreAssembly = LoadAssembly(m_MonoData.CoreAssemblyPath);
		m_MonoData.CoreImage = GetAssemblyImage(m_MonoData.CoreAssembly);
	}
	void HazardScriptEngine::LoadRuntimeAssembly()
	{
		m_MonoData.AppAssembly = LoadAssembly(m_MonoData.AppAssemblyPath);
		m_MonoData.AppImage = GetAssemblyImage(m_MonoData.AppAssembly);
	}
	MonoAssembly* HazardScriptEngine::LoadAssembly(const std::filesystem::path& path)
	{
		Buffer data = File::ReadBinaryFile(path);

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full((char*)data.Data, (uint32_t)data.Size, 1, &status, 0);
		MonoAssembly* loaded = mono_assembly_load_from_full(image, path.string().c_str(), &status, 0);
		mono_image_close(image);
		return loaded;
	}
	MonoImage* HazardScriptEngine::GetAssemblyImage(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		return image;
	}
}