
#include "AssetPanelItem.h"

#include "Hazard.h"
#include "MathCore.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Core/EditorAssetManager.h"
#include "AssetPanel.h"
#include "Directory.h"

#include "Editor/GUI/AssetTools/MaterialEditor.h"
#include "Editor/GUI/AssetTools/AssetImporterPanel.h"

#include "imgui_internal.h"
#include "Platform/OS.h"
#include <Editor/EditorWorldManager.h>
#include <Hazard/ImGUI/GUIManager.h>
#include "AssetEditors/MeshAssetEditorPanel.h"
#include "AssetEditors/MaterialAssetEditorPanel.h"

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
			auto color = style.BackgroundColor;
			if (ImGui::IsMouseHoveringRect(topLeft, bottomRight))
				color = style.MenuBarBackground;

			auto* drawList = ImGui::GetWindowDrawList();
			drawShadow(topLeft, bottomRight, false);

			drawList->AddRectFilled(topLeft, thumbBottomRight, ImGui::ColorConvertFloat4ToU32(color), 4.0f, ImDrawFlags_RoundCornersTop);
			drawList->AddRectFilled(infoTopLeft, bottomRight, ImGui::ColorConvertFloat4ToU32(color), 4.0f, ImDrawFlags_RoundCornersBottom);
		}
		else if (ImGui::IsMouseHoveringRect(topLeft, bottomRight))
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

		ImUI::Image(thumbnailIcon->GetSourceImage(), sampler,
					ImVec2(thumbnailSize - edgeOffset * 2.0, thumbnailSize - edgeOffset * 2.0));

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

		if (!IsFolder())
		{
			ImUI::DragSource(GetMetadata().Type, &m_Handle, [&]() {
				ImGui::Text("%s", name.c_str());
				ImGui::Text("%s", Hazard::Utils::AssetTypeToString(GetMetadata().Type));
			});
		}

		if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
		{
			if (!Input::IsKeyDown(Key::LeftControl))
			{
				OnItemDoubleClicked();
			}
			else
			{
				auto& panel = Application::Get().GetModule<Hazard::GUIManager>().GetExistingOrNew<AssetImporterPanel>();
				panel.Open(GetMetadata().Handle);
			}
		}

		if (ImGui::IsMouseClicked(0) && ImGui::IsItemHovered() && m_Flags == 0)
			OnItemClicked();

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
		return File::GetNameNoExt(metadata.FilePath);
	}

	void AssetPanelItem::DrawItemName(const char* name, float edgeOffset)
	{
		if (m_Flags & AssetPanelItemFlags_Renaming)
		{
			ImGui::SetKeyboardFocusHere();
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - edgeOffset * 4.0f);

			char buf[512];
			strcpy(buf, m_RenameValue.c_str());
			if (ImGui::InputText("##", buf, 512))
				m_RenameValue = buf;

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
		else
		{
			auto& metadata = Hazard::AssetManager::GetMetadata(m_Handle);
			metadata.FilePath = newName;
		}

		Application::Get().GetModule<GUIManager>().GetExistingOrNew<AssetPanel>().Refresh();
	}

	void AssetPanelItem::OnItemClicked()
	{

	}

	void AssetPanelItem::OnItemDoubleClicked()
	{
		switch (GetType())
		{
			case AssetType::World:
				Editor::EditorWorldManager::LoadWorld(m_Handle);
				break;
			case AssetType::Mesh:
			{
				auto& panel = Application::Get().GetModule<GUIManager>().GetExistingOrNew<MeshAssetEditorPanel>();
				panel.BringToFront();
				panel.SetMeshHandle(m_Handle);
				break;
			}
		}
	}
}
