#pragma once
#include <Interfaces/IInitializable.hpp>
#include <string>

namespace AnvilAusar
{
	class Engine : public AnvilCommon::IInitializable
	{
	private:
		std::string m_LocalAppDataPath;
		std::string m_LocalCachePath;

	protected:
		bool InitializeDirectories();

	public:
		virtual bool Init() override;
	};
}