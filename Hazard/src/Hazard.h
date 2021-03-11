#pragma once

//Hazard Application and Module
#include "Hazard/Core/Application.h"
#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Module/Module.h"


//Hazard Event system
#include "Hazard/Events/Input.h"
#include "Hazard/Events/KeyCode.h"
#include "Hazard/Events/MouseCode.h"

//Hazard render engien includes
#include "Hazard/Rendering/RenderEngine.h"
#include "Hazard/Rendering/RenderCommand.h"
#include "Hazard/Rendering/RenderUtils.h"

#include "Hazard/RenderContext/RenderContext.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

//Hazard ECS includes
#include "Hazard/Entity/Loader/SceneHandler.h"
#include "Hazard/Entity/Loader/SceneCommand.h"
#include "Hazard/Entity/Loader/SceneSerializer.h"
#include "Hazard/Entity/Scene.h"
#include "Hazard/Entity/Entity.h"
#include "Hazard/Entity/Component.h"

//Hazadrd script engine
#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/Scripting/ScriptCommand.h"

//Hazard Math classes
#include "Hazard/Math/Color.h"
#include "Hazard/Math/Time.h"
#include "Hazard/Math/Math.h"
//Hazard utility
#include "Hazard/File/StringUtil.h"
#include "Hazard/File/File.h"