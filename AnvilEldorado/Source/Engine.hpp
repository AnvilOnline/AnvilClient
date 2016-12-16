#pragma once
#include <Interfaces/IInitializable.hpp>

namespace AnvilEldorado
{
	class Engine final : public AnvilCommon::IInitializable
	{
	private:
		bool ApplyCorePatches();
		bool ApplyCameraPatches();
		bool ApplyGraphicsPatches();
		bool ApplyInputPatches();
		bool ApplyNetworkPatches();
		bool ApplyContentPatches();
		bool ApplyUserInterfacePatches();
		bool ApplyArmorPatches();
		bool ApplyPlayerPatches();
		bool ApplyForgePatches();
		bool ApplyGameRulesPatches();
		bool ApplyScoreboardPatches();
		bool ApplyVirtualKeyboardPatches();
	
	public:
		inline bool Init() override
		{
			return ApplyCorePatches()
				&& ApplyCameraPatches()
				&& ApplyGraphicsPatches()
				&& ApplyInputPatches()
				&& ApplyNetworkPatches()
				&& ApplyContentPatches()
				&& ApplyUserInterfacePatches()
				&& ApplyArmorPatches()
				&& ApplyPlayerPatches()
				&& ApplyForgePatches()
				&& ApplyGameRulesPatches()
				&& ApplyScoreboardPatches()
				&& ApplyVirtualKeyboardPatches();
		}
	};
}