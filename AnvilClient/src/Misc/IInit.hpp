#pragma once

namespace Anvil
{
	class IInit
	{
	public:
		virtual ~IInit()
		{
		}

		virtual bool Init() { return true; }
		virtual bool PreInit() { return true; }
		virtual bool PostInit() { return true; }
	};
}