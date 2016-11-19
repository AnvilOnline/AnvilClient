#include "Actions.hpp"

namespace Blam::Input
{
	ConfigurableAction::ConfigurableAction()
		: Name(nullptr)
	{
	}

	ActionState *GetActionState(const Action &p_Action)
	{
		if (p_Action < 0 || p_Action >= Action::KeyboardMouseCount)
			return nullptr;

		return &reinterpret_cast<ActionState*>(0x244D1F0)[p_Action];
	}
}