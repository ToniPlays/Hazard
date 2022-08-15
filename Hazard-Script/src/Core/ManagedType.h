#pragma once

#include "UtilityCore.h"
#include "Ref.h"

#include "Mono/metadata/reflection.h"

namespace HazardScript
{
	namespace Utils {

		// NOTE(Toni): Stolen From Peter
		// NOTE(Peter): Stolen from Boost
		template<typename T>
		inline void HashCombine(size_t& result, const T& value)
		{
			std::hash<T> h;
			result ^= h(value) + 0x9e3779b9 + (result << 6) + (result >> 2);
		}
	}
	class ScriptField;
	enum MonoFlags : uint32_t
	{
		MonoFlags_Private = BIT(0),
		MonoFlags_Protected = BIT(1),
		MonoFlags_Public = BIT(2),
		MonoFlags_Internal = BIT(3),
		MonoFlags_Static = BIT(4),
		MonoFlags_Abstract = BIT(5),
		MonoFlags_Virtual = BIT(6),
		MonoFlags_Interface = BIT(7),
		MonoFlags_Sealed = BIT(8),
	};

	enum class NativeType 
	{
		Undefined = 0,
		Void,
		Bool,
		Float, Float2, Float3, Float4,
		Double,
		Int8, Int16, Int32, Int64,
		UInt8, UInt16, UInt32, UInt64,
		String,
		Value,
		Reference,
	};


	struct ManagedClass 
	{
		uint32_t ID = 0;
		std::string FullName = "";
		uint32_t Size = 0;

		MonoClass* Class = nullptr;
		MonoFlags Flags;
	};

	struct ManagedMethod {

	};

	class ManagedType
	{
	public:
		MonoType* RawMonoType = nullptr;
		int TypeEncoding = 0;
		ManagedClass* TypeClass = nullptr;
		NativeType NativeType = NativeType::Undefined;

		bool HasInterface(const ManagedClass* interfaceClass) const;
		bool IsSubClassOf(const ManagedClass* baseClass) const;
		bool CanAssignTo(const ManagedClass* managedClass) const;
		bool IsVoid() const;
		bool IsArray() const;
		bool IsReferenceType() const;
		bool IsValueType() const;
		bool IsGeneric() const;
		bool IsPrimitive() const;
		uint32_t GetSize() const;
		ManagedType GetElementType() const;

		static ManagedType FromClass(ManagedClass* managedClass);
		static ManagedType FromClass(MonoClass* monoClass);
		static ManagedType FromType(MonoType* monoType);

		bool IsValid() const { return TypeEncoding != 0; }

		ManagedType() = default;
		~ManagedType() {
			RawMonoType = nullptr;
			TypeClass = nullptr;
		}

		bool operator == (const ManagedType& s) {
			return TypeClass == s.TypeClass && TypeEncoding == s.TypeEncoding;
		}
	};

	struct ManagedField
	{
		//ScriptField* Field;
		MonoClassField* MonoField;
		ManagedType Type;
		//Ref<FieldValueStorageBase> Storage;
	};
}