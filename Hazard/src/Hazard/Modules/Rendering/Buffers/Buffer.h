#pragma once

#include "Hazard/Core/Core.h"
#include "glad/glad.h"
#include "Hazard/Utils/ShaderData.h"

namespace Hazard {

	class HAZARD_API VertexBuffer {

	public:
		virtual void SetData(void* data) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetID() const = 0;
		virtual std::string GetName() const = 0;
		virtual uint32_t GetSize() const = 0;
		virtual ShaderDataType GetType() const = 0;
	};

	class HAZARD_API IndexBuffer {

	public:

		virtual void SetData(int* indices, uint32_t size) = 0;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;
		virtual uint32_t GetID() const = 0;
	};
}