#pragma once

#include "Hazard.h"

#include "Event.h"

inline static std::string GetBuildType()
{
#ifdef HZR_DEBUG
	return "Debug";
#elif HZR_DIST
	return "Distribution";
#elif HZR_RELEASE
	return "Release";
#else
	return "Unknown";
#endif
}


class HazardEditorApplication : public Hazard::Application {

public:
	HazardEditorApplication() = default;
	~HazardEditorApplication() = default;

	void PreInit() override;
	void Init() override;
	void Update() override;
	bool OnEvent(Event& e) override;

private:
	void InitJobsystemHooks();
	void InitializeGUIPanels();

private:
};
