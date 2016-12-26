#pragma once
#include <stack>
#include <vector>
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"
#include "Blam\Input\InputTypes.hpp"

namespace AnvilEldorado
{
	// Base class for objects which override game input.
	// While a context is active, all game input is blocked.
	class InputContext
	{
	public:
		virtual ~InputContext() { }

		// Called when the input context is made active.
		virtual void Activated() = 0;

		// Called when the input context is deactivated.
		virtual void Deactivated() = 0;

		// Called on the active context each time the game processes input,
		// overriding the default input processing routine. If this returns
		// false, the input context will be deactivated.
		//
		// The difference between this and UiInputTick() is that this
		// cannot call Blam::Input::ReadKeyEvent() because Windows messages
		// won't have been pumped yet. However, this happens earlier on in
		// the engine update cycle.
		virtual bool GameInputTick() = 0;

		// Called on the active context each time Windows messages are
		// pumped. If this returns false, the input context will be
		// deactivated.
		virtual bool UiInputTick() = 0;
	};

	class Input final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Input>
	{
	public:
		bool Init() override;

		std::stack<std::shared_ptr<InputContext>> &GetContextStack();

		void PushContext(std::shared_ptr<InputContext> p_Context);
		void PopContext();

		bool IsContextDone() const;
		void SetContextDone(const bool p_Done = true);

		void SetInputTypeBlocked(const Blam::Input::InputType &p_Type, const bool p_Blocked = true);

		typedef std::function<void()> DefaultInputHandler;

		std::vector<DefaultInputHandler> &GetDefaultHandlers();

		// Registers a function to be called when the default input handler is
		// ticked.
		void RegisterDefaultInputHandler(DefaultInputHandler p_Handler);

		// Sets what controls can be configured in the keyboard settings menu.
		void SetKeyboardSettingsMenu(
			const std::vector<Blam::Input::ConfigurableAction> &infantrySettings,
			const std::vector<Blam::Input::ConfigurableAction> &vehicleSettings);

		bool RawInputEnabled() const;
		void SetRawInputEnabled(const bool p_Enabled = true);

		int32_t GetControllerPort() const;
		void SetControllerPort(const int32_t p_Port);

		float GetControllerHorizontalSensitivity() const;
		void SetControllerHorizontalSensitivity(const float p_Sensitivity);

		float GetControllerVerticalSensitivity() const;
		void SetControllerVerticalSensitivity(const float p_Sensitivity);

		Blam::Input::BindingsTable &GetBindings();
		void LoadDefaultBindings();
		void UpdateBindings();
		void CopyBinding(const Blam::Input::GameAction &p_SourceAction, const Blam::Input::GameAction &p_DestAction);

		bool BindControllerAction(const Blam::Input::GameAction &p_GameAction, const Blam::Input::ControllerButton &p_ControllerButton);
		bool BindKeyboardAction(const Blam::Input::GameAction &p_GameAction, const Blam::Input::KeyCode &p_PrimaryKey, const Blam::Input::KeyCode &p_SecondaryKey = Blam::Input::eKeyCode_None);
		bool BindMouseAction(const Blam::Input::GameAction &p_GameAction, const Blam::Input::MouseButton &p_PrimaryButton, const Blam::Input::MouseButton &p_SecondaryButton = Blam::Input::eMouseButton_None);

	private:
		bool m_RawInputEnabled;
		int32_t m_ControllerPort;
		Blam::Input::BindingsTable m_Bindings;
		std::vector<Blam::Input::ConfigurableAction> m_Settings;
		std::stack<std::shared_ptr<InputContext>> m_ContextStack;
		std::vector<DefaultInputHandler> m_DefaultHandlers;
		bool m_ContextDone = false;
		bool m_BlockStates[Blam::Input::eInputType_Count];
		Blam::Input::InputType m_ActionInputTypes[Blam::Input::eGameAction_KeyboardMouseCount];
	};
}