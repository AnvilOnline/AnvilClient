#pragma once
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"
#include "Utils\Singleton.hpp"
#include "ModuleBase.hpp"

namespace AnvilEldorado::Modules
{
	using AnvilCommon::Utils::Hook;
	using AnvilCommon::Utils::Patch;

	class ModuleCamera final : public ModuleBase, public AnvilCommon::Singleton<ModuleCamera>
	{
	public:
		Command* VarCameraCrosshair;
		Command* VarCameraFov;
		Command* VarCameraHideHud;
		Command* VarCameraMode;
		Command* VarCameraSpeed;
		Command* VarCameraSave;
		Command* VarCameraLoad;
		Command* VarCameraPosition;

		// patches to stop camera mode from changing
		Patch Debug1CameraPatch;
		Patch Debug2CameraPatch;
		Patch ThirdPersonPatch;
		Patch FirstPersonPatch;
		Patch DeadPersonPatch;

		Patch StaticILookVectorPatch;
		Patch StaticKLookVectorPatch;

		Patch HideHudPatch;
		Patch CenteredCrosshairPatch;

		Hook CameraPermissionHook;
		Hook CameraPermissionHookAlt1;
		Hook CameraPermissionHookAlt2;
		Hook CameraPermissionHookAlt3;

		ModuleCamera();

		void UpdatePosition();
	};
}