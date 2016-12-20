#pragma once
#include "Interfaces\IInitializable.hpp"
#include "Utils\Singleton.hpp"
#include "Blam\Math\RealPoint3D.hpp"
#include "Blam\Math\RealVector2D.hpp"
#include "Blam\Math\RealVector3D.hpp"

namespace AnvilEldorado
{
	enum class CameraMode
	{
		FirstPerson,
		Following,
		Flying,
		Static
	};

	enum class CameraDefinitionType : int32_t
	{
		Position = 0,
		PositionShift = 1,
		LookShift = 2,
		Depth = 3,
		FieldOfView = 4,
		LookVectors = 5
	};

	struct CameraGlobals
	{
		uint32_t Unknown1;
		void *UpdatePerspectiveFunction;
		uint8_t Unknown2[0x830];
		Blam::Math::RealPoint3D Position;
		Blam::Math::RealVector3D PositionShift;
		Blam::Math::RealVector2D LookShift;
		float Depth;
		float FieldOfView;
		Blam::Math::RealVector3D Forward;
		Blam::Math::RealVector2D Up;
	};

	class Camera final : public AnvilCommon::IInitializable, public AnvilCommon::Singleton<Camera>
	{
	public:
		static CameraGlobals *GetCameraGlobals();

		bool Init() override;

		CameraMode GetCameraMode() const;
		void SetCameraMode(const CameraMode &p_Mode);

		bool IsHudVisible() const;
		void SetHudVisible(const bool p_Visible = true);

		bool IsReticleCentered() const;
		void SetReticleCentered(const bool p_Centered = true);

		float GetSpeed() const;
		void SetSpeed(const float p_Speed);

		float GetFieldOfView() const;
		void SetFieldOfView(const float p_FieldOfView);

		Blam::Math::RealPoint3D GetPosition() const;
		void SetPosition(const Blam::Math::RealPoint3D &p_Position);
		void SetPosition(const float p_X, const float p_Y, const float p_Z);

	private:
		CameraMode m_CameraMode;
	};
}