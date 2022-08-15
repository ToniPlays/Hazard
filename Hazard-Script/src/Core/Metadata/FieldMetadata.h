#pragma once

#include "UtilityCore.h"
#include "Core/Attribute.h"
#include "Mono/Core/Mono.h"

namespace HazardScript {
	class ScriptField {
	public:
		FieldMetadata() = default;
		FieldMetadata(MonoClassField* field);

		std::string GetName() { return m_Name; }
		const FieldVisibility& GetVisibility() { return m_Visibility; }
		const FieldType& GetType() { return m_Type; }
		MonoClassField* GetField() { return m_Field; }

		template<typename T>
		bool Has() const {
			for (Attribute* attrib : m_Attributes) {
				if (attrib->GetAttributeType() == T::GetStaticType()) return true;
			}
			return false;
		}
		template<typename T>
		const T& Get() {
			for (Attribute* attrib : m_Attributes) {
				if (attrib->GetAttributeType() == T::GetStaticType()) return dynamic_cast<T&>(*attrib);
			}
			return T();
		}
	private:
		void LoadAttributes(MonoClassField* field);

	private:
		std::string m_Name;
		FieldType m_Type = FieldType::None;
		FieldVisibility m_Visibility;
		std::vector<Attribute*> m_Attributes;
	};
}
