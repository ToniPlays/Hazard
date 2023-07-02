#pragma once

#include "Backend/Core/Core.h"

namespace HazardRenderer 
{
	enum DescriptorType 
	{
		DESCRIPTOR_TYPE_SAMPLER_2D,
		DESCRIPTOR_TYPE_SAMPLER_CUBE,
		DESCRIPTOR_TYPE_STORAGE_IMAGE,
		DESCRIPTOR_TYPE_UNIFORM_BUFFER,
		DESCRIPTOR_TYPE_STORAGE_BUFFER,
		DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE,
	};

	struct DescriptorSetElement
	{
		std::string Name;
		uint32_t Binding;
		DescriptorType Type;

		DescriptorSetElement() = default;

		DescriptorSetElement(const std::string& name, uint32_t binding, DescriptorType type)
			: Name(name), Binding(binding), Type(type)
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

		const std::vector<DescriptorSetElement>& GetElements() const { return m_Elements; }
		size_t GetElementCount() { return m_Elements.size(); }

		std::vector<DescriptorSetElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<DescriptorSetElement>::iterator end() { return m_Elements.end(); }
		std::vector<DescriptorSetElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<DescriptorSetElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		std::vector<DescriptorSetElement> m_Elements;
	};
}
