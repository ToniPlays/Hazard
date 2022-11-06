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
	class BottomLevelAS;

	struct AccelerationStructureCreateInfo
	{
		std::string DebugName;
		AccelerationStructureLevel Level;
		size_t Size = 0;
		Ref<VertexBuffer> VertexBuffer;
		Ref<IndexBuffer> IndexBuffer;
		BoundingBox BoundingBox;

		BottomLevelAS* pBottomLevel;
	};

	struct TransformMatrixAS
	{
		glm::vec4 MRow0;
		glm::vec4 MRow1;
		glm::vec4 MRow2;
	};

	class AccelerationStructure : public RefCount
	{
	public:
		AccelerationStructure() = default;
		~AccelerationStructure() = default;

		virtual AccelerationStructureLevel GetLevel() const = 0;
	};

	class TopLevelAS : public AccelerationStructure
	{
	public:
		virtual ~TopLevelAS() = default;

		virtual size_t GetCount() const = 0;
		virtual void PushInstances(const glm::mat4& transform, Ref<AccelerationStructure> accelerationStructure) = 0;

		AccelerationStructureLevel GetLevel() const override { return AccelerationStructureLevel::Top; };

		static Ref<TopLevelAS> Create(AccelerationStructureCreateInfo* info);
	};
	class BottomLevelAS : public AccelerationStructure
	{
	public:
		virtual ~BottomLevelAS() = default;

		virtual size_t GetCount() const = 0;
		virtual void PushTransforms(const BufferCopyRegion& copyRegion) = 0;
		AccelerationStructureLevel GetLevel() const override { return AccelerationStructureLevel::Bottom; };

		static Ref<BottomLevelAS> Create(AccelerationStructureCreateInfo* info);
	};
}