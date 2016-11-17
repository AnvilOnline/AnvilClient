#pragma once
#include <Interfaces/IInitializable.hpp>

namespace AnvilEldorado::Patches
{
	class AnvilPatch : AnvilCommon::IInitializable
	{
	protected:
		void Patch_Tags();
		void Patch_Accounting();
		void Patch_Camera();
		void Patch_Weapons();
		void Patch_Audio();
		void Patch_Equipment();
		void Patch_Rendering();

	public:
		virtual bool Init() override;
	};
}