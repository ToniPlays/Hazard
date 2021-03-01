#pragma once

#include <hzreditor.h>
#include "FileView.h"
#include "GUI/Library/Layout.h"

namespace WindowElement {

	FileView::FileView() : EditorWindow("Asset manager") {
	
	}

	FileView::~FileView()
	{

	}
	void FileView::OnWindowRender()
	{
		WindowLayout::Layout::Text(rootPath.c_str());
	}
}