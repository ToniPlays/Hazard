#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Scripting/Script.h"
#include "CSharpField.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoClassField MonoClassField;
}

namespace Hazard::Scripting::CSharp 
{
	class CSharpScript : public Script
	{
	public:
		CSharpScript() = default;
		CSharpScript(const ScriptMetadata& data);
		~CSharpScript();

		const std::string& GetModuleName() override { return m_Metadata.ModuleName; }
		bool IsValid() const override { return true; }
		virtual uint32_t GetHandle() const override { return m_ScriptHandle; };
		uint32_t GetFieldCount() override { return m_Fields.size(); }
		std::unordered_map<uint32_t, ScriptField*> GetFields() override { return m_Fields; };

		void InitClassMethods();
		void UpdateFields();
		void SetRuntimeValues();

		MonoClass* m_MonoClass;
		MonoMethod* Constructor = nullptr;
		MonoMethod* OnCreated = nullptr;
		MonoMethod* OnStart = nullptr;
		MonoMethod* OnUpdate = nullptr;
		MonoMethod* OnLateUpdate = nullptr;
		MonoMethod* OnFixedUpdate = nullptr;
		MonoMethod* OnColliderEnter2D = nullptr;
		MonoMethod* OnTriggerEnter2D = nullptr;
		MonoMethod* OnColliderExit2D = nullptr;
		MonoMethod* OnTriggerExit2D = nullptr;

	private:
		uint32_t m_ScriptHandle;
		ScriptMetadata m_Metadata;

		std::unordered_map<uint32_t, ScriptField*> m_Fields;
	};
}
