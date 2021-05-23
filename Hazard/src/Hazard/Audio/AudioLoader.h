#pragma once

#include "Hazard/Core/Core.h"
#include "AudioClip.h"

#include "AL/al.h"

namespace Hazard::Audio {

	enum class FileFormat {
		None = 0,
		Ogg,
		Mp3
	};

	class AudioLoader {
	public:
		static void Init();
		static FileFormat GetFileFormat(const std::string& file);
		static AudioClip LoadFile(const std::string& file);
	private:
		static ALenum GetOpenALFormat(uint32_t channels);
		static AudioClip LoadMp3(const std::string& file);
	};
}