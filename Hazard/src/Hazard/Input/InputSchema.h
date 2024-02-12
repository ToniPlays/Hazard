#pragma once

#include "UtilityCore.h"
#include "Input.h"
#include "InputDevice.h"

#include <functional>

namespace Hazard
{
	enum InputSource : uint32_t
	{
		InputSource_None = 0,
		InputSource_Keyboard = BIT(0),
		InputSource_Mouse = BIT(1),
		InputSource_Gamepad = BIT(2)
	};
	enum InputAxisDirection : uint32_t
	{
		InputAxisDirection_None = 0,
		InputAxisDirection_Button = BIT(0),
		InputAxisDirection_Horizontal = BIT(1),
		InputAxisDirection_Vertical = BIT(2),
		InputAxisDirection_Depth = BIT(3)
	};

	enum InputSourceFlags : uint32_t
	{
		INPUT_SOURCE_DISCONNECTED = BIT(0)
	};

	struct InputAxis
	{
		float Value = 0.0f;
		float Multiplier = 1.0f;
		bool Inverted = false;

		InputAxisDirection Direction;
		uint32_t KeyCode = UINT32_MAX;
		uint32_t InverseKeyCode = UINT32_MAX;

		float GetValueWithMultiplier() const
		{
			return Value * (Inverted ? -Multiplier : Multiplier);
		}
		float GetValue() const
		{
			return Value * (Inverted ? -1.0f : 1.0f);
		}
		bool Pressed() const
		{
			return GetValue() >= 0.5f;;
		}
	};

	struct BindingGroup
	{
		InputAxis Axis[3];
		InputDeviceType Type = InputDeviceType::None;
	};

	struct InputBinding
	{
		uint32_t DeviceMask = 0;
		uint32_t AxisMask = 0;

		std::string Name;
		std::vector<BindingGroup> Groups;
		std::function<void(const InputBinding&, uint32_t)> Callback;


		bool IsPressed() const
		{
			for (auto& group : Groups)
			{
				if (group.Axis[0].Pressed())
					return true;
			}
			return false;
		}

		template<typename T>
		T GetAxis() const
		{
			HZR_ASSERT(false, "T GetAxis() const");
		}
		template<>
		float GetAxis() const
		{
			for (auto& group : Groups)
			{
				float value = group.Axis[0].GetValueWithMultiplier();
				if (value != 0.0f)
					return value;
			}
			return 0.0f;
		}

		template<>
		glm::vec2 GetAxis() const
		{
			for (auto& group : Groups)
			{
				glm::vec2 value = {
					group.Axis[0].GetValueWithMultiplier(),
					group.Axis[1].GetValueWithMultiplier()
				};

				if (value.x != 0.0f || value.y != 0.0f)
					return value;
			}
			return { 0.0f, 0.0f };
		}
		template<>
		glm::vec3 GetAxis() const
		{
			for (auto& group : Groups)
			{
				glm::vec3 value = {
					group.Axis[0].GetValueWithMultiplier(),
					group.Axis[1].GetValueWithMultiplier(),
					group.Axis[2].GetValueWithMultiplier()
				};

				if (value.x != 0.0f || value.y != 0.0f || value.z != 0.0f)
					return value;
			}
			return { 0.0f, 0.0f, 0.0f };
		}
	};

	struct InputSchema
	{
		bool Enabled = true;
		uint32_t DeviceMask = 0;
		std::vector<InputBinding> Bindings;
	};

	static const char* InputSourceToString(uint32_t source)
	{
		switch (source)
		{
			case InputSource_None:		return "None";
			case InputSource_Keyboard:	return "Keyboard";
			case InputSource_Mouse:		return "Mouse";
			case InputSource_Gamepad:	return "Gamepad";
		}
        return "None";
	}
}
