#pragma once

//Hazard Application and Module
#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Hazard/Module.h"
#include "Hazard/Core/CommandLineArgs.h"
#include "Hazard/Module.h"

#include "Hazard/Assets/AssetManager.h"

//Hazard ECS includes
#include "Hazard/ECS/WorldHandler.h"
#include "Hazard/ECS/Loader/WorldDeserializer.h"
#include "Hazard/ECS/Loader/WorldSerializer.h"
#include "Hazard/ECS/World.h"
#include "Hazard/ECS/Entity.h"
#include "Hazard/ECS/Component.h"

#include "Hazard/Rendering/Mesh/Mesh.h"
#include "Hazard/Rendering/WorldRenderer.h"
#include "Hazard/Rendering/RendererDrawList.h"

//Hazard Physics
#include "Hazard/Physics/PhysicsCommand.h"

#include "Backend/Core/Core.h"

#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/Scripting/ScriptUtilities.h"

#include <hzrpch.h>


//Hazard ImGui library
#include "Hazard/ImGUI/Panel.h"
#include "Hazard/ImGUI/PanelManager.h"
#include "Hazard/ImGUI/UILibrary.h"
#include "Hazard/ImGUI/StyleManager.h"
#include "Hazard/ImGUI/MenuBar.h"
#include "Hazard/ImGUI/Dockspace.h"
