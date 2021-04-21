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
		CSharpField(FieldType type);
	public: 
		void SetEntityInstance(EntityInstance* instance) { entityInstance = instance; }
		void SetField(MonoClassField* field) { monoClassField = field; }

		void CopyStoredToRuntimeValue() override;
		bool RuntimeAvailable() override;
	private:
		void GetStoredValueInternal(void* value) const override;
		void SetStoredValueInternal(void* value) const override;
		void GetRuntimeValueInternal(void* value) const override;
		void SetRuntimeValueInternal(void* value) const override;
	private:
		std::byte* buffer;

		EntityInstance* entityInstance;
		MonoClassField* monoClassField;
	};
}