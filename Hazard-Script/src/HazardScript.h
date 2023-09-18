#pragma once

#include "Core/HazardScriptCore.h"
#include "Core/HazardScriptEngine.h"
#include "Core/Instance/ScriptObject.h"
#include "Core/Metadata/FieldMetadata.h"
#include "Core/Metadata/MethodMetadata.h"
#include "Core/AttributeBuilder.h"
#include "Core/FieldValueStorage.h"

#define BIND_ICALL(x) assembly->AddInternalCall("Hazard.InternalCalls", #x, (void*)x)
