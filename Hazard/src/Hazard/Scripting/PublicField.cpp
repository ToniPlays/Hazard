#pragma once

#include <hzrpch.h>
#include "PublicField.h"

#include "Mono/MonoCommands.h"

namespace Hazard::Scripting {

	void PublicField::CopyStoredToRuntimeValue()
	{
		MonoCommand::SetFieldValue(entityInstance->GetInstance(), monoClassField, storedValueBuffer);
	}

	bool PublicField::RuntimeAvailable()
	{
		return entityInstance->handle != 0;
	}

	void PublicField::SetStoredValueRaw(void* src)
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		memcpy(storedValueBuffer, src, size);
	}

	std::byte* PublicField::AllocateBuffer(VarFieldType type)
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		std::byte* buffer = new std::byte[size];
		memset(buffer, 0, size);
		return buffer;
	}

	void PublicField::SetStoredValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		memcpy(storedValueBuffer, value, size);
	}

	void PublicField::GetStoredValueInternal(void* value) const
	{
		uint32_t size = ScriptUtils::GetFieldSize(type);
		memcpy(value, storedValueBuffer, size);
	}

	void PublicField::SetRuntimeValueInternal(void* value) const
	{
		MonoCommand::SetFieldValue(entityInstance->GetInstance(), monoClassField, value);
	}

	void PublicField::GetRuntimeValueInternal(void* value) const
	{
		MonoCommand::GetFieldValue(entityInstance->GetInstance(), monoClassField, value);
	}
	 
	//ENTITY SCRIPT
	void EntityScript::InitClassMethods()
	{
		Constructor = MonoCommand::GetMonoCoreMethod("Hazard.Entity:.ctor(ulong)");
		OnUpdate = MonoCommand::GetMonoAppMethod(moduleName + ":OnUpdate()");
	}

	//ENTITY INSTANCE
	MonoObject* EntityInstance::GetInstance()
	{
		return MonoCommand::GetClassFromHandle(handle);
	}
}