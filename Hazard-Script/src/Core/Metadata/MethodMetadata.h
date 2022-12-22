#pragma once

#include "Mono/Core/Mono.h"
#include "Core/Attribute.h"
#include "ManagedType.h"

namespace HazardScript 
{
	class MethodMetadata : public RefCount
	{
	public:
		MethodMetadata() = default;
#ifdef HZR_INCLUDE_MONO
		MethodMetadata(MonoMethod* method);
#endif

		MonoFlags& GetFlags() { return m_Flags; }
		std::string GetName() { return m_Name; }
		ManagedType& GetReturnType() { return m_ManagedMethod.ReturnType; }
#ifdef HZR_INCLUDE_MONO
		MonoObject* Invoke(MonoObject* obj, void** params = nullptr);
#endif

		template<typename T>
		bool Has() const 
		{
			for (Ref<Attribute> attrib : m_Attributes) 
			{
				if (attrib->GetAttributeType() == T::GetStaticType()) 
					return true;
			}
			return false;
		}
		template<typename T>
		const Ref<T> Get() 
		{
			for (Ref<Attribute> attrib : m_Attributes) 
			{
				if (attrib->GetAttributeType() == T::GetStaticType()) 
					return attrib.As<T>();
			}
			return Ref<T>();
		}


	private:
		void Init();
		void LoadAttributes();

	private:
		ManagedMethod m_ManagedMethod;
		MonoFlags m_Flags;
		std::string m_Name;

		std::vector<Ref<Attribute>> m_Attributes;
	};
}
