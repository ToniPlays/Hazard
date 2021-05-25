#pragma once
#include <hzrpch.h>
#include "AudioSourceBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"

namespace Hazard::Scripting::CSharp::Bindings {

	BindMap AudioSourceBindings::GetBindings()
	{
		BindMap result;
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_Play_Native", AudioSource_Play_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_Stop_Native", AudioSource_Stop_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_Pause_Native", AudioSource_Pause_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_GetGain_Native", AudioSource_GetGain_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetGain_Native", AudioSource_SetGain_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_GetPitch_Native", AudioSource_GetPitch_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetPitch_Native", AudioSource_SetPitch_Native);

		return result;
	}
	void AudioSourceBindings::AudioSource_Play_Native(uint32_t id)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).source.Play();
	}
	void AudioSourceBindings::AudioSource_Stop_Native(uint32_t id)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).source.Stop();
	}
	void AudioSourceBindings::AudioSource_Pause_Native(uint32_t id)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).source.Pause();
	}
	float AudioSourceBindings::AudioSource_GetGain_Native(uint32_t id)
	{
		return ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).source.GetGain();
	}
	void AudioSourceBindings::AudioSource_SetGain_Native(uint32_t id, float value)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).source.SetGain(value);
	}
	float AudioSourceBindings::AudioSource_GetPitch_Native(uint32_t id)
	{
		return ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).source.GetPitch();
	}
	void AudioSourceBindings::AudioSource_SetPitch_Native(uint32_t id, float value)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).source.SetPitch(value);
	}
}