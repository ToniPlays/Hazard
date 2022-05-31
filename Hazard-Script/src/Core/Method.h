#pragma once

#include "Mono/Core/Mono.h"
#include "Attribute.h"

namespace HazardScript 
{
	class Method {
	public:
		Method() = default;
		Method(MonoMethod* method);

		FieldVisibility& GetVisibility() { return m_Visibility; }
		std::string GetName() { return m_Name; }

		MonoObject* Invoke(MonoObject* obj, void* params);

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