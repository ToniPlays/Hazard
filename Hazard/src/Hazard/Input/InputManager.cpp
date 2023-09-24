#include <hzrpch.h>

#include "InputManager.h"
#include "KeyboardInputDevice.h"
#include "MouseInputDevice.h"
#include "GamepadInputDevice.h"

namespace Hazard
{
	InputManager::InputManager() : Module("InputManager")
	{
		m_ConnectedDevices[InputSource_Keyboard].push_back(Ref<KeyboardInputDevice>::Create());
		m_ConnectedDevices[InputSource_Mouse].push_back(Ref<MouseInputDevice>::Create());
	}
	void InputManager::Init()
	{
		for (auto& gamepad : Input::GetGamepads())
		{
			if (!gamepad.Connected) continue;

			GamepadConnectedEvent e(gamepad.Binding);
			GamepadConnected(e);
		}

		PostInit();
	}
	void InputManager::PostInit()
	{
		{
			InputAxis horizontal = {};
			horizontal.KeyCode = Gamepad::AxisLeftHorizontal;
			horizontal.Direction = InputAxisDirection_Horizontal;

			InputAxis vertical = {};
			vertical.KeyCode = Gamepad::AxisLeftVertical;
			vertical.Direction = InputAxisDirection_Vertical;

			InputAxis horizontalKB = {};
			horizontalKB.KeyCode = Key::D;
			horizontalKB.InverseKeyCode = Key::A;
			horizontalKB.Direction = InputAxisDirection_Horizontal;

			InputAxis verticalKB = {};
			verticalKB.KeyCode = Key::W;
			verticalKB.InverseKeyCode = Key::S;
			verticalKB.Direction = InputAxisDirection_Vertical;

			BindingGroup gpGroup = {};
			gpGroup.Type = InputDeviceType::Gamepad;
			gpGroup.Axis[0] = horizontal;
			gpGroup.Axis[1] = vertical;

			BindingGroup kbGroup = {};
			kbGroup.Type = InputDeviceType::Keyboard;
			kbGroup.Axis[0] = horizontalKB;
			kbGroup.Axis[1] = verticalKB;

			InputBinding& axisBinding = s_InputSchema.Bindings.emplace_back();
			axisBinding.Name = "Axis2D";
			axisBinding.Groups = { gpGroup, kbGroup };
			axisBinding.Callback = [](const InputBinding& axis, uint32_t binding) {
				glm::vec2 v = axis.GetAxis<glm::vec2>();
			};
		}
		{
			InputAxis horizontal = {};
			horizontal.KeyCode = Gamepad::A;
			horizontal.Direction = InputAxisDirection_Button;

			InputAxis horizontalKB = {};
			horizontalKB.KeyCode = Key::Space;
			horizontalKB.Direction = InputAxisDirection_Button;

			BindingGroup gpGroup = {};
			gpGroup.Type = InputDeviceType::Gamepad;
			gpGroup.Axis[0] = horizontal;

			BindingGroup kbGroup = {};
			kbGroup.Type = InputDeviceType::Keyboard;
			kbGroup.Axis[0] = horizontalKB;


			InputBinding& buttonBinding = s_InputSchema.Bindings.emplace_back();
			buttonBinding.Name = "Jump";
			buttonBinding.Groups = { gpGroup, kbGroup };
			buttonBinding.Callback = [](const InputBinding& axis, uint32_t binding) {

			};
			InvalidateSchema(s_InputSchema);
		}
	}
	void InputManager::Update()
	{
		for (auto& [type, devices] : m_ConnectedDevices)
		{
			for (auto& binding : s_InputSchema.Bindings)
			{
				if (!(binding.DeviceMask & type)) continue;

				for (auto& device : devices)
				{
					if (device->IsConnected())
						ProcessBinding(device, binding);
				}
			}
		}
	}
	bool InputManager::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<GamepadConnectedEvent>(BIND_EVENT(InputManager::GamepadConnected));
		dispatcher.Dispatch<GamepadDisconnectedEvent>(BIND_EVENT(InputManager::GamepadDisconnected));
		return false;
	}
	void InputManager::InvalidateSchema(InputSchema& schema)
	{
		for (auto& binding : schema.Bindings)
		{
			for (auto& group : binding.Groups)
			{
				schema.DeviceMask |= (uint32_t)group.Type;
				binding.DeviceMask |= (uint32_t)group.Type;

				for (auto& axis : group.Axis)
					binding.AxisMask |= (uint32_t)axis.Direction;
			}
		}
	}
	Ref<InputDevice> InputManager::GetGamepad(uint32_t binding)
	{
		return m_ConnectedDevices[InputSource_Gamepad][binding];
	}
	void InputManager::ProcessBinding(Ref<InputDevice> device, InputBinding& binding)
	{
		bool anyChange = false;
		for (auto& group : binding.Groups)
		{
			if (device->GetType() != group.Type) continue;

			for (auto& axis : group.Axis)
			{
				float currentValue = axis.Value;
				axis.Value = ProcessAxis(device, axis);
				anyChange |= glm::abs(currentValue - axis.Value) >= 0.0025f;
			}
		}
		if (anyChange)
			binding.Callback(binding, device->GetBindingPoint());
	}
	float InputManager::ProcessAxis(Ref<InputDevice> device, const InputAxis& axis)
	{
		return device->GetAxis(axis.KeyCode, axis.InverseKeyCode);
	}
	bool InputManager::GamepadConnected(GamepadConnectedEvent& e)
	{
		for (auto& device : m_ConnectedDevices[InputSource_Gamepad])
		{
			if (device->GetBindingPoint() != e.GetGamepadIndex()) continue;

			HZR_CORE_INFO("Gamepad connected: {}", e.GetGamepadIndex());
			device->OnConnected(e.GetGamepadIndex());
			return false;
		}

		Ref<GamepadInputDevice> device = Ref<GamepadInputDevice>::Create();
		device->OnConnected(e.GetGamepadIndex());
		m_ConnectedDevices[InputSource_Gamepad].emplace_back(device);
		HZR_CORE_INFO("New Gamepad connected: {}", e.GetGamepadIndex());
		return false;
	}
	bool InputManager::GamepadDisconnected(GamepadDisconnectedEvent& e)
	{
		HZR_CORE_INFO("Gamepad disconnected: {}", e.GetGamepadIndex());

		for (auto& device : m_ConnectedDevices[InputSource_Gamepad])
		{
			if (device->GetBindingPoint() != e.GetGamepadIndex()) continue;

			device->OnDisconnected();
			break;
		}

		return false;
	}
}