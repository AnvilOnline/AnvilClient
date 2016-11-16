#pragma once
#include <Misc/IInitializable.hpp>

namespace Anvil::Client
{
	class AnvilPatch : IInitializable
	{
	protected:
		void Patch_Tags();
		void Patch_Accounting();
		void Patch_Preferences();
		void Patch_Ai();
		void Patch_Camera();
		void Patch_Weapons();
		void Patch_Audio();
		void Patch_Equipment();
		void Patch_Rendering();

	public:
		virtual bool PreInit() override;

		virtual bool Init() override;

		virtual bool PostInit() override;
	};
}