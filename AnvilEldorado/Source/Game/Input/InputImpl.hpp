#pragma once
#include <Interfaces\Initializable.hpp>
#include <cstdint>

namespace Blam::Input
{
	struct ActionState;
	struct BindingsTable;
	enum GameAction : uint8_t;
	enum InputType;
	enum KeyCode;
	enum KeyEvent;
}

namespace AnvilEldorado::Game::Input
{
	class InputImpl :
		public AnvilCommon::Initializable
	{
	public:
		virtual bool Init();

		uint8_t GetKeyTicks(Blam::Input::KeyCode key, Blam::Input::InputType type);
		uint16_t GetKeyMs(Blam::Input::KeyCode key, Blam::Input::InputType type);
		bool ReadKeyEvent(Blam::Input::KeyEvent* result, Blam::Input::InputType type);
		void BlockInput(Blam::Input::InputType type, bool block);
		bool GetBindings(int localPlayerIndex, Blam::Input::BindingsTable *result);
		bool SetBindings(int localPlayerIndex, const Blam::Input::BindingsTable &newBindings);
		Blam::Input::ActionState* GetActionState(Blam::Input::GameAction action);
	};
}