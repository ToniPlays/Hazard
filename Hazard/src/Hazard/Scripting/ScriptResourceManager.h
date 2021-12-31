#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Assets/Asset.h"

namespace Hazard::Scripting 
{
	class ScriptResourceManager 
	{
	public:
		static void Init() 
		{
			Clear();
		};
		static void Clear() 
		{
			for (auto [index, resource] : s_Resources) {
				s_Resources.at(index) = nullptr;
			}
			s_Count = 0;
		}
		template<typename T>
		static Ref<T> GetResource(uint32_t resourceID) {
			return s_Resources[resourceID].As<T>();
		}
		template<typename T, typename CI>
		static uint32_t CreateResource(CI* createInfo) 
		{
			s_Resources[s_Count] = T::Create(createInfo);
			return s_Count++;
		};
		static void Destroy(uint32_t id) 
		{
			auto it = s_Resources.find(id);
			if (it != s_Resources.end()) {
				s_Resources.erase(it);
			}
		}
		
	private:
		inline static std::unordered_map<uint32_t, Ref<Resource>> s_Resources;
		inline static uint32_t s_Count;
	};
}