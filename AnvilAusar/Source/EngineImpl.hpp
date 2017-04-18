#pragma once
#include <Interfaces/Engine.hpp>
#include <string>

namespace AnvilAusar
{
	class EngineImpl : 
		public AnvilCommon::Engine
	{
	private:
		std::string m_LocalAppDataPath;
		std::string m_LocalCachePath;

	protected:
		bool InitializeDirectories();

	public:
		virtual bool Init();

		virtual uint8_t* ExecutableBase();
		virtual size_t ExecutableSize();
	};
}