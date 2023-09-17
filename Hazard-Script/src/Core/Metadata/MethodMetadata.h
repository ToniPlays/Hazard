#pragma once

#include "Core/Attribute.h"

namespace HazardScript 
{
	class MethodMetadata : public RefCount
	{
	public:
		MethodMetadata() = default;

		std::string GetName() { return m_Name; }

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
		std::string m_Name;

		std::vector<Ref<Attribute>> m_Attributes;
	};
}
