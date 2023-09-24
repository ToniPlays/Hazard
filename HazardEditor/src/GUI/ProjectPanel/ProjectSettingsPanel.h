#pragma region

#include "Hazard.h"
#include "Hazard/ImGUI/Panel.h"
#include "Hazard/ImGUI/UIElements/TextField.h"
#include "Hazard/Input/InputSchema.h"

namespace UI
{
	class ProjectSettingsPanel : public Hazard::ImUI::Panel
	{
	public:
		ProjectSettingsPanel();
		~ProjectSettingsPanel() = default;

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

		bool KeyPressed(KeyPressedEvent& e);
		bool MouseButtonPressed(MouseButtonPressedEvent& e);
		bool GamepadButtonPressed(GamepadButtonPressedEvent& e);
		bool GamepadAxisMoved(GamepadAxisMovedEvent& e);
		void WaitForInput(Hazard::InputDeviceType device, Hazard::InputAxis* axis);

	private:
		void RenderNavbar();
		void RenderPageHeader(const char* title, const char* description);
		void DrawApplicationSettings();
		void DrawInputSettingsPanel();
		void DrawRuntimeBuildSettings();
		void DrawGraphicsQualitySettings();
		void DrawGraphicsLightingSettings();
		void DrawGraphicsShadowSettings();
		void DrawPhysicsProperties();
		void DrawPhysicsCollisionLayers();
		void DrawScriptingPreprocessors();
		void DrawScriptingDependencies();
		void DrawScriptingExecutionOrder();


	private:
		Hazard::ImUI::TextField m_SearchField;

		enum SettingsPage
		{
			GENERAL_APP_PROPERTIES,
			GENERAL_INPUT_SETTINGS,
			RUNTIME_BUILD_SETTINGS,
			RENDERING_QUALITY_SETTINGS,
			RENDERING_LIGHTING_SETTINGS,
			RENDERING_SHADOW_SETTINGS,
			PHYSICS_PROPERTIES,
			PHYSICS_COLLISION_LAYERS,
			SCRIPTING_PREPROCESSORS,
			SCRIPTING_DEPENDENCIES,
			SCRIPTING_EXECUTION_ORDER
		};
		SettingsPage m_CurrentPage = GENERAL_APP_PROPERTIES;

		Key::KeyCode m_PreviousKey = Key::None;
		Mouse::MouseCode m_PreviousMouseKey = Mouse::None;
		Gamepad::GamepadCode m_PreviousGamepadKey = Gamepad::None;
		Hazard::InputAxis* m_PendingAxis = nullptr;
		Hazard::InputDeviceType m_PendingType = Hazard::InputDeviceType::None;
	};
}