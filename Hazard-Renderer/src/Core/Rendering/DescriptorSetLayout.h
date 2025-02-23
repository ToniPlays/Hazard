#pragma once

#include "Core/Core.h"

namespace HazardRenderer
{
	enum DescriptorType : uint16_t
	{
		DESCRIPTOR_TYPE_INVALID_DESCRIPTOR_TYPE = BIT(0),
		DESCRIPTOR_TYPE_SAMPLER_2D = BIT(1),
		DESCRIPTOR_TYPE_SAMPLER_CUBE = BIT(2),
		DESCRIPTOR_TYPE_STORAGE_IMAGE = BIT(3),
		DESCRIPTOR_TYPE_UNIFORM_BUFFER = BIT(4),
		DESCRIPTOR_TYPE_STORAGE_BUFFER = BIT(5),
		DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE = BIT(6),
	};

	struct DescriptorSetElement
	{
		std::string Name;
		uint32_t Binding;
		uint32_t Length = 1;
		DescriptorType Type;
		uint32_t Flags;

		DescriptorSetElement() = default;

		DescriptorSetElement(uint32_t stageMask, const std::string& name, uint32_t binding, DescriptorType type)
			: Flags(stageMask), Name(name), Binding(binding), Type(type)
		{
		}
		DescriptorSetElement(uint32_t stageMask, const std::string& name, uint32_t binding, uint32_t length, DescriptorType type)
			: Flags(stageMask),Name(name), Binding(binding), Length(length), Type(type)
		{
		}
	};

	class DescriptorSetLayout
	{
	public:
		DescriptorSetLayout() {}

		DescriptorSetLayout(std::initializer_list<DescriptorSetElement> elements)
			: m_Elements(elements)
		{
		}
		DescriptorSetLayout(std::vector<DescriptorSetElement> elements)
			: m_Elements(elements)
		{
		}

		std::vector<DescriptorSetElement>& GetElements() { return m_Elements; }
		uint64_t GetElementCount() { return m_Elements.size(); }

		std::vector<DescriptorSetElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<DescriptorSetElement>::iterator end() { return m_Elements.end(); }
		std::vector<DescriptorSetElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<DescriptorSetElement>::const_iterator end() const { return m_Elements.end(); }

		bool operator == (const DescriptorSetLayout& other) const
		{
			if (m_Elements.size() != other.m_Elements.size()) return false;

			for (uint32_t i = 0; i < m_Elements.size(); i++)
			{
				auto& l = m_Elements[i];
				auto& r = other.m_Elements[i];
				if (l.Type != r.Type || l.Binding != r.Binding || l.Length != r.Length || l.Flags != r.Flags)
					return false;
			}

			return true;
		}


	private:
		std::vector<DescriptorSetElement> m_Elements;
	};
}
