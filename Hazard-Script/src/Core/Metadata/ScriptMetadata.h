#pragma once

#include "MethodMetadata.h"
#include "FieldMetadata.h"
#include <unordered_map>
#include "../Instance/ScriptObject.h"
#include "Coral/HostInstance.hpp"

namespace HazardScript
{
	class Attribute;

	class ScriptMetadata
	{
		friend class ScriptObject;
	public:
		ScriptMetadata() = default;
		ScriptMetadata(const Coral::HostInstance& host, Coral::Type* type);

		std::string GetName();

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
		bool Contains() const
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

		template<typename... Args>
		ScriptObject* CreateObject(Args&&... args) {
			ScriptObject* object = hnew ScriptObject();
			object->m_Handle = m_ReflectionType->CreateInstance(std::forward<Args>(args)...);
			return object;
		}

	private:
		Coral::Type* m_ReflectionType;
		Coral::HostInstance m_Host;
		std::vector<Ref<Attribute>> m_Attributes;
	};
}
