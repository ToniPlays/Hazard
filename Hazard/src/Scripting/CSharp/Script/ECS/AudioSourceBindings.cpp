#pragma once
#include <hzrpch.h>
#include "AudioSourceBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"

namespace Hazard::Scripting::CSharp::Bindings {

	BindMap AudioSourceBindings::GetBindings()
	{
		BindMap result;
		result.reserve(11);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_Play_Native", AudioSource_Play_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_Stop_Native", AudioSource_Stop_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_Pause_Native", AudioSource_Pause_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_GetGain_Native", AudioSource_GetGain_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetGain_Native", AudioSource_SetGain_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_GetPitch_Native", AudioSource_GetPitch_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetPitch_Native", AudioSource_SetPitch_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_IsLoaded_Native", AudioSource_IsLoaded_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_IsLooping_Native", AudioSource_IsLooping_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetLooping_Native", AudioSource_SetLooping_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_IsSpatial_Native", AudioSource_IsSpatial_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetSpatial_Native", AudioSource_SetSpatial_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_GetDuration_Native", AudioSource_GetDuration_Native);

		return result;
	}
	void AudioSourceBindings::AudioSource_Play_Native(uint32_t id)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->Play();
	}
	void AudioSourceBindings::AudioSource_Stop_Native(uint32_t id)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->Stop();
	}
	void AudioSourceBindings::AudioSource_Pause_Native(uint32_t id)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->Pause();
	}
	float AudioSourceBindings::AudioSource_GetGain_Native(uint32_t id)
	{
		return ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->GetGain();
	}
	void AudioSourceBindings::AudioSource_SetGain_Native(uint32_t id, float value)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->SetGain(value);
	}
	float AudioSourceBindings::AudioSource_GetPitch_Native(uint32_t id)
	{
		return ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->GetPitch();
	}
	void AudioSourceBindings::AudioSource_SetPitch_Native(uint32_t id, float value)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->SetPitch(value);
	}
	bool AudioSourceBindings::AudioSource_IsLoaded_Native(uint32_t id)
	{
		return ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->IsLoaded();
	}
	bool AudioSourceBindings::AudioSource_IsLooping_Native(uint32_t id)
	{
		return ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->IsLooping();
	}
	void AudioSourceBindings::AudioSource_SetLooping_Native(uint32_t id, bool value)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->SetLoop(value);
	}
	bool AudioSourceBindings::AudioSource_IsSpatial_Native(uint32_t id)
	{
		return ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->IsSpatial();
	}
	void AudioSourceBindings::AudioSource_SetSpatial_Native(uint32_t id, bool value)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->SetSpatial(value);
	}
	void AudioSourceBindings::AudioSource_GetDuration_Native(uint32_t id)
	{
		ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id).AudioClip->GetTotalDuration();
	}
}