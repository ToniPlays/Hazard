#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Pipeline/Shader.h"
#include "Hazard/Rendering/Textures/Texture2D.h"
#include "Hazard/Audio/AudioClip.h"
#include "Hazard/File/File.h"

namespace Hazard 
{
	class Vault {
	public:
		template<typename T, typename Arg>
		static void Add(T type, Arg arg) {
			static_assert(false, "Vault::Add");
		};

		template<>
		static void Vault::Add(std::string name, Rendering::Shader* ref) 
		{
			std::cout << "Added Shader " << name << std::endl;
			s_Shaders[File::GetName(name)] = ref;
		}
		template<>
		static void Vault::Add(std::string name, Rendering::Texture2D* ref)
		{
			std::cout << "Added texture2D " << name << std::endl;
			std::string key = File::GetName(name);
			s_Textures[key] = ref;
		}
		template<>
		static void Vault::Add(std::string name, Audio::AudioBufferData* ref)
		{
			std::cout << "Added AudioBuffer " << name << std::endl;
			s_Audio[File::GetName(name)] = ref;
		}
		template<typename T, typename Arg>
		static bool Has(Arg arg) 
		{
			static_assert(false);
		};
		template<>
		static bool Has<Rendering::Shader>(const char* arg)
		{
			for (auto[key, shader] : s_Shaders) {
				if (key == arg) 
					return shader;
			}
			std::cout << "Shader not found " << arg << std::endl;
			return nullptr;
		};
		template<>
		static bool Has<Rendering::Texture2D>(const char* arg)
		{
			std::string val = File::GetName(arg);
			for (auto [key, texture] : s_Textures) {
				if (key == val)
					return texture;
			}
			std::cout << "Texture2D not found " << arg << std::endl;
			return nullptr;
		};
		template<>
		static bool Has<Audio::AudioBufferData>(const char* arg)
		{
			for (auto [key, clip] : s_Audio) {
				if (strcmp(key.c_str(), arg) == 0)
					return clip;
			}
			std::cout << "AudioBuffer not found " << arg << std::endl;
			return nullptr;
		};

		template<typename T, typename Arg>
		static T* Get(Arg arg)
		{
			return nullptr;
		};

		template<>
		static Rendering::Shader* Get(const char* arg)
		{
			for (auto [key, shader] : s_Shaders) {
				if (key == arg)
					return shader;
			}
			std::cout << "Shader not found " << arg << std::endl;
			return nullptr;
		};
		template<>
		static Rendering::Texture2D* Get(const char* arg)
		{
			std::string val = File::GetName(arg);
			for (auto [key, texture] : s_Textures) {
				if (key == val)
					return (Rendering::Texture2D*)texture;
			}
			std::cout << "Texture2D not found " << arg << std::endl;
			return nullptr;
		};
		template<>
		static Audio::AudioBufferData* Get(const char* arg)
		{
			for (auto [key, clip] : s_Audio) {
				if (strcmp(key.c_str(), arg) == 0)
					return clip;
			}
			std::cout << "AudioBuffer not found " << arg << std::endl;
			return nullptr;
		};

		
		template<typename T>
		static bool Delete(T*)
		{
			static_assert(false, "Vault::Delete(Arg, T)");
		};

	private:
		static std::unordered_map<std::string, Rendering::Shader*> s_Shaders;
		static std::unordered_map<std::string, Rendering::Texture*> s_Textures;
		static std::unordered_map<std::string, Audio::AudioBufferData*> s_Audio;
	};
}