#pragma once

#include "Hazard/Scripting/PublicField.h"
#include "ScriptUtils.h"
#include "CSharpEngine.h"

extern "C"
{
	typedef struct _MonoClassField MonoClassField;
}

namespace Hazard::Scripting::CSharp {

	class CSharpField : public PublicField {
	public:
		CSharpField() = default;
		CSharpField(FieldType type, std::string customType = "");
	public: 
		void SetEntityInstance(EntityInstance* instance) { m_EntityInstance = instance; }
		void SetField(MonoClassField* field) { m_MonoClassField = field; }

		void CopyStoredToRuntimeValue() override;
		bool RuntimeAvailable() override;

	private:
		void GetStoredValueInternal(void* value) const override;
		void SetStoredValueInternal(void* value) const override;
		void GetRuntimeValueInternal(void* value) const override;
		void SetRuntimeValueInternal(void* value) const override;

	private:
		std::byte* m_Buffer;

		EntityInstance* m_EntityInstance;
		MonoClassField* m_MonoClassField;
	};
}