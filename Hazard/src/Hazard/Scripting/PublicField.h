#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Scripting {

	enum FieldType { Float, Float2, Float3, Float4, Int, UInt, String, None };

	class PublicField {
	public:
		PublicField(const std::string& name, FieldType type) : name(name), type(type) {};
		virtual ~PublicField() = default;

		const char* GetName() const { return name.c_str();}
		FieldType GetType() const { return type;}

		template<typename T>
		T GetStoredValue() {
			T value;
			GetStoredValueInternal(&value);
			return value;
		}
		template<typename T>
		void SetStoredValue(T value) {
			SetStoredValueInternal(&value);
		}
		template<typename T>
		T GetRuntimeValue() {
			T value;
			GetRuntimeValueInternal(&value);
			return value;
		}
		template<typename T>
		void SetRuntimeValue(T value) {
			SetRuntimeValueInternal(&value);
		}
	protected:
		virtual void GetStoredValueInternal(void* value) const = 0;
		virtual void SetStoredValueInternal(void* value) const = 0;
		virtual void GetRuntimeValueInternal(void* value) const = 0;
		virtual void SetRuntimeValueInternal(void* value) const = 0;
	

	protected:
		std::string name;
		FieldType type;
	};
}