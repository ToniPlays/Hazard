#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Pipeline/Shader.h"
#include "Hazard/Rendering/Textures/Texture2D.h"
#include "Hazard/Rendering/Material/Material.h"
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
			s_Shaders[File::GetName(name)] = ref;
		}
		template<>
		static void Vault::Add(std::string name, Rendering::Texture2D* ref)
		{
			std::string key = File::GetName(name);
			s_Textures[key] = ref;
		}
		template<>
		static void Vault::Add(std::string name, Rendering::GraphicsPipeline* ref)
		{
			std::cout << "Added pipeline " << name << std::endl;
			s_Pipelines.push_back(ref);
		}
		template<>
		static void Vault::Add(std::string name, Audio::AudioBufferData* ref)
		{
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
			for (auto [key, shader] : s_Shaders) {
				if (key == arg)
					return true;
			}
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
			return false;
		};
		template<>
		static bool Has<Rendering::GraphicsPipeline>(const char* arg)
		{
			for (auto pipeline : s_Pipelines) {
				if (strcmp(pipeline->GetInfo().pipelineName.c_str(), arg) == 0)
					return true;
			}
			return false;
		};
		template<>
		static bool Has<Audio::AudioBufferData>(const char* arg)
		{
			for (auto [key, clip] : s_Audio) {
				if (strcmp(key.c_str(), arg) == 0)
					return true;
			}
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
			return Get<Rendering::Texture2D>("White");
		};
		template<>
		static Rendering::GraphicsPipeline* Get(const char* arg)
		{
			for (auto pipeline : s_Pipelines) {
				if (strcmp(pipeline->GetInfo().pipelineName.c_str(), arg) == 0)
					return pipeline;
			}
			return nullptr;
		};
		template<>
		static Audio::AudioBufferData* Get(const char* arg)
		{
			for (auto [key, clip] : s_Audio) {
				if (strcmp(key.c_str(), arg) == 0)
					return clip;
			}
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
		template<>
		static bool Delete(Rendering::GraphicsPipeline* graphicsPipeline)
		{
			auto it = std::find(s_Pipelines.begin(), s_Pipelines.end(), graphicsPipeline);
			if (it != s_Pipelines.end()) {
				std::cout << "Removed pipeline " << graphicsPipeline->GetInfo().pipelineName << std::endl;
				s_Pipelines.erase(it);
				return true;
			}
			return false;
		}

	private:
		static std::unordered_map<std::string, Rendering::Shader*> s_Shaders;
		static std::unordered_map<std::string, Rendering::Texture*> s_Textures;
		static std::vector<Rendering::GraphicsPipeline*> Vault::s_Pipelines;
		static std::unordered_map<std::string, Audio::AudioBufferData*> s_Audio;
	};
}