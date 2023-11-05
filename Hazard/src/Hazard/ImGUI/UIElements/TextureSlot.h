#pragma once

#include "UtilityCore.h"
#include "Hazard/Assets/Asset.h"
#include "Color.h"
#include <imgui_internal.h>
#include "Hazard/ImGUI/StyleManager.h"
#include "Hazard/ImGUI/UILibrary.h"

namespace Hazard::ImUI
{
	class TextureSlot
	{
	public:
		TextureSlot(const std::string& title, AssetHandle handle) : m_Title(title), m_Handle(handle) {};
		TextureSlot(const char* title) : m_Title(title) {};
		~TextureSlot() {}

		void Render()
		{
			using namespace Hazard::ImUI;

			float width = ImGui::GetContentRegionAvail().x;
			float size = 28.0f;

			const Style& style = StyleManager::GetCurrent();
			ImGui::PushID(m_Title.c_str());
			ImGui::Columns(3, 0, false);
			ImGui::SetColumnWidth(0, 125.0f);
			ImGui::SetColumnWidth(1, width - 125.0f - size - 7.0f);
			ImGui::SetColumnWidth(2, 28.0f);
			ImUI::Shift(0.0f, 4.0f);
			ImGui::Text(m_Title.c_str());
			ImGui::NextColumn();

			ImGui::SetNextItemWidth(width);

			AssetMetadata& metadata = AssetManager::GetMetadata(m_Handle);

			ImUI::TextField text(metadata.Key.empty() ? "None" : metadata.Key);
			text.Render();

			ImGui::NextColumn();

			if (metadata.IsValid())
			{
				if (metadata.LoadState == LoadState::None)
					JobPromise promise = AssetManager::GetAssetAsync(m_Handle);
				else if (metadata.LoadState == LoadState::Loading)
					m_Handle = INVALID_ASSET_HANDLE;
			}
			else m_Handle = INVALID_ASSET_HANDLE;

			Ref<Texture2DAsset> asset = AssetManager::GetAsset<Texture2DAsset>(m_Handle);

			ImUI::ShiftX(-8.0f);

			if (!asset)
			{
				ImVec2 pos = ImGui::GetCursorScreenPos();
				ImGui::GetWindowDrawList()->AddRectFilled(pos, { pos.x + size, pos.y + size }, ImGui::GetColorU32({ 1.0, 1.0, 1.0, 1.0 }));
			}
			else
			{
				ImUI::Image(asset->GetSourceImageAsset()->Value.As<Image2D>(), asset->GetSampler(), { size, size });
			}

			if (!m_DropType.empty())
				ImUI::DropTarget<AssetHandle>(m_DropType.c_str(), m_Callback);

			ImGui::Columns();
			ImGui::PopID();
		}

		void SetDropCallback(AssetType type, const std::function<void(AssetHandle)> callback)
		{
			m_DropType = Utils::AssetTypeToString(type);
			m_Callback = callback;
		};

		bool DidChange() { return m_DidChange; }
		AssetHandle GetValue() { return m_Handle; }

	private:
		std::string m_Title;
		AssetHandle m_Handle;
		std::string m_DropType;
		std::function<void(AssetHandle)> m_Callback;
		bool m_DidChange = false;
	};
}