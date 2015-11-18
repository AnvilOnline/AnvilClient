// TODO: Verify

#pragma once
#include "simulation_entity_definition.h"

class simulation_sandbox_engine_globals_definition : public simulation_entity_definition
{
protected:
	~simulation_sandbox_engine_globals_definition() { }

public:
	void* m_SlayerEngineGlobals;
	void* m_CtfEngineGlobals;
	void* m_AssaultEngineGlobals;
	void* m_OddballEngineGlobals;
	void* m_KingEngineGlobals;
	void* m_TerritoriesEngineGlobals;
	void* m_JuggernautEngineGlobals;


	void* m_SandboxEngineGlobals;
	void* m_InfectionEngineGlobals;
	void* m_VipEngineGlobals;

	void* m_GameEnginePlayerEntity;
	void* m_GameStatborgEntity;

	void* m_BreakableSurfaceGroup;

	void* m_MapVariantEntity;
	void* m_UnitEntity;
	void* m_ItemEntity;
	void* m_GenericEntity;
	void* m_GenericGarbageEntity;
	void* m_VehicleEntity;
	void* m_ProjectileEntity;
	void* m_WeaponEntity;
	void* m_DeviceEntity;

	void* m_DamageAftermathEvent;
	void* m_DamageSectionResponse;

	void* m_ProjectileAttachedEvent;
	void* m_ProjectileDetonateEvent;
	void* m_ProjectileImpactEvent;

	void* m_ProjectileObjectImpactEffectEvent;
	void* m_EffectOnPosEvent;
	void* m_GameEngineEvent;
	void* m_UnitBoardVehicleEvent;
	void* m_UnitPickupEvent;
	void* m_BreakableSurfaceDamageEvent;
	void* m_WeaponEffectEvent;
	void* m_WeaponEmptyClickEvent;
	void* m_HsScriptWakeEvent;
	void* m_HitMarkerEvent;
	void* m_UnitExitVehicleEvent;
	void* m_UnitAssassinateEvent;
	void* m_PlayerTauntRequestEvent;
	void* m_WeaponFireEvent;
	void* m_WeaponReloadEvent;
	void* m_UnitThrowInitiateEvent;
	void* m_UnitMeleeInitiateEvent;
	void* m_WeaponPickupEvent;
	void* m_WeaponPutAwayEvent;
	void* m_WeaponDropEvent;
	void* m_WeaponTetherRequestEvent;
	void* m_VehicleFlipEvent;
	void* m_VehicleTrickEvent;
	void* m_DeviceTouchEvent;
	void* m_UnitThrowReleaseEvent;
	void* m_UnitMeleeDamageEvent;
	void* m_UnitMeleeClangEvent;
	void* m_UnitEnterVehicleEvent;
	void* m_GameEngineRequestBootPlayerEvent;
	void* m_UnitEquipmentUseEvent;
	void* m_UnitEquipmentPickupEvent;
	void* m_PlayerRespawnRequestEvent;
	void* m_PlayerForceBaseRespawnEvent;
	void* m_PlayerEditorRequestEvent;
	void* m_ProjectileSupercombineRequestEvent;
	void* m_ObjectRefreshEvent;
	void* m_AuthorityIgnoredPredictionPositionEvent;
};