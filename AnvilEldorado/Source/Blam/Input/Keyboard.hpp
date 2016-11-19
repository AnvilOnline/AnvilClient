#pragma once
#include <cstdint>
#include "Input.hpp"

namespace Blam::Input
{
	enum class KeyCode : int16_t
	{
		None = -1,
		Escape,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		PrintScreen,
		F14,
		F15,
		Tilde,
		Number1,
		Number2,
		Number3,
		Number4,
		Number5,
		Number6,
		Number7,
		Number8,
		Number9,
		Number0,
		Minus,
		Plus,
		Back,
		Tab,
		Q,
		W,
		E,
		R,
		T,
		Y,
		U,
		I,
		O,
		P,
		LBracket,
		RBracket,
		Pipe,
		Capital,
		A,
		S,
		D,
		F,
		G,
		H,
		J,
		K,
		L,
		Colon,
		Quote,
		Enter,
		LShift,
		Z,
		X,
		C,
		V,
		B,
		N,
		M,
		Comma,
		Period,
		Question,
		RShift,
		LControl,
		Unused46,
		LAlt,
		Space,
		RAlt,
		Unused4A,
		Apps,
		Rcontrol,
		Up,
		Down,
		Left,
		Right,
		Insert,
		Home,
		PageUp,
		Delete,
		End,
		PageDown,
		NumLock,
		Divide,
		Multiply,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		Subtract,
		Add,
		NumpadEnter,
		Decimal,
		Unused68,
		Shift,
		Ctrl,
		Unused6B,
		Alt,

		Count
	};

	enum class KeyEventModifiers : uint8_t
	{
		Shift = 1 << 0,
		Ctrl = 1 << 1,
		Alt = 1 << 2,
	};

	enum class KeyEventType : int32_t
	{
		Down,
		Up,
		Press
	};

	struct KeyEvent
	{
		KeyEventModifiers Modifiers : 8;
		KeyEventType Type : 32;
		KeyCode Code : 16;
		char16_t Char;
		bool PreviousState;

		bool Read(const InputType &p_Type);
	};
	static_assert(sizeof(KeyEvent) == 0x10, "Blam::Input::KeyEvent");

	uint8_t GetKeyTicks(const KeyCode &p_Key, const InputType &p_Type);

	uint16_t GetKeyMs(const KeyCode &p_Key, const InputType &p_Type);
}