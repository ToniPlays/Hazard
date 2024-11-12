
#pragma once

#include "HazardRenderer.h"
#include "Application/Test.h"

#include "Utility/Awaitable.h"
#include "Utility/Coroutine.h"

class TestAwaitable : public Awaitable<int>
{
public:
    void OnSuspend() override {        
        std::thread t([this]() {
            std::this_thread::sleep_for(2000ms);
            Resolve();
        });
        t.detach();
    }
};


class CoroutineTest : public Test
{
public:
    
    void Reset() override;
    void Init() override;
    void Run() override;
    void Terminate() override;

    const char* GetName() override { return "Compute shader test"; }

private:
    Coroutine m_Coroutine;
};
