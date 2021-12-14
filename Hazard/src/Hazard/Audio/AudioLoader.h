#pragma once

#include "Hazard/Core/Core.h"
#include "AudioClip.h"
#include <AL/al.h>

namespace Hazard::Audio {

	enum class FileFormat {
		None = 0,
		Ogg,
		Mp3,
		Waw
	};

	struct AudioClipCreateInfo {
		std::string FileName;
		float Gain = 1.0f;
		float Pitch = 1.0f;
		bool PlayOnCreate = false;
		bool Looping = false;
		bool Spatial = false;
	};


	class AudioLoader {
	public:
		static void Init();
		static FileFormat GetFileFormat(const std::string& file);
		static Ref<AudioClip> LoadFile(const std::string& file);

		static std::vector<Ref<AudioClip>> GetAllClips() { return AudioClip::s_AudioClips; }

	private:
		static ALenum GetOpenALFormat(uint32_t channels);
		static void LoadMp3(const std::string& file);
	};
}