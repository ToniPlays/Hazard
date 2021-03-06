#pragma once

#include <hzreditor.h>
#include "FileView.h"
#include "GUI/Library/Layout.h"

namespace WindowElement {

	FileView::FileView() : EditorWindow(ICON_FK_FOLDER_OPEN" Asset manager") {
	
	}

	FileView::~FileView()
	{

	}
	void FileView::OnWindowRender()
	{
		WindowLayout::Layout::Text(rootPath.c_str());
	}
}