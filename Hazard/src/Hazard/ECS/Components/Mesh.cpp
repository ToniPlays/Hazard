#pragma once

#include <hzrpch.h>
#include "Mesh.h"

namespace Hazard {
	
	Mesh::Mesh()
	{

	}

	Mesh::~Mesh()
	{
		delete indices;
		delete vertices;
	}

}