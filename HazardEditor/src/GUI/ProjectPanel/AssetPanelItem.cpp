
#include "AssetPanelItem.h"

#include "Hazard.h"
#include "MathCore.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Core/EditorAssetManager.h"
#include "Core/GUIManager.h"
#include "AssetPanel.h"
#include "Directory.h"

#include "GUI/AssetTools/MaterialEditor.h"
#include "GUI/AssetTools/AssetImporterPanel.h"

#include "imgui_internal.h"

#include "Platform/OS.h"

using namespace Hazard;

namespace UI
{
	void AssetPanelItem::BeginRender()
	{
		ImGui::PushID(m_SourcePath.c_str());
		ImGui::BeginGroup();

		if (m_Flags & m_Flags)
		{
			m_RenameValue = GetName();
			m_Flags &= ~AssetPanelItemFlags_StartRename;
			m_Flags |= AssetPanelItemFlags_Renaming;
		}
	}
	void AssetPanelItem::OnRender(Ref<Hazard::Texture2DAsset> thumbnailIcon, Ref<HazardRenderer::Sampler> sampler, const float& thumbnailSize)
	{
		HZR_PROFILE_FUNCTION();
		const float edgeOffset = 4.0f;

		const float textLineHeight = ImGui::GetTextLineHeightWithSpacing() * 2.0f + edgeOffset * 2.0f;
		const float infoPanelHeight = Math::Min(false ? thumbnailSize * 0.5f : textLineHeight, textLineHeight);

		const ImVec2 topLeft = ImGui::GetCursorScreenPos();
		const ImVec2 thumbBottomRight = { topLeft.x + thumbnailSize, topLeft.y + thumbnailSize };
		const ImVec2 infoTopLeft = { topLeft.x,				 topLeft.y + thumbnailSize };
		const ImVec2 bottomRight = { topLeft.x + thumbnailSize, topLeft.y + thumbnailSize + infoPanelHeight };

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 4.0f });

		auto drawShadow = [&](const ImVec2& topLeft, const ImVec2& bottomRight, bool directory) {
			const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
			auto* drawList = ImGui::GetWindowDrawList();
			const ImRect itemRect = ImUI::RectOffset(topLeft, bottomRight, 1.0f, 1.0f);
			drawList->AddRect(itemRect.Min, itemRect.Max, ImGui::ColorConvertFloat4ToU32(style.MenuBarBackground), 4.0f, directory ? 0 : ImDrawFlags_RoundCornersAll, 4.0f);
		};

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		if (!IsFolder())
		{
			auto* drawList = ImGui::GetWindowDrawList();
			drawShadow(topLeft, bottomRight, false);

			drawList->AddRectFilled(topLeft, thumbBottomRight, ImGui::ColorConvertFloat4ToU32(style.BackgroundColor), 4.0f, ImDrawFlags_RoundCornersTop);
			drawList->AddRectFilled(infoTopLeft, bottomRight, ImGui::ColorConvertFloat4ToU32(style.BackgroundColor), 4.0f, ImDrawFlags_RoundCornersBottom);
		}
		else if (ImGui::ItemHoverable(ImRect(topLeft, bottomRight), ImGui::GetID(&m_Handle)))
		{
			drawShadow(topLeft, bottomRight, true);
			auto* drawList = ImGui::GetWindowDrawList();
			drawList->AddRectFilled(topLeft, bottomRight, ImGui::ColorConvertFloat4ToU32(style.BackgroundColor), 4.0f);
		}

		//Preview icon
		float yPos = ImGui::GetCursorPosY();
		ImGui::InvisibleButton("##thumbnailButton", ImVec2(thumbnailSize, thumbnailSize));

		ImGui::SetCursorPosY(yPos + edgeOffset);
		ImUI::ShiftX(edgeOffset);


		ImUI::Image(thumbnailIcon->GetSourceImageAsset()->Value.As<HazardRenderer::Image2D>(), sampler,
					ImVec2(thumbnailSize - edgeOffset * 2.0, thumbnailSize - edgeOffset * 2.0));

		if (IsFolder())
		{
			for (uint32_t i = 0; i < (uint32_t)AssetType::Last; i++)
			{
				ImUI::DropTarget<AssetHandle>((AssetType)i, [&](AssetHandle handle) {
					AssetMetadata& data = AssetManager::GetMetadata(m_Handle);
					//EditorAssetManager::MoveAssetToFolder(handle, data.Key);
					Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<AssetPanel>()->Refresh();
				});
			}
		}


		ImUI::ShiftY(edgeOffset);
		ImUI::Separator({ thumbnailSize, 2.0f }, style.Colors.AxisX);
		ImUI::Shift(edgeOffset, edgeOffset);
		std::string name = GetName();

		if (IsFolder())
		{
			ImGui::BeginVertical((std::string("InfoPanel") + name).c_str(), ImVec2(thumbnailSize - edgeOffset * 2.0f, infoPanelHeight - edgeOffset));
			{
				ImGui::BeginHorizontal(name.c_str(), ImVec2(thumbnailSize - 2.0f, 0.0f));
				ImGui::Spring();
				//Center text
				{
					ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 3.0f));
					const float textWidth = Math::Min(ImGui::CalcTextSize(name.c_str()).x, thumbnailSize);
					ImGui::SetNextItemWidth(textWidth);
					DrawItemName(name.c_str(), 0.0f);

					ImGui::PopTextWrapPos();
				}

				ImGui::Spring();
				ImGui::EndHorizontal();
				ImGui::Spring();
			}
			ImGui::EndVertical();
		}
		else
		{
			ImGui::BeginVertical((std::string("InfoPanel") + name).c_str(), ImVec2(thumbnailSize - edgeOffset * 2.0f, infoPanelHeight - edgeOffset));
			{
				ImGui::BeginHorizontal("label", ImVec2(0.0f, 0.0f));
				ImGui::SuspendLayout();

				ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 2.0f));

				//Asset name
				DrawItemName(name.c_str(), edgeOffset);
				ImGui::PopTextWrapPos();

				ImGui::ResumeLayout();

				ImGui::Spring();
				ImGui::EndHorizontal();
			}
			ImGui::Spring();
			ImGui::EndVertical();
		}

		ImGui::EndGroup();

		ImUI::DragSource(GetMetadata().Type, &m_Handle, [&]() {
			ImGui::Text("%s", name.c_str());
			ImGui::Text("%s", Hazard::Utils::AssetTypeToString(GetMetadata().Type));
		});

		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered() && !IsFolder() && Input::IsKeyDown(Key::LeftControl))
			OS::OpenInDefault(GetMetadata().Key);

		if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered() && m_Flags == 0)
		{
			OnItemClicked();
		}
		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered() && m_Flags == 0)
		{
			if (!IsFolder())
			{
				if (GetType() == AssetType::Script)
				{
					HZR_INFO("Open script in editor");
				}
				else
				{
					//Open import settings
					auto panel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<AssetImporterPanel>();
					panel->OpenExisting(m_SourcePath, m_Handle);
				}
			}
		}

		ImGui::PopStyleVar();

	}
	void AssetPanelItem::EndRender()
	{
		HZR_PROFILE_FUNCTION();
		ImGui::PopID();
		ImGui::NextColumn();

		if (m_Flags & AssetPanelItemFlags_EndRename)
		{
			RenameTo(m_RenameValue);
			m_Flags &= ~AssetPanelItemFlags_EndRename;
		}
	}
	std::string AssetPanelItem::GetName()
	{
		if (IsFolder())
			return File::GetName(m_SourcePath);

		const AssetMetadata& metadata = GetMetadata();
		return File::GetNameNoExt(GetMetadata().Key);
	}
	void AssetPanelItem::DrawItemName(const char* name, float edgeOffset)
	{
		if (m_Flags & AssetPanelItemFlags_Renaming)
		{
			ImGui::SetKeyboardFocusHere();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - edgeOffset * 4.0f);
			ImUI::TextField_OLD(m_RenameValue);

			if (ImGui::IsItemDeactivated())
			{
				m_Flags &= ~AssetPanelItemFlags_StartRename;
				m_Flags &= ~AssetPanelItemFlags_Renaming;
				m_Flags |= AssetPanelItemFlags_EndRename;
			}
		}
		else
		{
			ImGui::Text("%s", name);

			if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
				m_Flags |= AssetPanelItemFlags_StartRename;
		}
	}
	void AssetPanelItem::RenameTo(const std::string& newName)
	{
		if (newName == GetName())
			return;

		if (IsFolder())
			Directory::Rename(m_SourcePath, newName);

		Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<AssetPanel>()->Refresh();
	}
	void AssetPanelItem::OnItemClicked()
	{
		switch (GetType())
		{
			case AssetType::Material:
			{
				auto panel = Application::GetModule<GUIManager>().GetPanelManager().GetRenderable<MaterialEditor>();

				panel->SetSelectedMaterial(AssetManager::GetAsset<Material>(m_Handle));
			}
			default: break;
		}
	}
}
