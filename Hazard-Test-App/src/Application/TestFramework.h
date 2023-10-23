#pragma once

#include "Hazard/Core/Application.h"
#include "Hazard/Core/EntryPoint.h"

#include "Test.h"

class TestFramework : public Hazard::Application
{
public:
    TestFramework() = default;
    ~TestFramework() = default;

    void PreInit() override;
    void Init() override;
    void Update() override;

private: 
    void RestartTest();

private:
    uint32_t m_TestIndex = 4;
    std::vector<Test*> m_Tests;
    Test* m_CurrentTest = nullptr;
};

Hazard::Application* Hazard::CreateApplication()
{
    return hnew TestFramework();
}
