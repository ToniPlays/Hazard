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

	class CSharpField : public ScriptField 
	{
	public:
		CSharpField() = default;
		CSharpField(const ScriptFieldMetadata& metadata, CSharpScript* parent);
		~CSharpField();
	public: 

		std::string GetName() override { return m_Metadata.Name; }
		FieldType GetType() override { return m_Metadata.Type; }

		FieldVisibility GetVisibility() override { return m_Metadata.Visibility; }
		const std::string GetCustomType() { return m_Metadata.CustomType; }
		ScriptFieldMetadata GetFieldMetadata() override { return m_Metadata; };

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

		std::byte* m_Buffer;
		ScriptFieldMetadata m_Metadata;

		CSharpScript* m_Script;
		MonoClassField* m_MonoClassField;
	};
}