#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Audio {

	class AudioClip {
	public:
		AudioClip() {};
		~AudioClip();

		void Play();
		void Stop();
		void Pause();

		void SetPosition(glm::vec3 pos);
		void SetGain(float gain);
		void SetPitch(float pitch);
		void SetSpatial(bool spatial);
		void SetLoop(bool loop);

		std::pair<uint32_t, uint32_t> GetLength() const;
		
		static AudioClip LoadFromFile(const std::string& file, bool spatial = false);

	private:

		AudioClip(uint32_t buffer, bool loaded, float length);

		uint32_t m_Buffer;
		uint32_t m_Source;

		bool m_Loaded = false;
		bool m_Spatial = false;

		float m_TotalDuration = 0;
		float m_Gain = 1.0f;

		friend class AudioLoader;
	};
}