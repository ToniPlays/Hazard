#pragma once

#include "Hazard/Audio/AudioSource.h"

 #include <AL/al.h>

namespace Hazard::Audio 
{
	class AudioFactory {
	public:

		static Ref<AudioBufferData> Load(const std::string& path);
		static FileFormat GetFileFormat(const std::string& path);
		static Ref<AudioBufferData> LoadMp3(const std::string& path);
		static ALenum GetOpenALFormat(uint32_t channels);

	private:
		static void Init();
		friend class AudioLoader;
	};
}
