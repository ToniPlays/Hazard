#pragma once
#include <hzrpch.h>
#include "AudioLoader.h"
#include "AudioClip.h"

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

		// Init default listener
		ALfloat listenerPos[] = { 0.0, 0.0, 0.0 };
		ALfloat listenerVel[] = { 0.0, 0.0, 0.0 };
		ALfloat listenerOri[] = { 0.0, 0.0,-1.0, 0.0, 1.0, 0.0 };

		alListenerfv(AL_POSITION, listenerPos);
		alListenerfv(AL_VELOCITY, listenerVel);
		alListenerfv(AL_ORIENTATION, listenerOri);
	}

	FileFormat AudioLoader::GetFileFormat(const std::string& file)
	{
		std::string ext = File::GetFileExtension(file);

		if (ext == "ogg") return FileFormat::Ogg;
		if (ext == "mp3") return FileFormat::Mp3;

		HZR_ERROR("[Audio]: Failed to load {0}", file);
		return FileFormat::None;
	}
	AudioClip AudioLoader::LoadFile(const std::string& file)
	{
		switch (GetFileFormat(file))
		{
		case FileFormat::Mp3: return LoadMp3(file);
		}

		HZR_ERROR("[Audio]: Failed to load {0}", file);
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
		//Check if file is already loaded
		AudioBufferData* buffer = nullptr;
		//if (Vault::Has<AudioBufferData>(file.c_str())) {
		//	buffer = Vault::Get<AudioBufferData>(file.c_str());
		//}
		//else 
		{
			//Load new MP3 track
			mp3dec_file_info_t info;
			int loadResult = mp3dec_load(&s_Mp3d, file.c_str(), &info, NULL, NULL);
			size_t size = info.samples * sizeof(mp3d_sample_t);

			buffer = new AudioBufferData();
			buffer->name = file;
			buffer->size = size;
			buffer->sampleRate = info.hz;
			buffer->channels = info.channels;
			buffer->alFormat = GetOpenALFormat(buffer->channels);
			buffer->lenSec = size / (info.avg_bitrate_kbps * 1024.0f);
			buffer->audioData = info.buffer;

			//Vault::Add(file, buffer);
		}
		//Initialize AudioClip
		ALuint bufferID;
		alGenBuffers(1, &bufferID);
		alBufferData(bufferID, buffer->alFormat, buffer->audioData, buffer->size, buffer->sampleRate);

		AudioClip clip = { bufferID, true, buffer->lenSec };
		alGenSources(1, &clip.m_Source);
		alSourcei(clip.m_Source, AL_BUFFER, bufferID);

		return clip;
	}
}