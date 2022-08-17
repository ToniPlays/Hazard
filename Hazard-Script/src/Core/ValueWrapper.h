#pragma once

#include "UtilityCore.h"
#include "Mono/Core/Mono.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace HazardScript 
{
	class ValueWrapper 
	{
	public:
		ValueWrapper() = default;
		ValueWrapper(bool value) : ValueWrapper(&value, sizeof(bool)) {};

		ValueWrapper(int8_t value) : ValueWrapper(&value, sizeof(int8_t)) {};
		ValueWrapper(int16_t value) : ValueWrapper(&value, sizeof(int16_t)) {};
		ValueWrapper(int32_t value) : ValueWrapper(&value, sizeof(int32_t)) {};
		ValueWrapper(int64_t value) : ValueWrapper(&value, sizeof(int64_t)) {};

		ValueWrapper(uint8_t value) : ValueWrapper(&value, sizeof(uint8_t)) {};
		ValueWrapper(uint16_t value) : ValueWrapper(&value, sizeof(uint16_t)) {};
		ValueWrapper(uint32_t value) : ValueWrapper(&value, sizeof(uint32_t)) {};
		ValueWrapper(uint64_t value) : ValueWrapper(&value, sizeof(uint64_t)) {};

		ValueWrapper(float value) : ValueWrapper(&value, sizeof(float)) {};
		ValueWrapper(double value) : ValueWrapper(&value, sizeof(double)) {};

		ValueWrapper(const std::string& value) : ValueWrapper(static_cast<const char*>(value.c_str()), value.size()) {};
		ValueWrapper(const glm::vec2& value) : ValueWrapper(static_cast<const void*>(glm::value_ptr<float>(value)), sizeof(float) * 2) {};
		ValueWrapper(const glm::vec3& value) : ValueWrapper(static_cast<const void*>(glm::value_ptr<float>(value)), sizeof(float) * 3) {};
		ValueWrapper(const glm::vec4& value) : ValueWrapper(static_cast<const void*>(glm::value_ptr<float>(value)), sizeof(float) * 4) {};


		ValueWrapper(const void* data, size_t size) : m_Size(size) 
		{
			m_Buffer = new uint8_t[size];
			memcpy(m_Buffer, data, size);
		}

		ValueWrapper(const ValueWrapper& other) 
		{
			m_Size = other.m_Size;

			if (m_Size) {
				m_Buffer = new uint8_t[m_Size];
				memcpy(m_Buffer, other.m_Buffer, m_Size);
			}
		}

		ValueWrapper& operator=(const ValueWrapper& other)
		{
			if (m_Buffer != other.m_Buffer)
			{
				ReleaseBuffer();

				m_Size = other.m_Size;
				if (m_Size)
				{
					m_Buffer = new byte[m_Size];
					memcpy(m_Buffer, other.m_Buffer, m_Size);
				}
				else
				{
					m_Buffer = nullptr;
				}
			}

			return *this;
		}


		~ValueWrapper()
		{
			if (!HasValue()) return;
			m_Size = 0;
			delete[] m_Buffer;
			m_Buffer = nullptr;
		}

		void ReleaseBuffer()
		{
			if (m_Size == 0) return;
			m_Size = 0;
			delete[] m_Buffer;
			m_Buffer = nullptr;
		}

		bool HasValue() { return m_Size > 0 && m_Buffer != nullptr; }

		template<typename TValueType>
		TValueType Get() const
		{
			if constexpr (std::is_same<TValueType, std::string>::value)
				return TValueType((char*)m_Buffer, m_Size);
			else
				return *(TValueType*)m_Buffer;
		}

		template<typename TValueType>
		TValueType GetOrDefault(const TValueType& defaultValue) const
		{
			if (!HasValue())
				return defaultValue;

			if constexpr (std::is_same<TValueType, std::string>::value)
				return TValueType((char*)m_Buffer, m_Size);
			else
				return *(TValueType*)m_Buffer;
		}

		void* GetRawData() const { return m_Buffer; }
		size_t GetDataSize() const { return m_Size; }

		template<typename TValueType>
		void Set(TValueType value)
		{
			if constexpr (std::is_same<TValueType, std::string>::value)
			{
				if (value.size() > m_Size)
				{
					delete[] m_Buffer;
					m_Buffer = new byte[value.size()];
					m_Size = value.size();
				}

				memcpy(m_Buffer, value.data(), value.size());
			}
			else
			{
				memcpy(m_Buffer, &value, sizeof(value));
			}
		}

		static ValueWrapper OfSize(size_t size)
		{
			ValueWrapper value;
			value.m_Buffer = new byte[size];
			value.m_Size = size;
			return value;
		}

		static ValueWrapper Empty() { return ValueWrapper(); }


	private:
		size_t m_Size = 0;
		uint8_t* m_Buffer;
	};
}