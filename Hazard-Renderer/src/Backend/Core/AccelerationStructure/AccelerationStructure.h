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
	struct AccelerationStructureCreateInfo
	{
		AccelerationStructureLevel Level;
		Ref<VertexBuffer> VertexBuffer;
		Ref<IndexBuffer> IndexBuffer;
		BoundingBox BoundingBox;
	};

	class AccelerationStructure : public RefCount
	{
	public:

		static Ref<AccelerationStructure> Create(AccelerationStructureCreateInfo* info);
	};
}