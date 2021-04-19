#pragma once

#include "Hazard/Scripting/PublicField.h"
#include "ScriptUtils.h"

namespace Hazard::Scripting::CSharp {

	class CSharpField : public PublicField {
	public:
		CSharpField(const std::string& name, FieldType type);

	private:
		void GetStoredValueInternal(void* value) const override;
		void  SetStoredValueInternal(void* value) const override;
		void GetRuntimeValueInternal(void* value) const override;
		void  SetRuntimeValueInternal(void* value) const override;
	private:
		std::byte* buffer;
	};
}