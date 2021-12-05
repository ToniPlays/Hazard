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
		CSharpScript(const std::string& moduleName);
		~CSharpScript();

		const std::string& GetModuleName() override { return m_ModuleName; }
		bool IsValid() const override { return true; }
		virtual uint32_t GetHandle() const { return m_ScriptHandle; };
		uint32_t GetPublicFieldCount() override { return m_PublicFields.size(); }
		PublicField& GetPublicField(uint32_t index) override { return *m_PublicFields[index]; }

		void InitClassMethods();
		void UpdatePublicFields();
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
		std::string m_ModuleName;
		std::vector<CSharpField*> m_PublicFields;

	};
}