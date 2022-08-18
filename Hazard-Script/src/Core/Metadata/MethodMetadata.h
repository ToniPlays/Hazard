#pragma once

#include "Mono/Core/Mono.h"
#include "Core/Attribute.h"
#include "ManagedType.h"

namespace HazardScript 
{
	class MethodMetadata 
	{
	public:
		MethodMetadata() = default;
		MethodMetadata(MonoMethod* method);

		MonoFlags& GetFlags() { return m_Flags; }
		std::string GetName() { return m_Name; }
		ManagedType& GetReturnType() { return m_ManagedMethod.ReturnType; }

		MonoObject* Invoke(MonoObject* obj, void** params = nullptr);

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
		ManagedMethod m_ManagedMethod;
		MonoFlags m_Flags;
		std::string m_Name;

		std::vector<Attribute*> m_Attributes;
	};
}