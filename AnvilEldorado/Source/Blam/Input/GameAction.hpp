#pragma once
#include <cstdint>

namespace Blam::Input
{
	enum GameAction : uint8_t
	{
		// These actions mirror the ControllerButton enum

		eGameActionUiLeftTrigger,
		eGameActionUiRightTrigger,
		eGameActionUiUp,
		eGameActionUiDown,
		eGameActionUiLeft,
		eGameActionUiRight,
		eGameActionUiStart,
		eGameActionUiSelect,
		eGameActionUiLeftStick,
		eGameActionUiRightStick,
		eGameActionUiA,
		eGameActionUiB,
		eGameActionUiX,
		eGameActionUiY,
		eGameActionUiLeftBumper,
		eGameActionUiRightBumper,

		// In-game actions

		eGameActionJump,
		eGameActionSwitchGrenades,
		eGameActionSwitchWeapons,
		eGameActionUnk19,
		eGameActionReloadRight,
		eGameActionUse,
		eGameActionReloadLeft,
		eGameActionPickUpLeft,
		eGameActionMelee,
		eGameActionThrowGrenade,
		eGameActionFireRight,
		eGameActionFireLeft,
		eGameActionMeleeFire, // Fires a weapon with a trigger bound to the melee key
		eGameActionCrouch,
		eGameActionZoom,
		eGameActionUnk31,
		eGameActionUnk32,
		eGameActionSprint,
		eGameActionUnk34,
		eGameActionUnk35,
		eGameActionUnk36,
		eGameActionUnk37,
		eGameActionUnk38,
		eGameActionGeneralChat,
		eGameActionTeamChat,
		eGameActionUnk41,
		eGameActionUnk42,
		eGameActionUnk43,
		eGameActionUseConsumable1,
		eGameActionUseConsumable2,
		eGameActionUseConsumable3,
		eGameActionUseConsumable4,
		eGameActionVehicleBoost,
		eGameActionVehicleDive,
		eGameActionVehicleRaise,
		eGameActionVehicleAccelerate,
		eGameActionVehicleBrake,
		eGameActionVehicleFire,
		eGameActionVehicleAltFire,
		eGameActionVehicleExit,
		eGameActionUnk56,
		eGameActionUnk57,
		eGameActionUnk58,

		eGameAction_ControllerCount = 59,

		// These actions CANNOT be bound to controller buttons or else you
		// will overflow the controller bindings array! (Also, it seems
		// that mouse bindings will ignore these, even though there's room
		// for them.)

		eGameActionMoveForward = 59,
		eGameActionMoveBack,
		eGameActionMoveLeft,
		eGameActionMoveRight,

		eGameAction_KeyboardMouseCount = 63
	};
}