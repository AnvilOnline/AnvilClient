#pragma once

namespace Anvil
{
	class IInitializable
	{
	public:
		virtual bool PreInit() { return true; }
		virtual bool Init() = 0;
		virtual bool PostInit() { return true; }

		virtual ~IInitializable()
		{

		}
	};
}