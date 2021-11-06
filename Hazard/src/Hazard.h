#pragma once

//Hazard Application and Module
#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Core/Application.h"
#include "Hazard/Core/ApplicationCreateInfo.h"
#include "Hazard/Module.h"

#include "Hazard/Assets/AssetManager.h"
#include "Hazard/Assets/Asset.h"

//Hazard Event system
#include "Hazard/Events/Input.h"
#include "Hazard/Events/KeyCode.h"
#include "Hazard/Events/MouseCode.h"

//Hazard render engine includes

#include "Hazard/Rendering/Image/Image2D.h"
#include "Hazard/Rendering/Pipeline/FrameBuffer.h"
#include "Hazard/Rendering/RenderCommand.h"

//Hazard render context window
#include "Hazard/RenderContext/RenderContext.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

#include "Platform/System/Window.h"

//Hazard ECS includes
#include "Hazard/Entity/WorldCommand.h"
#include "Hazard/Entity/WorldHandler.h"
#include "Hazard/Entity/Loader/WorldDeserializer.h"
#include "Hazard/Entity/Loader/WorldSerializer.h"
#include "Hazard/Entity/World.h"
#include "Hazard/Entity/Entity.h"
#include "Hazard/Entity/Component.h"

//Hazadrd script engine
#include "Hazard/Scripting/ScriptEngineManager.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "Hazard/Scripting/PublicField.h"

//Hazard Math classes
#include "Hazard/Math/Color.h"
#include "Hazard/Math/Time.h"
#include "Hazard/Math/Math.h"
//Hazard Physics
#include "Hazard/Physics/PhysicsCommand.h"

//Hazard utility
#include "Hazard/File/StringUtil.h"
#include "Hazard/File/YamlUtils.h"
#include "Hazard/File/File.h"