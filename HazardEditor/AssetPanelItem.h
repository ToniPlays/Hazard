#pragma once

#include "Hazard/Assets/AssetManager.h"
#include "UtilityCore.h"
#include "File.h"

namespace UI {
	using namespace Hazard;

	class AssetPanelItem {
	public:
		AssetPanelItem() = default;
		AssetPanelItem(AssetHandle handle) : m_Handle(handle) { }

		void BeginRender();
		void OnRender(const float& thumbnailSize);
		void EndRender();

		const AssetHandle& GetHandle() { return m_Handle; }
		const AssetMetadata& GetMetadata() { return AssetManager::GetMetadata(m_Handle); }
		std::string GetName() { return File::GetName(GetMetadata().Path); }
		const AssetType& GetType() { return GetMetadata().Type; }
		


	private:
		AssetHandle m_Handle;
	};
}