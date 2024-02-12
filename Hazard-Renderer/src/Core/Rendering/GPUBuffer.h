#pragma once

#include "BufferLayout.h"
#include "Core/Core.h"

namespace HazardRenderer
{
	class VertexBuffer;

	enum BufferUsageFlags : uint32_t
	{
		BUFFER_USAGE_VERTEX_BUFFER_BIT = BIT(0),
		BUFFER_USAGE_INDEX_BUFFER_BIT = BIT(1),
		BUFFER_USAGE_UNIFORM_BUFFER_BIT = BIT(2),
		BUFFER_USAGE_STORAGE_BUFFER_BIT = BIT(3),
		BUFFER_USAGE_INDIRECT_BIT = BIT(4),
		BUFFER_USAGE_ACCELERATION_STRUCTURE = BIT(5),
		BUFFER_USAGE_DYNAMIC = BIT(6),
		BUFFER_USAGE_TRANSFER_SRC = BIT(7)
	};
    struct BufferCreateInfo
    {
        std::string Name;
		uint32_t UsageFlags = 0;
		uint64_t Size;
		void* Data = nullptr;
    };

	struct BufferCopyRegion
	{
		uint64_t Size = 0;
		uint64_t Offset = 0;
		const void* Data = nullptr;
	};

	class GPUBuffer : public RefCount
	{
	public:
		virtual ~GPUBuffer() = default;

		virtual const std::string& GetDebugName() const = 0;

		virtual Buffer ReadData(const BufferCopyRegion& copyRegion) = 0;
		virtual void SetData(const BufferCopyRegion& copyRegion) = 0;
		virtual const uint32_t GetUsageFlags() const = 0;
        virtual const uint64_t GetSize() const = 0;

		static Ref<GPUBuffer> Create(BufferCreateInfo* createInfo);
	};
}
