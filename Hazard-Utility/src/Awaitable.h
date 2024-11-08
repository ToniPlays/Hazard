//
//  Awaitable.h
//  Hazard-Utility
//
//  Created by Toni Simoska on 9.11.2024.
//

#include <coroutine.h>
#include <condition_variable>

#include "Coroutine.h"
#include "Utility/Callback.h"

template<typename T>
class Awaitable
{
public:
    
    virtual void OnSuspend() {};
    
    T await_resume() noexcept { return false; }
    bool await_ready() { return false; }
    void await_suspend(Coroutine::handle_type handle)
    {
        m_OnResolved = [handle]() {
            if(--handle.promise().m_Dependencies)
                handle.resume();
        };
        
        OnSuspend();
    }
    
    void Resolve()
    {
        m_OnResolved();
    }
private:
    std::function<void()> m_OnResolved;
};
