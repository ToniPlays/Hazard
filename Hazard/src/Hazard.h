#pragma once

#include "Hazard/Core/Application.h"
#include "Hazard/Modules/Module.h"

#include "Hazard/Modules/Renderer/AssetManager.h"
#include "Hazard/Modules/Renderer/RendererAPI.h"
#include "Hazard/Modules/Renderer/RenderEngine.h"
#include "Hazard/Modules/Scene/SceneManager.h"
#include "Hazard/Modules/Logging/Logger.h"

#include "Hazard/ECS/Entity.h"
#include "Hazard/ECS/Components/SpriteRenderer.h"
#include "Hazard/ECS/Components/Mesh.h"
#include "Hazard/ECS/Components/MeshRenderer.h"
#include "Hazard/ECS/Components/CameraComponent.h"
#include "Hazard/ECS/Components/BatchRendererComponent.h"

#include "Hazard/Utils/Maths/Math.h"
#include "Hazard/Utils/Maths/Time.h"
#include "Hazard/Utils/Maths/Color.h"
#include "Hazard/Utils/Maths/Vector/Vector2.h"
#include "Hazard/Utils/Maths/Vector/Vector3.h"
#include "Hazard/Utils/Maths/Vector/Matrix4.h"
#include "Hazard/Utils/StringUtils.h"
#include "Hazard/Utils/Loaders/File.h"
#include "Hazard/Utils/Loaders/Serializer.h"
#include "Hazard/Utils/Loaders/Deserializer.h"

#include "Hazard/Core/Events/Event.h"
#include "Hazard/Core/Events/MouseEvent.h"
#include "Hazard/Core/Events/KeyEvent.h"
