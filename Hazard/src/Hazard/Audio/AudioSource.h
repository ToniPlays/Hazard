#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Assets/Asset.h"
#include "AudioLoader.h"

namespace Hazard::Audio {

	struct AudioBufferData : public Asset
	{
	public:
		AudioBufferData() = default;
		~AudioBufferData() = default;

		int16_t* AudioData = nullptr;
		size_t Size = 0;
		uint32_t SampleRate = 0;
		uint32_t Channels = 0;
		int AlFormat = 0;
		float LenSec = 0;

		uint32_t bufferID;
	};

	class AudioSource {
	public:
		AudioSource();
		~AudioSource();

		void Play();
		void Stop();
		void Pause();

		bool IsLoaded() { return m_AudioSourceBuffer; }

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

		void SetSourceBuffer(Ref<AudioBufferData> buffer);

		float GetTotalDuration() { return m_AudioSourceBuffer->LenSec; }
		std::pair<uint32_t, uint32_t> GetLength() const;

	private:

		AudioSource(Ref<AudioBufferData> buffer);

		uint32_t m_Source = 0;

		glm::vec3 m_Pos = { 0, 0, 0 };

		float m_Gain = 1.0f;
		float m_Pitch = 1.0f;

		bool m_Spatial = false;
		bool m_Looping = false;
			
		Ref<AudioBufferData> m_AudioSourceBuffer;
		friend class AudioFactory;
	};
}