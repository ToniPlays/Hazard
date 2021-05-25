#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Pipeline/Shader.h"
#include "Hazard/Rendering/Textures/Texture2D.h"

namespace Hazard 
{
	class Vault {
	public:
		template<typename Arg, typename T>
		static void Add(Arg arg, T type) {
			static_assert(false, "Vault::Add");
		};

		template<>
		static void Vault::Add(std::string name, RefCount* ref) {
			if (name == "")
				throw std::runtime_error("Key cannot be null");
			s_Refs[name] = ref;
		}
		template<typename T, typename Arg>
		static bool Has(Arg arg) 
		{
			for (auto [key, ref] : s_Refs) {
				if (key == arg) {
					if (static_cast<T*>(ref))
						return true;
				}
			}

			return false;
		};

		template<typename T, typename Arg>
		static T* Get(Arg arg)
		{
			for (auto [key, ref] : s_Refs) {
				if (key == arg) {
					if (static_cast<T*>(ref))
						return (T*)ref;
				}
			}
			throw std::runtime_error("Error finding Texture2D from Vault");
		};
		
		template<typename T>
		static bool Delete(T*)
		{
			static_assert(false, "Vault::Add(Arg, T)");
		};

	private:
		static std::unordered_map<std::string, RefCount*> s_Refs;
	};
}