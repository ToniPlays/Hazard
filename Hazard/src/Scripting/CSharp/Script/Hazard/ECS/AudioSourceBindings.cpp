
#include <hzrpch.h>
#include "AudioSourceBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"

namespace Hazard::Scripting::CSharp::Bindings {

	BindMap AudioSourceBindings::GetBindings()
	{
		BindMap result;
		result.reserve(11);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_Play_Native", (void*) AudioSource_Play_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_Stop_Native", (void*) AudioSource_Stop_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_Pause_Native", (void*) AudioSource_Pause_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_GetGain_Native", (void*) AudioSource_GetGain_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetGain_Native", (void*) AudioSource_SetGain_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_GetPitch_Native", (void*) AudioSource_GetPitch_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetPitch_Native", (void*) AudioSource_SetPitch_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_IsLoaded_Native", (void*) AudioSource_IsLoaded_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_IsLooping_Native", (void*) AudioSource_IsLooping_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetLooping_Native", (void*) AudioSource_SetLooping_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_IsSpatial_Native", (void*) AudioSource_IsSpatial_Native);
		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_SetSpatial_Native", (void*) AudioSource_SetSpatial_Native);

		result.emplace_back("Hazard.AudioSourceComponent::AudioSource_GetDuration_Native", (void*) AudioSource_GetDuration_Native);

		return result;
	}
	void AudioSourceBindings::AudioSource_Play_Native(uint32_t id)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		as.Source.Play();
	}
	void AudioSourceBindings::AudioSource_Stop_Native(uint32_t id)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		as.Source.Stop();
	}
	void AudioSourceBindings::AudioSource_Pause_Native(uint32_t id)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		as.Source.Pause();
	}
	float AudioSourceBindings::AudioSource_GetGain_Native(uint32_t id)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		return as.Gain;
	}
	void AudioSourceBindings::AudioSource_SetGain_Native(uint32_t id, float value)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		as.Gain = value;
		as.Source.SetGain(value);
	}
	float AudioSourceBindings::AudioSource_GetPitch_Native(uint32_t id)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		return as.Pitch;
	}
	void AudioSourceBindings::AudioSource_SetPitch_Native(uint32_t id, float value)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		as.Pitch = value;
		as.Source.SetPitch(value);
	}
	bool AudioSourceBindings::AudioSource_IsLoaded_Native(uint32_t id)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		return as.Source.IsLoaded();
	}
	bool AudioSourceBindings::AudioSource_IsLooping_Native(uint32_t id)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		return as.Looping;
	}
	void AudioSourceBindings::AudioSource_SetLooping_Native(uint32_t id, bool value)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		as.Looping = value;
		as.Source.SetLoop(value);
	}
	bool AudioSourceBindings::AudioSource_IsSpatial_Native(uint32_t id)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		return as.Spatial;
	}
	void AudioSourceBindings::AudioSource_SetSpatial_Native(uint32_t id, bool value)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		as.Spatial = value;
		as.Source.SetSpatial(value);
	}
	float AudioSourceBindings::AudioSource_GetDuration_Native(uint32_t id)
	{
		auto& as = ScriptCommand::EntityGetComponent<ECS::AudioSourceComponent>(id);
		return as.Source.GetTotalDuration();
	}
}
