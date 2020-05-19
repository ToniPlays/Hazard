#pragma once
#include "Core.h"

namespace Hazard {
	class HAZARD_API HazardInterface {
	public:
		virtual void Start() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Close() {};
	};
}