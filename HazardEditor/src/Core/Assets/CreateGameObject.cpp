#pragma once
#include "CreateGameObject.h"

void CreateGameObject::CreateEmpty()
{
	Hazard::Scene::GetCurrent()->AddGameObject(new Hazard::GameObject("New object"));
}

void CreateGameObject::CreateWithMesh()
{

}
