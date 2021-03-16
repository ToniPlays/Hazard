#include "hzreditor.h"
#include "NodeEditor.h"

NodeEditor::NodeEditor()
{
	context = node::CreateEditor();
}

NodeEditor::~NodeEditor()
{
	node::DestroyEditor(context);
}

void NodeEditor::BeginEditor()
{

}

void NodeEditor::EndEditor()
{

}
