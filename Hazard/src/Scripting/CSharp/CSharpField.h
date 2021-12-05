#pragma once

#include "Hazard/Scripting/PublicField.h"
#include "ScriptUtils.h"
#include "CSharpEngine.h"

extern "C"
{
	typedef struct _MonoClassField MonoClassField;
}

namespace Hazard::Scripting::CSharp {

	class CSharpScript;

	class CSharpField : public PublicField 
	{
	public:
		CSharpField() = default;
		CSharpField(const std::string& name, FieldType type);
		CSharpField(const std::string& name, const std::string& customType);

	public: 

		std::string GetName() override { return m_Name; }
		FieldType GetType() { return m_Type; }
		const std::string GetCustomType() { return m_CustomType; }

		void SetField(MonoClassField* field) { m_MonoClassField = field; }
		void SetParentScript(CSharpScript* script) { m_Script = script; }

		void CopyStoredToRuntimeValue() override;
		bool RuntimeAvailable() override;

	private:
		void GetStoredValueInternal(void* value) const override;
		void SetStoredValueInternal(void* value) const override;
		void GetRuntimeValueInternal(void* value) const override;
		void SetRuntimeValueInternal(void* value) const override;

	private:

		std::string m_Name;
		FieldType m_Type;
		std::string m_CustomType;
		std::byte* m_Buffer;

		CSharpScript* m_Script;
		MonoClassField* m_MonoClassField;
	};
}