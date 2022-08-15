#pragma once

#include "UtilityCore.h"
#include "Core/Attribute.h"
#include "Mono/Core/Mono.h"

namespace HazardScript 
{
	class FieldMetadata
	{
	public:
		FieldMetadata() = default;
		FieldMetadata(MonoClassField* field);

		std::string GetName() { return m_Name; }
		bool HasValue() { return false; }

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
		std::vector<Attribute*> m_Attributes = std::vector<Attribute*>();
	};
}