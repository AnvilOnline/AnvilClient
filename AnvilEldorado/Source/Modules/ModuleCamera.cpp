#include <locale>
#include <string>
#include <sstream>
#include "Blam\Math\RealPoint3D.hpp"
#include "Blam\Math\RealVector2D.hpp"
#include "Blam\Math\RealVector3D.hpp"
#include "ModuleCamera.hpp"

namespace AnvilEldorado::Modules
{
	enum class CameraDefinitionType : int32_t
	{
		Position = 0,
		PositionShift = 1,
		LookShift = 2,
		Depth = 3,
		FieldOfView = 4,
		LookVectors = 5
	};

	// determine which camera definitions are editable based on the current camera mode
	bool __stdcall IsCameraDefinitionEditable(const CameraDefinitionType &definition)
	{
		auto s_CameraMode = Modules::ModuleCamera::Instance()->VarCameraMode->ValueString;

		std::locale s_Locale;
		for (std::string::size_type i = 0; i < s_CameraMode.length(); ++i)
			s_CameraMode[i] = std::tolower(s_CameraMode[i], s_Locale);

		if (!s_CameraMode.compare("first") || !s_CameraMode.compare("third"))
		{
			if (definition == CameraDefinitionType::PositionShift ||
				definition == CameraDefinitionType::LookShift ||
				definition == CameraDefinitionType::Depth)
			{
				return true;
			}
		}
		else if (!s_CameraMode.compare("flying") || !s_CameraMode.compare("static"))
		{
			return true;
		}

		return false;
	}

	// hook @ 0x61440D - allows for the modification of specific camera components based on current perspective
	__declspec(naked) void UpdateCameraDefinitions()
	{
		__asm
		{
			pushad
			shr		esi, 1						; convert camera definition offset to an index
			push	esi							; CameraDefinitionType
			call	IsCameraDefinitionEditable
			test	al, al
			popad
			jnz		skip
			mov		eax, [eax + ecx * 4]		; get data(definition + item * 4)
			mov		[edi + ecx * 4], eax		; store it in the 3rd camera array
			
		skip:
			push	0614413h
			ret
		}
	}

	// hook @ 0x614818 - allows for the modification of specific camera components based on current perspective
	__declspec(naked) void UpdateCameraDefinitionsAlt1()
	{
		__asm
		{
			pushad
			shr		esi, 1
			push	esi
			call	IsCameraDefinitionEditable
			test	al, al
			popad
			jnz		skip
			movss   dword ptr [edx + eax * 4], xmm0
			
		skip:
			push	061481Dh
			ret
		}
	}

	// hook @ 0x6148BE - allows for the modification of specific camera components based on current perspective
	__declspec(naked) void UpdateCameraDefinitionsAlt2()
	{
		__asm
		{
			pushad
			shr		esi, 1
			push	esi
			call	IsCameraDefinitionEditable
			test	al, al
			popad
			jnz		skip
			movss   dword ptr [edx + eax * 4], xmm1
			
		skip:
			push	06148C3h
			ret
		}
	}

	// hook @ 0x614902 - allows for the modification of specific camera components based on current perspective
	__declspec(naked) void UpdateCameraDefinitionsAlt3()
	{
		__asm
		{
			pushad
			shr		esi, 1
			push	esi
			call	IsCameraDefinitionEditable
			test	al, al
			popad
			jnz		skip
			movss   dword ptr [edi + eax * 4], xmm0
			
		skip:
			push	0614907h
			ret
		}
	}

	bool VariableCameraCrosshairUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		unsigned long value = Modules::ModuleCamera::Instance()->VarCameraCrosshair->ValueInt;

		std::string status = "disabled.";
		bool statusBool = value != 0;
		if (statusBool)
			status = "enabled.";

		Modules::ModuleCamera::Instance()->CenteredCrosshairPatch.Apply(!statusBool);

