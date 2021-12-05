#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Scripting/Script.h"

extern "C" {
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoMethod MonoMethod;
}

namespace Hazard::Scripting::CSharp 
{
	class CSharpScript : public Script
	{
	public:
		CSharpScript() = default;
		CSharpScript(const std::string& moduleName);

		const std::string& GetModuleName() override { return m_ModuleName; }
		bool IsValid() const override { return true; }
		uint32_t GetHandle() override { return m_Handle; }

		void InitClassMethods();

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
		std::string m_ModuleName;
		uint32_t m_Handle;

	};
}