#pragma once

namespace Anvil
{
	class IInit
	{
	protected:
		~IInit()
		{
		}

	public:
		virtual bool Init() { return true; }
		virtual bool PreInit() { return true; }
		virtual bool PostInit() { return true; }
	};
}