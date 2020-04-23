#pragma once

#define SYSTEM_GPU 0
#define SYSTEM_CPU 1
#define SYSTEM_RENDERER 2

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <stdio.h>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//Hazard stuff
#include "Core/Application.h"
#include "Core/Core.h"
#include "Utils/Time.h"
#include "Core/Logging/Log.h"
#include "Core/Layers/Layer.h"