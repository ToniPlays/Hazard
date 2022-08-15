#pragma once

#include "UtilityCore.h"
#include "Attribute.h"
#include "Mono/Core/Mono.h"

namespace HazardScript 
{
	class ScriptField 
	{
	public:
		ScriptField() = default;
		ScriptField(MonoClassField* field);

		std::string GetName() { return m_Name; }
		MonoClassField* GetField() { return m_Field; }
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
		void LoadAttributes();

	private:
		MonoClassField* m_Field;
		std::string m_Name;
		std::vector<Attribute*> m_Attributes;
	};
}