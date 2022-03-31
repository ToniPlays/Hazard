#pragma once

#include <iostream>

#define THROW_EXCEPT(x) throw std::runtime_error(std::string("[Hazard-Utility]: " x))
#ifdef HZR_DEBUG
#define HZR_ASSERT(x, m) if(!(x)) THROW_EXCEPT(m)
#else
#define HZR_ASSERT(x, m) x
#endif


template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }
using byte = uint8_t;
