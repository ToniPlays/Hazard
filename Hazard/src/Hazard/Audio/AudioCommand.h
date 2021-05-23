#pragma once

#include "Hazard/Entity/Component.h"

namespace Hazard::Audio {
	class AudioCommand {
	public:
		static AudioClip Create(ECS::AudioSourceComponent& component);
	};
}