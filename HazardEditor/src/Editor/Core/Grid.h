#pragma once

#include "Hazard.h"
using namespace Hazard;

class Grid : public Module {
public:
	Grid() : Module("Grid") {}
	~Grid();
	void Render();
	void OnEnabled() override;

private:
	Shader* shader;
	VertexArray* vertexArray;
};