#pragma once


namespace Hazard::Rendering
{
	struct PipelineSpecification
	{

	};


	class Pipeline {
	public:
		virtual ~Pipeline() = default;

		virtual PipelineSpecification GetSpecifications() = 0;
		virtual const PipelineSpecification GetSpecifications() const = 0;

		virtual void Invalidate() = 0;
		virtual void Bind() = 0;

		static Pipeline* Create(const PipelineSpecification& specs);
	};
}