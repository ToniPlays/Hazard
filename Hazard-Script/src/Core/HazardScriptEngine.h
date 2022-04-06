#pragma once

#include "HazardScriptCore.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoType MonoType;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;

	typedef struct _MonoDomain MonoDomain;
	typedef struct _MonoAssembly MonoAssembly;
	typedef struct _MonoImage MonoImage;
}

namespace HazardScript 
{
	struct MonoData {
		MonoDomain* Domain;

		std::filesystem::path MonoAssemblyDir;
		std::filesystem::path MonoConfigDir;
		
		MonoAssembly* CoreAssembly;
		MonoImage* CoreImage;
		std::filesystem::path CoreAssemblyPath;

		MonoAssembly* AppAssembly;
		MonoImage* AppImage;
		std::filesystem::path AppAssemblyPath;
	};

	class HazardScriptEngine 
	{
	public:
		HazardScriptEngine() = delete;
		~HazardScriptEngine() {};


		void Reload() {};
		
		void RegisterInternalCall(const std::string& signature, void* function);

	
	public:
		static HazardScriptEngine* Create(HazardScriptCreateInfo* info);
		static void SendDebugMessage(ScriptMessage message);

	private:
		HazardScriptEngine(HazardScriptCreateInfo* info);

		void InitializeMono();
		void LoadCoreAssebly();
		void LoadRuntimeAssembly();
		MonoAssembly* LoadAssembly(const std::filesystem::path& path);
		MonoImage* GetAssemblyImage(MonoAssembly* assembly);

	private:

		ScriptDebugCallback m_DebugCallback;
		MonoData m_MonoData;

		inline static HazardScriptEngine* s_Instance;

	};
}