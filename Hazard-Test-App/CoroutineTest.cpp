
#include "CoroutineTest.h"


void CoroutineTest::Reset()
{

}
void CoroutineTest::Init()
{
    std::function<Coroutine()> fn = []() -> Coroutine {
        std::cout << "Running" << std::endl;
        
        std::cout << "Finished" << std::endl;
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
