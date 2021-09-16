#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Scripting {

	enum FieldType { Float, Float2, Float3, Float4, Int, UInt, String, None };

	class PublicField {
	public:
		PublicField(FieldType type, std::string customType = "") : m_Type(type), customType(customType) {};
		virtual ~PublicField() = default;

		virtual FieldType GetType() { return m_Type; };
		virtual const std::string GetCustomType() { return customType; }
		virtual void CopyStoredToRuntimeValue() = 0;
		virtual bool RuntimeAvailable() = 0;

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

		FieldType m_Type;
		std::string customType;
	};
}