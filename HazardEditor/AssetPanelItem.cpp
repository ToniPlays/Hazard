
#include "AssetPanelItem.h"

#include "GUI/Core/UILibrary.h"
#include "MathCore.h"

namespace UI
{
	void AssetPanelItem::BeginRender()
	{
		ImGui::PushID(m_Handle);
		ImGui::BeginGroup();
	}
	void AssetPanelItem::OnRender(const float& thumbnailSize)
	{
		const float edgeOffset = 4.0f;

		const float textLineHeight = ImGui::GetTextLineHeightWithSpacing() * 2.0f + edgeOffset * 2.0f;
		const float infoPanelHeight = Math::Max(false ? thumbnailSize * 0.5f : textLineHeight, textLineHeight);

		const ImVec2 topLeft = ImGui::GetCursorScreenPos();
		const ImVec2 thumbBottomRight = { topLeft.x + thumbnailSize, topLeft.y + thumbnailSize };
		const ImVec2 infoTopLeft	  = { topLeft.x,				 topLeft.y + thumbnailSize };
		const ImVec2 bottomRight	  = { topLeft.x + thumbnailSize, topLeft.y + thumbnailSize + infoPanelHeight };

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 4.0f });

		auto drawShadow = [&](const ImVec2& topLeft, const ImVec2& bottomRight, bool directory)
		{
			const Style& style = StyleManager::GetCurrent();
			auto* drawList = ImGui::GetWindowDrawList();
			const ImRect itemRect = UI::RectOffset(topLeft, bottomRight, 1.0f, 1.0f);
			drawList->AddRect(itemRect.Min, itemRect.Max, ImGui::ColorConvertFloat4ToU32(style.MenuBarBackground), 4.0f, directory ? 0 : ImDrawFlags_RoundCornersAll, 4.0f);
		};

		const Style& style = StyleManager::GetCurrent();

		if (GetType() != AssetType::Folder) {
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

		if (GetMetadata().Type == AssetType::Image) {
			ImGui::SetCursorPosY(yPos + edgeOffset);
			ShiftX(edgeOffset);
			UI::Image(AssetManager::GetAsset<Texture2D>(m_Handle)->GetSourceImage(), ImVec2(thumbnailSize - edgeOffset * 2.0, thumbnailSize - edgeOffset * 2.0), { 0, 1 }, {1, 0});
		}
		ShiftY(edgeOffset);
		UI::Separator({ thumbnailSize, 2.0f }, style.Colors.AxisX);
		Shift(edgeOffset, edgeOffset);

		if (GetType() == AssetType::Folder)
		{
			std::string name = GetName();
			ImGui::BeginVertical((std::string("InfoPanel") + name).c_str(), ImVec2(thumbnailSize - edgeOffset * 2.0f, infoPanelHeight - edgeOffset));
			{
				ImGui::BeginHorizontal(name.c_str(), ImVec2(thumbnailSize - 2.0f, 0.0f));
				ImGui::Spring();
				//Center text
				{
					ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 3.0f));

					const float textWidth = Math::Min(ImGui::CalcTextSize(name.c_str()).x, thumbnailSize);
					ImGui::SetNextItemWidth(textWidth);
					ImGui::Text(name.c_str());

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
			std::string name = GetName();
			ImGui::BeginVertical((std::string("InfoPanel") + name).c_str(), ImVec2(thumbnailSize - edgeOffset * 2.0f, infoPanelHeight - edgeOffset));
			{
				ImGui::BeginHorizontal("label", ImVec2(0.0f, 0.0f));
				ImGui::SuspendLayout();

				ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + (thumbnailSize - edgeOffset * 2.0f));
				//Asset name
				ImGui::Text(name.c_str());
				ImGui::PopTextWrapPos();

				ImGui::ResumeLayout();

				ImGui::Spring();
				ImGui::EndHorizontal();
			}
			ImGui::Spring();
			ImGui::EndVertical();
		}

		ImGui::EndGroup();

		ImGui::PopStyleVar();
	}
	void AssetPanelItem::EndRender()
	{
		ImGui::PopID();
		ImGui::NextColumn();
	}
}