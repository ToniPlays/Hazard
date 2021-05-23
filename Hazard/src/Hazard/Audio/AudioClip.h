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

		bool IsLoaded() { return m_Loaded; }

		glm::vec3 GetPosition() { return m_Pos; }
		void SetPosition(glm::vec3 pos);

		float GetGain() { return m_Gain; }
		void SetGain(float gain);

		float GetPitch() { return m_Pitch; }
		void SetPitch(float pitch);

		bool IsSpatial() { return m_Spatial; }
		void SetSpatial(bool spatial);

		bool IsLooping() { return m_Looping; }
		void SetLoop(bool loop);

		std::pair<uint32_t, uint32_t> GetLength() const;
		
		static AudioClip LoadFromFile(const std::string& file, bool spatial = false);

	private:

		AudioClip(uint32_t buffer, bool loaded, float length);

		uint32_t m_Buffer;
		uint32_t m_Source;

		bool m_Loaded = false;
		bool m_Spatial = false;

		glm::vec3 m_Pos;

		float m_TotalDuration = 0;
		float m_Gain = 1.0f;
		float m_Pitch = 1.0f;
		bool m_Looping = false;

		friend class AudioLoader;
	};
}