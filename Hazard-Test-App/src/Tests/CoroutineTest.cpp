
#include "Hazard.h"
#include "CoroutineTest.h"


void CoroutineTest::Reset()
{

}

void CoroutineTest::Init()
{
    std::function<Coroutine()> fn = []() -> Coroutine {
        HZR_CORE_INFO("Started");
        
        co_await TestAwaitable();
        
        HZR_CORE_INFO("Finished");
        co_return;
    };
    
    m_Coroutine = fn();
    
    m_Coroutine.MoveNext();
}

void CoroutineTest::Run()
{

}

void CoroutineTest::Terminate()
{

}
