#pragma once

#include "Core.h"
namespace Hazard {
	class HAZARD_API IApplication {
	public:
		virtual void Awake() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void OnClose() {};
	};
}