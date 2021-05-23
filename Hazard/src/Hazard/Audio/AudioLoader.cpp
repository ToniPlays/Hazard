#pragma once
#include <hzrpch.h>
#include "AudioLoader.h"

#include <string>
#include <thread>
#include <filesystem>


#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

#include "vorbis/codec.h"
#include "vorbis/vorbisfile.h"

namespace Hazard::Audio {

	static mp3dec_t s_Mp3d;


	void AudioLoader::Init()
	{
		mp3dec_init(&s_Mp3d);

		// Init listener
		ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
		ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
		ALfloat listenerOri[] = { 0.0, 0.0,-1.0, 0.0, 1.0, 0.0 };

		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);
	}

	FileFormat AudioLoader::GetFileFormat(const std::string& file)
	{
		std::filesystem::path path = file;
		std::string ext = path.extension().string();

		if (ext == ".ogg") return FileFormat::Ogg;
		if (ext == ".mp3") return FileFormat::Mp3;
		return FileFormat::None;
	}
	AudioClip AudioLoader::LoadFile(const std::string& file)
	{
		switch (GetFileFormat(file))
		{
		case FileFormat::Mp3: return LoadMp3(file);
		}
		HZR_ERROR("Failed to load {0}", file);
		return AudioClip();
	}
	ALenum AudioLoader::GetOpenALFormat(uint32_t channels)
	{
		switch (channels)
		{
		case 1: return AL_FORMAT_MONO16;
		case 2: return AL_FORMAT_STEREO16;
		}
		return 0;
	}
	AudioClip AudioLoader::LoadMp3(const std::string& file)
	{
		mp3dec_file_info_t info;
		int loadResult = mp3dec_load(&s_Mp3d, file.c_str(), &info, NULL, NULL);

		uint32_t size = info.samples * sizeof(mp3d_sample_t);
		auto sampleRate = info.hz;
		auto channels = info.channels;
		auto alFormat = GetOpenALFormat(channels);
		float lenSec = size / (info.avg_bitrate_kbps * 1024.0f);

		if (true) 
		{
			HZR_CORE_WARN("Loaded audio file: {0}", file);
			HZR_CORE_WARN("Channels: {0}", channels);
			HZR_CORE_WARN("Format: {0}", alFormat);
			HZR_CORE_WARN("Length: {0}", lenSec);
		
		}

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, alFormat, info.buffer, size, sampleRate);

		AudioClip clip = { buffer, true, lenSec };
		alGenSources(1, &clip.m_Source);
		alSourcei(clip.m_Source, AL_BUFFER, buffer);

		return clip;
	}
}