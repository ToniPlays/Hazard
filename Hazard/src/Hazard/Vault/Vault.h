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
					return true;
			}
			std::cout << "Shader not found " << arg << std::endl;
			return false;
		};
		template<>
		static bool Has<Rendering::Texture2D>(const char* arg)
		{
			std::string val = File::GetName(arg);
			for (auto [key, texture] : s_Textures) {
				if (key == val)
					return true;
			}
			std::cout << "Texture2D not found " << arg << std::endl;
			return false;
		};
		template<>
		static bool Has<Audio::AudioBufferData>(const char* arg)
		{
			for (auto [key, clip] : s_Audio) {
				if (strcmp(key.c_str(), arg) == 0)
					return true;
			}
			std::cout << "AudioBuffer not found " << arg << std::endl;
			return false;
		};

		template<typename T, typename Arg>
		static T* Get(Arg arg)
		{
			static_assert(false);
			return nullptr;
		};

		template<>
		static Rendering::Shader* Get(const char* arg)
		{
			for (auto [key, shader] : s_Shaders) {
				if (strcmp(key.c_str(), arg) == 0)
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
				if (strcmp(val.c_str(), key.c_str()) == 0)
					return (Rendering::Texture2D*)texture;
			}
			std::cout << "Texture2D not found " << arg << std::endl;
			return Get<Rendering::Texture2D>("White");
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
		template<>
		static bool Delete(Rendering::Shader* shader) 
		{
			for (auto [key, s] : s_Shaders) {
				if (s == shader) 
				{
					s_Shaders.erase(key);
					return true;
				}
			}
			return false;
		}

	private:
		static std::unordered_map<std::string, Rendering::Shader*> s_Shaders;
		static std::unordered_map<std::string, Rendering::Texture*> s_Textures;
		static std::unordered_map<std::string, Audio::AudioBufferData*> s_Audio;
	};
}