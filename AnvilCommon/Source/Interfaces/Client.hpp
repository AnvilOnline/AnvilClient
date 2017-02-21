#pragma once
#include "Initializable.hpp"

namespace AnvilCommon
{
	class Engine;

	class Client :
		public AnvilCommon::Initializable
	{
	public:
		virtual std::shared_ptr<AnvilCommon::Engine> GetEngine() = 0;
	};
}

extern __declspec(dllexport) std::shared_ptr<AnvilCommon::Client> GetClientInterface();