		returnInfo = "Centered crosshair " + status;
		return true;
	}

	bool VariableCameraFovUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		float value = Modules::ModuleCamera::Instance()->VarCameraFov->ValueFloat;

		*(float *)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x1F01D98) = value;
		*(float *)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x149D42C) = value;

		return true;
	}

	bool VariableCameraHideHudUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		unsigned long value = Modules::ModuleCamera::Instance()->VarCameraHideHud->ValueInt;

		std::string status = "shown.";
		bool statusBool = value != 0;
		if (statusBool)
			status = "hidden.";

		Modules::ModuleCamera::Instance()->HideHudPatch.Apply(!statusBool);

		returnInfo = "HUD " + status;
		return true;
	}

	bool VariableCameraSpeedUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		float speed = Modules::ModuleCamera::Instance()->VarCameraSpeed->ValueFloat;

		std::stringstream ss;
		ss << "Camera speed set to " << speed;
		returnInfo = ss.str();

		return true;
	}

	bool VariableCameraPositionUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		using Blam::Math::RealPoint3D;

		auto *s_CameraGlobals = AnvilCommon::GetThreadStorage<uint8_t>(0x60);
		auto &s_CameraPosition = *(RealPoint3D *)(s_CameraGlobals + 0x834);

		if (Arguments.size() < 1 || Arguments.size() > 3)
		{
			std::stringstream ss;
			ss << "X: " << s_CameraPosition.X << ", Y: " << s_CameraPosition.Y << ", Z: " << s_CameraPosition.Z;
			returnInfo = ss.str();
			return false;
		}

		// update position
		s_CameraPosition.X = stof(Arguments[0]);
		s_CameraPosition.Y = stof(Arguments[1]);
		s_CameraPosition.Z = stof(Arguments[2]);// Z

		return true;
	}

	bool VariableCameraModeUpdate(const std::vector<std::string>& Arguments, std::string& returnInfo)
	{
		using Blam::Math::RealPoint3D;
		using Blam::Math::RealVector2D;
		using Blam::Math::RealVector3D;

		auto *s_ModuleCamera = ModuleCamera::Instance();
		auto s_CameraMode = s_ModuleCamera->VarCameraMode->ValueString;

		std::locale s_Locale;
		for (std::string::size_type i = 0; i < s_CameraMode.length(); ++i)
			s_CameraMode[i] = std::tolower(s_CameraMode[i], s_Locale);

		// prevent the game from updating certain camera values depending on the current camera mode
		s_ModuleCamera->CameraPermissionHook.Apply(s_CameraMode == "default");
		s_ModuleCamera->CameraPermissionHookAlt1.Apply(s_CameraMode == "default");
		s_ModuleCamera->CameraPermissionHookAlt2.Apply(s_CameraMode == "default");
		s_ModuleCamera->CameraPermissionHookAlt3.Apply(s_CameraMode == "default");

		// prevent the game from automatically switching camera modes depending on the current mode
		s_ModuleCamera->Debug1CameraPatch.Apply(s_CameraMode == "default");
		s_ModuleCamera->Debug2CameraPatch.Apply(s_CameraMode == "default");
		s_ModuleCamera->ThirdPersonPatch.Apply(s_CameraMode == "default");
		s_ModuleCamera->FirstPersonPatch.Apply(s_CameraMode == "default");
		s_ModuleCamera->DeadPersonPatch.Apply(s_CameraMode == "default");

		// hides the hud when flying or in static camera mode
		s_ModuleCamera->HideHudPatch.Apply(s_CameraMode != "flying" && s_CameraMode != "static");

		// prevents death from resetting look angles when in static camera mode
		s_ModuleCamera->StaticILookVectorPatch.Apply(s_CameraMode != "static");
		s_ModuleCamera->StaticKLookVectorPatch.Apply(s_CameraMode != "static");

		// disable player movement while in flycam
		auto *s_PlayerControlGlobals = AnvilCommon::GetThreadStorage<uint8_t>(0xC4);
		*(bool *)(s_PlayerControlGlobals + 6) = (s_CameraMode == "flying");

		// TODO: Map out camera globals structure
		auto *s_CameraGlobals = AnvilCommon::GetThreadStorage<uint8_t>(0x60);
		auto *s_CameraUpdatePerspFunc = (size_t *)(s_CameraGlobals + 4);
		auto *s_CameraPosition = (RealPoint3D *)(s_CameraGlobals + 0x834);
		auto *s_CameraPositionShift = (RealVector3D *)(s_CameraGlobals + 0x840);
		auto *s_CameraLookShift = (RealVector2D *)(s_CameraGlobals + 0x84C);
		auto *s_CameraDepth = (float *)(s_CameraGlobals + 0x854);
		auto *s_Camera110Degrees = (float *)(s_CameraGlobals + 0x858);

		// get new camera perspective function offset

		if (!s_CameraMode.compare("first")) // c_first_person_camera
		{
			*s_CameraUpdatePerspFunc = 0x166ACB0;
			s_CameraPositionShift->I = 0.0f;
			s_CameraPositionShift->J = 0.0f;
			s_CameraPositionShift->K = 0.0f;
			s_CameraLookShift->I = 0.0f;
			s_CameraLookShift->J = 0.0f;
			*s_CameraDepth = 0.0f;
		}
		else if (!s_CameraMode.compare("third")) // c_following_camera
		{
			*s_CameraUpdatePerspFunc = 0x16724D4;
			s_CameraPositionShift->I = 0.0f;
			s_CameraPositionShift->J = 0.0f;
			s_CameraPositionShift->K = 0.1f;
			s_CameraLookShift->I = 0.0f;
			s_CameraLookShift->J = 0.0f;
			*s_CameraDepth = 0.5f;
			*s_Camera110Degrees = 1.91986218f;
		}
		else if (!s_CameraMode.compare("flying")) // c_flying_camera
		{
			*s_CameraUpdatePerspFunc = 0x16726D0;
			s_CameraPositionShift->I = 0.0f;
			s_CameraPositionShift->J = 0.0f;
			s_CameraPositionShift->K = 0.0f;
			s_CameraLookShift->I = 0.0f;
			s_CameraLookShift->J = 0.0f;
			*s_CameraDepth = 0.0f;
		}
		else if (!s_CameraMode.compare("static")) // c_static_camera
		{
			*s_CameraUpdatePerspFunc = 0x16728A8;
			s_CameraPositionShift->I = 0.0f;
			s_CameraPositionShift->J = 0.0f;
			s_CameraPositionShift->K = 0.0f;
			s_CameraLookShift->I = 0.0f;
			s_CameraLookShift->J = 0.0f;
			*s_CameraDepth = 0.0f;
		}

		return true;
	}

	ModuleCamera::ModuleCamera() : ModuleBase("Camera"),
		CameraPermissionHook(0x21440D, UpdateCameraDefinitions),
		CameraPermissionHookAlt1(0x214818, UpdateCameraDefinitionsAlt1),
		CameraPermissionHookAlt2(0x2148BE, UpdateCameraDefinitionsAlt2),
		CameraPermissionHookAlt3(0x214902, UpdateCameraDefinitionsAlt3),
		Debug1CameraPatch(0x325A80, 0x90, 6),
		Debug2CameraPatch(0x191525, 0x90, 6),
		ThirdPersonPatch(0x328640, 0x90, 6),
		FirstPersonPatch(0x25F420, 0x90, 6),
		DeadPersonPatch(0x329E6F, 0x90, 6),
		StaticILookVectorPatch(0x211433, 0x90, 8),
		StaticKLookVectorPatch(0x21143E, 0x90, 6),
		HideHudPatch(0x12B5A5C, { 0xC3, 0xF5, 0x48, 0x40 }), // 3.14f in hex form
		CenteredCrosshairPatch(0x25FA43, { 0x31, 0xC0, 0x90, 0x90 })
	{
		// TODO: commands for setting camera speed, positions, save/restore etc.

		VarCameraCrosshair = AddVariableInt("Crosshair", "crosshair", "Controls whether the crosshair should be centered", eCommandFlagsArchived, 0, VariableCameraCrosshairUpdate);
		VarCameraCrosshair->ValueIntMin = 0;
		VarCameraCrosshair->ValueIntMax = 1;

		VarCameraFov = AddVariableFloat("FOV", "fov", "The cameras field of view", eCommandFlagsArchived, 90.f, VariableCameraFovUpdate);
		VarCameraFov->ValueFloatMin = 55.f;
		VarCameraFov->ValueFloatMax = 155.f;

		VarCameraHideHud = AddVariableInt("HideHUD", "hud", "Toggles the HUD", eCommandFlagsArchived, 0, VariableCameraHideHudUpdate);
		VarCameraHideHud->ValueIntMin = 0;
		VarCameraHideHud->ValueIntMax = 1;

		VarCameraSpeed = AddVariableFloat("Speed", "camera_speed", "The camera speed", eCommandFlagsArchived, 0.1f, VariableCameraSpeedUpdate);
		VarCameraSpeed->ValueFloatMin = 0.01f;
		VarCameraSpeed->ValueFloatMax = 5.0f;

		VarCameraPosition = AddCommand("Position", "camera_position", "The cameras position, Doesn't work when camera mode is set to default", eCommandFlagsNone, VariableCameraPositionUpdate, { "X Coordinate", "Y Coordinate", "Z Coordinate" });

		this->VarCameraMode = AddVariableString("Mode", "camera_mode", "Camera mode, valid modes: default, first, third, flying, static",
			(CommandFlags)(eCommandFlagsDontUpdateInitial | eCommandFlagsCheat), "default", VariableCameraModeUpdate);
	}

	void ModuleCamera::UpdatePosition()
	{

	}
}