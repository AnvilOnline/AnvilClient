#pragma once
#include <string>

namespace AnvilCommon::Utils
{
	enum HookFlags : int32_t
	{
		None = 0,
		IsCall = 1,
		IsJmpIfEqual = 2
	};

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

		static bool WriteCall(void *p_Address, void *p_NewFunction);
		static bool WriteJump(void *p_Address, void *p_NewFunction, int p_Flags = 0);
		static bool ApplyHook(size_t p_Offset, void *p_DestFunc, int p_Flags = 0);

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