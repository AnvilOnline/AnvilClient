#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Hook.hpp"
#include "Utils\Patch.hpp"
#include "Utils\Singleton.hpp"
#include "Module.hpp"
#include "Blam\Math\RealPoint3D.hpp"
#include "Blam\Math\RealVector2D.hpp"
#include "Blam\Math\RealVector3D.hpp"

namespace AnvilEldorado
{
	using AnvilCommon::Utils::Hook;
	using AnvilCommon::Utils::Patch;

	struct CameraGlobals
	{
		uint32_t Unknown1;
		size_t UpdatePerspectiveFunction;
		uint8_t Unknown2[0x830];
		Blam::Math::RealPoint3D Position;
		Blam::Math::RealVector3D PositionShift;
		Blam::Math::RealVector2D LookShift;
		float Depth;
		float FieldOfView;
		Blam::Math::RealVector3D Forward;
		Blam::Math::RealVector2D Up;
	};

	class Camera final : public Module, public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Camera>
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

		static CameraGlobals *GetCameraGlobals();

		Camera();

		bool Init() override;

		void UpdatePosition();
	};
}