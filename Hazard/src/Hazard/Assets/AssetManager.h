#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Core/UUID.h"

namespace Hazard 
{
	class AssetManager {
	public:
		AssetManager() = default;
		~AssetManager() = default;

		static void Init();
		static void Shutdown();
		
	};
}