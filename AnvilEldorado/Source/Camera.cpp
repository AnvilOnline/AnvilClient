//#include <locale>
//#include <string>
//#include <sstream>
//
//#include "Utils\Hook.hpp"
//#include "Utils\Patch.hpp"
//
//#include "Blam\Math\RealPoint3D.hpp"
//#include "Blam\Math\RealVector2D.hpp"
//#include "Blam\Math\RealVector3D.hpp"
//
//#include "Globals.hpp"
//#include "Camera.hpp"
//
//namespace AnvilEldorado
//{
//	// Override the FOV that the memmove before this sets
//	__declspec(naked) void FovHook()
//	{
//		__asm
//		{
//			mov eax, ds:[0x189D42C]
//			mov ds : [0x2301D98], eax
//			mov ecx, [edi + 0x18]
//			push 0x50CA08
//			ret
//		}
//	}
//
//	// determine which camera definitions are editable based on the current camera mode
//	bool __stdcall IsCameraDefinitionEditable(const CameraDefinitionType &definition)
//	{
//		auto *s_Camera = Camera::Instance();
//		auto s_CameraMode = s_Camera->GetCameraMode();
//
//		if (s_CameraMode != CameraMode::FirstPerson || s_CameraMode != CameraMode::Following)
//		{
//			if (definition == CameraDefinitionType::PositionShift ||
//				definition == CameraDefinitionType::LookShift ||
//				definition == CameraDefinitionType::Depth)
//			{
//				return true;
//			}
//		}
//		else if (s_CameraMode != CameraMode::Flying || s_CameraMode != CameraMode::Static)
//		{
//			return true;
//		}
//
//		return false;
//	}
//
//	// hook @ 0x61440D - allows for the modification of specific camera components based on current perspective
//	__declspec(naked) void UpdateCameraDefinitionsHook()
//	{
//		__asm
//		{
//			pushad
//			shr		esi, 1						; convert camera definition offset to an index
//			push	esi							; CameraDefinitionType
//			call	IsCameraDefinitionEditable
//			test	al, al
//			popad
//			jnz		skip
//			mov		eax, [eax + ecx * 4]		; get data(definition + item * 4)
//			mov		[edi + ecx * 4], eax		; store it in the 3rd camera array
//			
//		skip:
//			push	0614413h
//			ret
//		}
//	}
//
//	// hook @ 0x614818 - allows for the modification of specific camera components based on current perspective
//	__declspec(naked) void UpdateCameraDefinitionsAlt1Hook()
//	{
//		__asm
//		{
//			pushad
//			shr		esi, 1
//			push	esi
//			call	IsCameraDefinitionEditable
//			test	al, al
//			popad
//			jnz		skip
//			movss   dword ptr [edx + eax * 4], xmm0
//			
//		skip:
//			push	061481Dh
//			ret
//		}
//	}
//
//	// hook @ 0x6148BE - allows for the modification of specific camera components based on current perspective
//	__declspec(naked) void UpdateCameraDefinitionsAlt2Hook()
//	{
//		__asm
//		{
//			pushad
//			shr		esi, 1
//			push	esi
//			call	IsCameraDefinitionEditable
//			test	al, al
//			popad
//			jnz		skip
//			movss   dword ptr [edx + eax * 4], xmm1
//			
//		skip:
//			push	06148C3h
//			ret
//		}
//	}
//
//	// hook @ 0x614902 - allows for the modification of specific camera components based on current perspective
//	__declspec(naked) void UpdateCameraDefinitionsAlt3Hook()
//	{
//		__asm
//		{
//			pushad
//			shr		esi, 1
//			push	esi
//			call	IsCameraDefinitionEditable
//			test	al, al
//			popad
//			jnz		skip
//			movss   dword ptr [edi + eax * 4], xmm0
//			
//		skip:
//			push	0614907h
//			ret
//		}
//	}
//	
//	/*bool VariableCameraCrosshairUpdate(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo)
//	{
//		unsigned long s_Value = Camera::Instance()->VarCameraCrosshair->ValueInt;
//
//		auto s_Status = std::string("disabled.");
//
//		auto s_StatusBool = s_Value != 0;
//		if (s_StatusBool)
//			s_Status = "enabled.";
//
//		Camera::Instance()->CenteredCrosshairPatch.Apply(!s_StatusBool);
//
//		p_ReturnInfo = "Centered crosshair " + s_Status;
//
//		return true;
//	}
//
//	bool VariableCameraFovUpdate(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo)
//	{
//		auto s_Value = Camera::Instance()->VarCameraFov->ValueFloat;
//
//		*(float *)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x1F01D98) = s_Value;
//		*(float *)((uint8_t *)AnvilCommon::Internal_GetModuleStorage() + 0x149D42C) = s_Value;
//
//		return true;
//	}
//
//	bool VariableCameraHideHudUpdate(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo)
//	{
//		auto s_Value = Camera::Instance()->VarCameraHideHud->ValueInt;
//
//		auto status = std::string("shown.");
//
//		auto statusBool = s_Value != 0;
//		if (statusBool)
//			status = "hidden.";
//
//		Camera::Instance()->HideHudPatch.Apply(!statusBool);
//
//		p_ReturnInfo = "HUD " + status;
//
//		return true;
//	}
//
//	bool VariableCameraSpeedUpdate(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo)
//	{
//		auto s_Speed = Camera::Instance()->VarCameraSpeed->ValueFloat;
//
//		std::stringstream ss;
//		ss << "Camera speed set to " << s_Speed;
//
//		p_ReturnInfo = ss.str();
//
//		return true;
//	}
//
//	bool VariableCameraPositionUpdate(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo)
//	{
//		using Blam::Math::RealPoint3D;
//
//		auto *s_CameraGlobals = Camera::GetCameraGlobals();
//
//		auto s_Result = false;
//
//		if (p_Arguments.size() >= 1 || p_Arguments.size() <= 3)
//		{
//			std::stringstream ss;
//			ss << "X: " << s_CameraGlobals->Position.X << ", Y: " << s_CameraGlobals->Position.Y << ", Z: " << s_CameraGlobals->Position.Z;
//
//			p_ReturnInfo = ss.str();
//
//			s_Result = true;
//		}
//
//		return s_Result;
//	}
//
//	bool VariableCameraModeUpdate(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo)
//	{
//		using Blam::Math::RealPoint3D;
//		using Blam::Math::RealVector2D;
//		using Blam::Math::RealVector3D;
//
//		auto *s_ModuleCamera = Camera::Instance();
//		auto s_CameraMode = s_ModuleCamera->VarCameraMode->ValueString;
//
//		std::locale s_Locale;
//		for (std::string::size_type i = 0; i < s_CameraMode.length(); ++i)
//			s_CameraMode[i] = std::tolower(s_CameraMode[i], s_Locale);
//
//		// prevent the game from updating certain camera values depending on the current camera mode
//		s_ModuleCamera->CameraPermissionHook.Apply(s_CameraMode == "default");
//		s_ModuleCamera->CameraPermissionHookAlt1.Apply(s_CameraMode == "default");
//		s_ModuleCamera->CameraPermissionHookAlt2.Apply(s_CameraMode == "default");
//		s_ModuleCamera->CameraPermissionHookAlt3.Apply(s_CameraMode == "default");
//
//		// prevent the game from automatically switching camera modes depending on the current mode
//		s_ModuleCamera->Debug1CameraPatch.Apply(s_CameraMode == "default");
//		s_ModuleCamera->Debug2CameraPatch.Apply(s_CameraMode == "default");
//		s_ModuleCamera->ThirdPersonPatch.Apply(s_CameraMode == "default");
//		s_ModuleCamera->FirstPersonPatch.Apply(s_CameraMode == "default");
//		s_ModuleCamera->DeadPersonPatch.Apply(s_CameraMode == "default");
//
//		// hides the hud when flying or in static camera mode
//		s_ModuleCamera->HideHudPatch.Apply(s_CameraMode != "flying" && s_CameraMode != "static");
//
//		// prevents death from resetting look angles when in static camera mode
//		s_ModuleCamera->StaticILookVectorPatch.Apply(s_CameraMode != "static");
//		s_ModuleCamera->StaticKLookVectorPatch.Apply(s_CameraMode != "static");
//
//		// disable player movement while in flycam
//		auto *s_PlayerControlGlobals = AnvilCommon::GetThreadStorage<uint8_t>(0xC4);
//		*(bool *)(s_PlayerControlGlobals + 6) = (s_CameraMode == "flying");
//
//		// TODO: Map out camera globals structure
//		auto *s_CameraGlobals = Camera::GetCameraGlobals();
//
//		// get new camera perspective function offset
//
//		if (!s_CameraMode.compare("first")) // c_first_person_camera
//		{
//			s_CameraGlobals->UpdatePerspectiveFunction = 0x166ACB0;
//			s_CameraGlobals->PositionShift.I = 0.0f;
//			s_CameraGlobals->PositionShift.J = 0.0f;
//			s_CameraGlobals->PositionShift.K = 0.0f;
//			s_CameraGlobals->LookShift.I = 0.0f;
//			s_CameraGlobals->LookShift.J = 0.0f;
//			s_CameraGlobals->Depth = 0.0f;
//		}
//		else if (!s_CameraMode.compare("third")) // c_following_camera
//		{
//			s_CameraGlobals->UpdatePerspectiveFunction = 0x16724D4;
//			s_CameraGlobals->PositionShift.I = 0.0f;
//			s_CameraGlobals->PositionShift.J = 0.0f;
//			s_CameraGlobals->PositionShift.K = 0.0f;
//			s_CameraGlobals->LookShift.I = 0.0f;
//			s_CameraGlobals->LookShift.J = 0.0f;
//			s_CameraGlobals->Depth = 0.5f;
//			s_CameraGlobals->FieldOfView = 1.91986218f;
//		}
//		else if (!s_CameraMode.compare("flying")) // c_flying_camera
//		{
//			s_CameraGlobals->UpdatePerspectiveFunction = 0x16726D0;
//			s_CameraGlobals->PositionShift.I = 0.0f;
//			s_CameraGlobals->PositionShift.J = 0.0f;
//			s_CameraGlobals->PositionShift.K = 0.0f;
//			s_CameraGlobals->LookShift.I = 0.0f;
//			s_CameraGlobals->LookShift.J = 0.0f;
//			s_CameraGlobals->Depth = 0.0f;
//		}
//		else if (!s_CameraMode.compare("static")) // c_static_camera
//		{
//			s_CameraGlobals->UpdatePerspectiveFunction = 0x16728A8;
//			s_CameraGlobals->PositionShift.I = 0.0f;
//			s_CameraGlobals->PositionShift.J = 0.0f;
//			s_CameraGlobals->PositionShift.K = 0.0f;
//			s_CameraGlobals->LookShift.I = 0.0f;
//			s_CameraGlobals->LookShift.J = 0.0f;
//			s_CameraGlobals->Depth = 0.0f;
//		}
//
//		return true;
//	}*/
//
//	CameraGlobals *Camera::GetCameraGlobals()
//	{
//		static CameraGlobals *s_CameraGlobals = nullptr;
//
//		if (s_CameraGlobals == nullptr)
//			s_CameraGlobals = AnvilCommon::GetModuleStorage<CameraGlobals>(0x60);
//
//		return s_CameraGlobals;
//	}
//	
//	/*Camera::Camera()
//	{
//		VarCameraCrosshair = AddVariableInt(
//			"Crosshair", "crosshair",
//			"Controls whether the crosshair should be centered",
//			CommandFlags::Archived, 0, VariableCameraCrosshairUpdate);
//
//		VarCameraCrosshair->ValueIntMin = 0;
//		VarCameraCrosshair->ValueIntMax = 1;
//
//		VarCameraFov = AddVariableFloat(
//			"FOV", "fov",
//			"The camera's field of view",
//			CommandFlags::Archived, 90.f, VariableCameraFovUpdate);
//
//		VarCameraFov->ValueFloatMin = 55.f;
//		VarCameraFov->ValueFloatMax = 155.f;
//
//		VarCameraHideHud = AddVariableInt(
//			"HideHUD", "hud",
//			"Toggles the HUD",
//			CommandFlags::Archived, 0, VariableCameraHideHudUpdate);
//
//		VarCameraHideHud->ValueIntMin = 0;
//		VarCameraHideHud->ValueIntMax = 1;
//
//		VarCameraSpeed = AddVariableFloat(
//			"Speed", "camera_speed",
//			"The camera speed",
//			CommandFlags::Archived, 0.1f, VariableCameraSpeedUpdate);
//
//		VarCameraSpeed->ValueFloatMin = 0.01f;
//		VarCameraSpeed->ValueFloatMax = 5.0f;
//
//		VarCameraPosition = AddCommand(
//			"Position", "camera_position",
//			"The cameras position, Doesn't work when camera mode is set to default",
//			CommandFlags::None, VariableCameraPositionUpdate,
//			{ "X Coordinate", "Y Coordinate", "Z Coordinate" });
//
//		VarCameraMode = AddVariableString(
//			"Mode", "camera_mode",
//			"Camera mode, valid modes: default, first, third, flying, static",
//			(CommandFlags::DontUpdateInitial | CommandFlags::Cheat), "default", VariableCameraModeUpdate);
//	}*/
//
//	bool Camera::Init()
//	{
//		using AnvilCommon::Utils::Hook;
//		using AnvilCommon::Utils::Patch;
//
//		/*
//		CameraPermissionHook = Hook(0x21440D, UpdateCameraDefinitionsHook);
//		CameraPermissionHookAlt1 = Hook(0x214818, UpdateCameraDefinitionsAlt1Hook);
//		CameraPermissionHookAlt2 = Hook(0x2148BE, UpdateCameraDefinitionsAlt2Hook);
//		CameraPermissionHookAlt3 = Hook(0x214902, UpdateCameraDefinitionsAlt3Hook);
//		Debug1CameraPatch = Patch(0x325A80, 0x90, 6);
//		Debug2CameraPatch = Patch(0x191525, 0x90, 6);
//		ThirdPersonPatch = Patch(0x328640, 0x90, 6);
//		FirstPersonPatch = Patch(0x25F420, 0x90, 6);
//		DeadPersonPatch = Patch(0x329E6F, 0x90, 6);
//		StaticILookVectorPatch = Patch(0x211433, 0x90, 8);
//		StaticKLookVectorPatch = Patch(0x21143E, 0x90, 6);
//		HideHudPatch = Patch(0x12B5A5C, { 0xC3, 0xF5, 0x48, 0x40 }); // 3.14f in hex form
//		CenteredCrosshairPatch = Patch(0x25FA43, { 0x31, 0xC0, 0x90, 0x90 });
//		*/
//
//			// Prevent FOV from being overridden when the game loads
//		return Patch::NopFill(0x25FA79, 10)
//			&& Patch::NopFill(0x25FA86, 5)
//			&& Hook(0x10CA02, FovHook).Apply();
//	}
//
//	/*void Camera::UpdatePosition()
//	{
//		auto *s_ModuleCamera = Camera::Instance();
//
//		auto s_CameraMode = s_ModuleCamera->VarCameraMode->ValueString;
//
//		std::locale s_Locale;
//		for (std::string::size_type i = 0; i < s_CameraMode.length(); ++i)
//			s_CameraMode[i] = std::tolower(s_CameraMode[i], s_Locale);
//
//		// only allow camera input while flying
//		if (s_CameraMode.compare("flying"))
//			return;
//		
//		auto s_MoveDelta = Camera::Instance()->VarCameraSpeed->ValueFloat;
//
//		// TODO: Map out camera globals structure
//		auto *s_CameraGlobals = Camera::GetCameraGlobals();
//
//		// current values
//		float hLookAngle = playerControlGlobalsPtr(0x30C).Read<float>();
//		float vLookAngle = playerControlGlobalsPtr(0x310).Read<float>();
//		float iRight = cos(hLookAngle + 3.14159265359f / 2);
//		float jRight = sin(hLookAngle + 3.14159265359f / 2);
//
//		// TODO: use shockfire's keyboard hooks instead
//
//		// down
//		if (GetAsyncKeyState('Q') & 0x8000)
//		{
//			zPos -= s_MoveDelta;
//		}
//
//		// up
//		if (GetAsyncKeyState('E') & 0x8000)
//		{
//			zPos += s_MoveDelta;
//		}
//
//		// forward
//		if (GetAsyncKeyState('W') & 0x8000)
//		{
//			xPos += iForward * s_MoveDelta;
//			yPos += jForward * s_MoveDelta;
//			zPos += kForward * s_MoveDelta;
//		}
//
//		// back
//		if (GetAsyncKeyState('S') & 0x8000)
//		{
//			xPos -= iForward * s_MoveDelta;
//			yPos -= jForward * s_MoveDelta;
//			zPos -= kForward * s_MoveDelta;
//		}
//
//		// left
//		if (GetAsyncKeyState('A') & 0x8000)
//		{
//			xPos += iRight * s_MoveDelta;
//			yPos += jRight * s_MoveDelta;
//		}
//
//		// right
//		if (GetAsyncKeyState('D') & 0x8000)
//		{
//			xPos -= iRight * s_MoveDelta;
//			yPos -= jRight * s_MoveDelta;
//		}
//
//		if (GetAsyncKeyState(VK_UP))
//		{
//			// TODO: look up
//		}
//		if (GetAsyncKeyState(VK_DOWN))
//		{
//			// TODO: look down
//		}
//		if (GetAsyncKeyState(VK_LEFT))
//		{
//			// TODO: look left
//		}
//		if (GetAsyncKeyState(VK_RIGHT))
//		{
//			// TODO: look right
//		}
//
//		if (GetAsyncKeyState('Z') & 0x8000)
//		{
//			fov -= 0.003f;
//		}
//		if (GetAsyncKeyState('C') & 0x8000)
//		{
//			fov += 0.003f;
//		}
//
//		// update position
//		directorGlobalsPtr(0x834).Write<float>(xPos);
//		directorGlobalsPtr(0x838).Write<float>(yPos);
//		directorGlobalsPtr(0x83C).Write<float>(zPos);
//
//		// update look angles
//		directorGlobalsPtr(0x85C).Write<float>(cos(hLookAngle) * cos(vLookAngle));
//		directorGlobalsPtr(0x860).Write<float>(sin(hLookAngle) * cos(vLookAngle));
//		directorGlobalsPtr(0x864).Write<float>(sin(vLookAngle));
//		directorGlobalsPtr(0x868).Write<float>(-cos(hLookAngle) * sin(vLookAngle));
//		directorGlobalsPtr(0x86C).Write<float>(-sin(hLookAngle) * sin(vLookAngle));
//		directorGlobalsPtr(0x870).Write<float>(cos(vLookAngle));
//
//		directorGlobalsPtr(0x858).Write<float>(fov);
//	}*/
//
//	CameraMode Camera::GetCameraMode() const
//	{
//		return m_CameraMode;
//	}
//
//	void Camera::SetCameraMode(const CameraMode &p_Mode)
//	{
//		m_CameraMode = p_Mode;
//
//		// TODO: Apply changes after switching camera modes
//	}
//}
