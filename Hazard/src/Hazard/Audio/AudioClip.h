#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Audio {

	struct AudioBufferData : public RefCount
	{
	public:
		AudioBufferData() = default;
		~AudioBufferData() = default;

		std::string Name = "UndefinedAudio";
		int16_t* AudioData = nullptr;
		size_t Size = 0;
		uint32_t SampleRate = 0;
		uint32_t Channels = 0;
		float AlFormat = 0;
		float LenSec = 0;
	};

	class AudioClip {
	public:
		AudioClip() = default;
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

		float GetTotalDuration() { return m_TotalDuration; }
		std::pair<uint32_t, uint32_t> GetLength() const;

		static AudioClip LoadFromFile(const std::string& file, bool spatial = false);

	private:

		AudioClip(uint32_t buffer, bool loaded, float length);

		uint32_t m_Buffer = 0;
		uint32_t m_Source = 0;

		glm::vec3 m_Pos = { 0, 0, 0 };
		float m_TotalDuration = 0.0f;
		float m_Gain = 1.0f;
		float m_Pitch = 1.0f;

		bool m_Loaded = false;
		bool m_Spatial = false;
		bool m_Looping = false;

		friend class AudioLoader;
	};
}