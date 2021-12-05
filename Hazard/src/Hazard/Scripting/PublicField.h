#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Scripting {

	enum class FieldType {  None, Float, Float2, Float3, Float4, Int, UInt, String, Custom };

	class PublicField {
	public:

		virtual ~PublicField() = default;

		virtual std::string GetName() = 0;
		virtual FieldType GetType() = 0;
		virtual const std::string GetCustomType() = 0;
		virtual void CopyStoredToRuntimeValue() = 0;
		virtual bool RuntimeAvailable() = 0;

		template<typename T>
		T GetStoredValue() {
			T value;
			GetStoredValueInternal(&value);
			return value;
		}
		template<typename T>
		T GetValue(bool runtime = false) {
			T value;
			if (runtime)
				GetRuntimeValueInternal(&value);
			else 
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
		template<typename T>
		void SetValue(T value, bool runtime = false) {
			if (runtime) {
				if(RuntimeAvailable())
					SetRuntimeValueInternal(&value);
			}
			else
				SetStoredValueInternal(&value);
		}
		
	protected:
		virtual void GetStoredValueInternal(void* value) const = 0;
		virtual void SetStoredValueInternal(void* value) const = 0;
		virtual void GetRuntimeValueInternal(void* value) const = 0;
		virtual void SetRuntimeValueInternal(void* value) const = 0;
	};
}