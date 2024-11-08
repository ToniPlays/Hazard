
#include "HazardRenderer.h"
#include "Application/Test.h"
#include "Coroutine.h"

class CoroutineTest : public Test
{
public:
    
    void Reset() override;
    void Init() override;
    void Run() override;
    void Terminate() override;

    const char* GetName() override { return "Coroutine test"; }

private:
    Coroutine m_Coroutine;
};
