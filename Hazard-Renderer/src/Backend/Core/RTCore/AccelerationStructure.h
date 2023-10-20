#pragma once

#include "Ref.h"
#include "BoundingBox.h"
#include "Backend/Core/GPUBuffer.h"

namespace HazardRenderer
{
	enum class AccelerationStructureLevel
	{
		None = 0,
		Top,
		Bottom
	};
	enum class BuildType
	{
		Build,
		Update
	};
	class BottomLevelAS;
	
	struct AccelerationStructureGeometry
	{
		glm::mat4 Transform = glm::mat4(1.0f);
		Ref<GPUBuffer> VertexBuffer;
		Ref<GPUBuffer> IndexBuffer;
		BoundingBox BoundingBox;
	};
	struct AccelerationStructureInstance
	{
		glm::mat4 Transform = glm::mat4(1.0f);
		Ref<BottomLevelAS> pBottomLevelAS;
		uint32_t CustomIndex = 0;
	};

	struct AccelerationStructureCreateInfo
	{
		std::string DebugName;
		AccelerationStructureLevel Level;

		//BLAS info
		uint32_t GeometryCount = 0;
		AccelerationStructureGeometry* pGeometries;

		//TLAS info
		uint32_t InstanceCount = 0;
		AccelerationStructureInstance* pInstances;
	};

	class AccelerationStructure : public RefCount
	{
	public:
		AccelerationStructure() = default;
		~AccelerationStructure() = default;

		virtual AccelerationStructureLevel GetLevel() const = 0;
		virtual void Build(const BuildType& type) = 0;
		virtual void Invalidate() = 0;
	};

	class TopLevelAS : public AccelerationStructure
	{
	public:
		virtual ~TopLevelAS() = default;

		virtual uint64_t GetCount() const = 0;
		virtual void PushInstances(const glm::mat4& transform, Ref<AccelerationStructure> accelerationStructure) = 0;

		AccelerationStructureLevel GetLevel() const override { return AccelerationStructureLevel::Top; };

		static Ref<TopLevelAS> Create(AccelerationStructureCreateInfo* info);
	};
	class BottomLevelAS : public AccelerationStructure
	{
	public:
		virtual ~BottomLevelAS() = default;

		virtual uint64_t GetCount() const = 0;
		virtual void PushTransforms(const BufferCopyRegion& copyRegion) = 0;
		AccelerationStructureLevel GetLevel() const override { return AccelerationStructureLevel::Bottom; };

		static Ref<BottomLevelAS> Create(AccelerationStructureCreateInfo* info);
	};
}
