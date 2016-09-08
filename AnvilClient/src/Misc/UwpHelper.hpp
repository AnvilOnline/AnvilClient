#pragma once
#include <Misc/IInit.hpp>
#include <string>
#include <memory>

namespace Anvil
{
	class UwpHelper : public IInit
	{
	protected:
		std::wstring m_UserLocalPath;
		std::wstring m_LocalCachePath;

	public:
		static std::shared_ptr<UwpHelper> GetInstance();
		virtual bool Init() override;

		std::wstring& GetLocalUserPath();
		std::wstring& GetLocalCachePath();

	private:
		bool ReadDirectories();
	};
}
