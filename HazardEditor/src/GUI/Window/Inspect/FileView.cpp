#pragma once

#include <hzreditor.h>
#include "FileView.h"
#include "GUI/Library/Layout.h"

namespace WindowElement {

	FileView::FileView() : EditorWindow("File explorer")
	{
		rootPath = "UNDEFINED ROOT PATH";
	}
	FileView::~FileView()
	{

	}
	void FileView::OnWindowRender()
	{
		WindowLayout::Layout::Text(rootPath);
	}
}