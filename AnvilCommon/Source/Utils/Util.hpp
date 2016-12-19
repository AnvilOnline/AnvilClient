#pragma once
#include <string>

namespace AnvilCommon::Utils
{
	class Util
	{
	public:
		static bool HasCommandLine(std::string p_Command);

		static bool PatchAddressInMemory(const size_t p_Offset, const std::string &p_HexString, const int32_t p_Length);
		static bool PatchAddressInMemory(void *p_Address, const std::string &p_HexString, const int32_t p_Length);
		static bool PatchAddressInFile(const size_t p_Offset, const std::string &p_HexString, const int32_t p_Length);
		static bool PatchAddressInFile(void *p_Address, const std::string &p_HexString, const int32_t p_Length);
		static bool PatchAddress(const size_t p_Offset, const std::string &p_HexString, const int32_t p_Length, bool p_InMemory = false);
		static bool PatchAddress(void *p_Address, const std::string &p_HexString, const int32_t p_Length, bool p_InMemory = false);

		static bool ResumeAllThreads();

		template<class T>
		static int32_t CountBits(T p_Value)
		{
			int32_t result = 0;

			while (p_Value != 0)
			{
				p_Value >>= 1;
				result++;
			}

			return result;
		}
	};
}