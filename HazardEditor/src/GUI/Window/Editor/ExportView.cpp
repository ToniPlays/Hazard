#pragma once
#include <hzreditor.h>
#include "ExportView.h"
#include "Editor/GameExport.h"
#include "GUI/Library/Input.h"

namespace WindowElement 
{
	ExportView::ExportView() : EditorWindow("Export application")
	{
		SetActive(false);
	}
	ExportView::~ExportView()
	{
	}
	void ExportView::OnWindowRender()
	{
		if (Input::Button("Export")) {
			std::string location = File::SaveFolderDialog();

			if (location != "") {

				std::vector<ECS::World*> worlds;
				worlds.push_back(&ECS::WorldCommand::GetCurrentWorld());

				GameExport::ExportGame(location, worlds);
			}
		}
	}
}
