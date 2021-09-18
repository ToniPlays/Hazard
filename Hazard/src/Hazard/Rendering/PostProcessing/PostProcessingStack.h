#pragma once

#include "PostProcessingEffect.h"
#include "Hazard/Rendering/RenderUtils.h"

namespace Hazard::Rendering
{
	struct PostProcessingStackCreateInfo 
	{
		VignetteEffectCreateInfo* vignette = nullptr;
		GaussianBlurCreateInfo* gaussianBlur = nullptr;
		BloomCreateInfo* bloom = nullptr;
	};

	struct PostProcessData 
	{
		glm::vec2 resolution;
		float v_Outer;
		float v_Inner;
		float v_Intensity;

		float bloomThreshold;
		float bloomIntensity;
	};

	class PostProcessingStack 
	{
	public:
		PostProcessingStack(PostProcessingStackCreateInfo info);
		~PostProcessingStack() = default;

		void Resize(uint32_t width, uint32_t height);

		FrameBuffer* PostProcess(FrameBuffer* source, glm::vec2 size);

		template<typename T>
		static T* Get() 
		{
			for (auto effect : s_Instance->m_Effects) {
				if (dynamic_cast<T*>(effect)) 
					return (T*)effect;
			}
			return nullptr;
		}

	private:
		static PostProcessingStack* s_Instance;

		std::vector<PostProcessingEffect*> m_Effects;
		PostProcessData m_PostData;

		Ref<UniformBuffer> m_PostBuffer;

		VertexArray* m_VertexArray;
	};
}