
#include <hzrpch.h>
#include "AudioFactory.h"

 #define MINIMP3_IMPLEMENTATION
 #include "minimp3.h"
 #include "minimp3_ex.h"
 #include "vorbis/codec.h"

namespace Hazard::Audio 
{
     static mp3dec_t s_Mp3d;

     Ref<AudioBufferData> AudioFactory::Load(const std::string& path)
     {
         switch (GetFileFormat(path))
         {
         case FileFormat::Mp3: return LoadMp3(path);
         }

         return nullptr;
     }
     FileFormat AudioFactory::GetFileFormat(const std::string& path)
     {
         const std::string& ext = File::GetFileExtension(path);

         if (ext == "ogg") return FileFormat::Ogg;
         if (ext == "mp3") return FileFormat::Mp3;

         return FileFormat::None;
     }
     Ref<AudioBufferData> AudioFactory::LoadMp3(const std::string& path)
     {
          HZR_CORE_WARN("[AudioFactory]: Loading Mp3 file {0}", path);
          Ref<AudioBufferData> buffer = Ref<AudioBufferData>::Create();

          mp3dec_file_info_t info;
          int loadResult = mp3dec_load(&s_Mp3d, path.c_str(), &info, NULL, NULL);
          HZR_CORE_ASSERT(loadResult == 0, "Failed to load Mp3 file");
          size_t size = info.samples * sizeof(mp3d_sample_t);

          buffer->Size = size;
          buffer->SampleRate = info.hz;
          buffer->Channels = info.channels;
          buffer->AlFormat = GetOpenALFormat(buffer->Channels);
          buffer->LenSec = size / (info.avg_bitrate_kbps * 1024.0f);
          buffer->AudioData = info.buffer;

          alGenBuffers(1, &buffer->bufferID);
          alBufferData(buffer->bufferID, buffer->AlFormat, buffer->AudioData, (size_t)buffer->Size, buffer->SampleRate);

          return buffer;
     }
     ALenum AudioFactory::GetOpenALFormat(uint32_t channels)
     {
         switch (channels) {
         case 1: return AL_FORMAT_MONO16;
         case 2: return AL_FORMAT_STEREO16;
         }
     }
     void AudioFactory::Init()
     {
         mp3dec_init(&s_Mp3d);
     }
}
