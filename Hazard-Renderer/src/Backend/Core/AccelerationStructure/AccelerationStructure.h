#pragma once

#include "Ref.h"
#include "BoundingBox.h"
#include "Backend/Core/Pipeline/Buffers.h"

namespace HazardRenderer
{
	enum class AccelerationStructureLevel
	{
		None = 0,
		Top,
		Bottom
	};
	class AccelerationStructure;

	struct AccelerationStructureCreateInfo
	{
		std::string DebugName;
		AccelerationStructureLevel Level;
		Ref<VertexBuffer> VertexBuffer;
		Ref<IndexBuffer> IndexBuffer;
		BoundingBox BoundingBox;

		Ref<AccelerationStructure> pAccelerationStructure;
	};

	class AccelerationStructure : public RefCount
	{
	public:

		static Ref<AccelerationStructure> Create(AccelerationStructureCreateInfo* info);
	};
}