#pragma once

#include "InputSchema.h"
#include "Hazard/Module.h"
#include "Backend/Input.h"
#include "InputDevice.h"
#include "Backend/Core/Events/Events.h"

namespace Hazard
{
	class InputManager : public Module
	{
	public:
		InputManager();
		~InputManager() = default;

		void Init() override;
		void PostInit();
		void Update();

		bool OnEvent(Event& e) override;

		void InvalidateSchema(InputSchema& schema);

		static const std::unordered_map<InputSource, std::vector<Ref<InputDevice>>>& GetDevices() { return m_ConnectedDevices; }
		static Ref<InputDevice> GetGamepad(uint32_t binding);
		static InputSchema& GetSchema() { return s_InputSchema; }

	private:
		void ProcessBinding(Ref<InputDevice> device, InputBinding& binding);
		float ProcessAxis(Ref<InputDevice> device, const InputAxis& axis);

		bool GamepadConnected(GamepadConnectedEvent& e);
		bool GamepadDisconnected(GamepadDisconnectedEvent& e);

	private:
		inline static InputSchema s_InputSchema;
		inline static std::unordered_map<InputSource, std::vector<Ref<InputDevice>>> m_ConnectedDevices;
	};
}