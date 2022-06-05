#pragma once

class GUIRenderable 
{
public:
	GUIRenderable() = default;

	virtual void Update() {};
	virtual void Render() {};
};