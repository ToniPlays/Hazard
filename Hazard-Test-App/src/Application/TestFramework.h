#pragma once
#include "Hazard/Core/Application.h"

class TestFramework : public Hazard::Application
{
public:
    TestFramework() = default;
    ~TestFramework() = default;

    void PreInit() override;
    void Init() override;
};

Hazard::Application* Hazard::CreateApplication()
{
    return hnew TestFramework();
}
