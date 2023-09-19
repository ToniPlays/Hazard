#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "Hazard/Assets/AssetManager.h"

namespace Hazard
{
	using namespace HazardScript;

	static Coral::NativeString Asset_GetName_Native(uint64_t id)
	{
		const AssetMetadata& metadata = AssetManager::GetMetadata(id);
		if (metadata.Handle == INVALID_ASSET_HANDLE)
			return Coral::NativeString::FromUTF8("");

		return Coral::NativeString::FromUTF8("");
	}
}