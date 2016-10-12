#pragma once

namespace AsuarSDK
{
	class Initializer
	{
	private:
		static bool InitializeFunctions();
		static bool InitializePointers();

	public:
		static bool Initialize();
	};
}