#pragma once

#include "Mono/Core/Mono.h"
#include "Attribute.h"

namespace HazardScript 
{
	class Method {
	public:
		Method() = default;
		Method(MonoMethod* method);

		MonoFlags& GetFlags() { return m_Flags; }
		std::string GetName() { return m_Name; }

		MonoObject* Invoke(MonoObject* obj, void** params);

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
		MonoFlags m_Flags;
		std::string m_Name;

		std::vector<Attribute*> m_Attributes;
	};
}