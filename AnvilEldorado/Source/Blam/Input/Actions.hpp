#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum Action : uint8_t
	{
		UiLeftTrigger,
		UiRightTrigger,
		UiUp,
		UiDown,
		UiLeft,
		UiRight,
		UiStart,
		UiSelect,
		UiLeftStick,
		UiRightStick,
		UiA,
		UiB,
		UiX,
		UiY,
		UiLeftBumper,
		UiRightBumper,
		Jump,
		SwitchGrenades,
		SwitchWeapons,
		Unknown19,
		ReloadRight,
		Use,
		ReloadLeft,
		PickUpLeft,
		Melee,
		ThrowGrenade,
		FireRight,
		FireLeft,
		MeleeFire,
		Crouch,
		Zoom,
		Unknown31,
		Unknown32,
		Sprint,
		Unknown34,
		Unknown35,
		Unknown36,
		Unknown37,
		Unknown38,
		GeneralChat,
		TeamChat,
		Unknown41,
		Unknown42,
		Unknown43,
		UseConsumable1,
		UseConsumable2,
		UseConsumable3,
		UseConsumable4,
		VehicleBoost,
		VehicleDive,
		VehicleRaise,
		VehicleAccelerate,
		VehicleBrake,
		VehicleFire,
		VehicleAltFire,
		VehicleExit,
		Unknown56,
		Unknown57,
		Unknown58,

		ControllerCount = 59,

		MoveForward = 59,
		MoveBack,
		MoveLeft,
		MoveRight,

		KeyboardMouseCount = 63
	};

	enum class ActionStateFlags : uint8_t
	{
		None,
		Handled = 1 << 0
	};

	struct ActionState
	{
		uint16_t Msec;
		uint8_t Ticks;
		ActionStateFlags Flags : 8;
		float Unknown4;
		uint8_t Unknown8;
		uint8_t Unknown9;
		uint16_t UnknownA;
	};
	static_assert(sizeof(ActionState) == 0xC, "Blam::Input::ActionState");

	struct ConfigurableAction
	{
		const char *Name;
		int DefaultPrimaryKey;
		int DefaultSecondaryKey;
		Action Action;

		ConfigurableAction();
	};
	static_assert(sizeof(ConfigurableAction) == 0x10, "Blam::Input::ConfigurableAction");

	ActionState *GetActionState(const Action &p_Action);
}