#pragma once

#include "Mono/Core/Mono.h"
#include "Core/Attribute.h"

namespace HazardScript
{
	class MethodMetadata {
	public:
		MethodMetadata() = default;
		MethodMetadata(MonoMethod* method);

		FieldVisibility& GetVisibility() { return m_Visibility; }
		std::string GetName() { return m_Name; }

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
		void Init();
		void LoadAttributes();

	private:
		MonoMethod* m_Method;
		FieldVisibility m_Visibility;
		std::string m_Name;

		std::vector<Attribute*> m_Attributes;
	};
}
