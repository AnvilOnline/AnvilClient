#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Utils\Logger.hpp"
#include "Patch.hpp"

namespace AnvilCommon::Utils
{
	Patch::Patch()
		: m_Offset(0)
	{
	}

	Patch::Patch(const size_t p_Offset, const std::initializer_list<uint8_t> &p_PatchData, const std::initializer_list<uint8_t> &p_OriginalData)
		: m_Offset(p_Offset), m_PatchData(p_PatchData), m_OriginalData(p_OriginalData)
	{
	}

	Patch::Patch(const size_t p_Offset, const std::initializer_list<uint8_t> &p_PatchData, void *p_BaseAddress)
		: m_Offset(p_Offset), m_PatchData(p_PatchData)
	{
		m_OriginalData.resize(m_PatchData.size());
		memcpy(m_OriginalData.data(), (uint8_t *)p_BaseAddress + m_Offset, m_OriginalData.size());
	}

	Patch::Patch(const size_t p_Offset, const uint8_t p_Value, const size_t p_Length, void *p_BaseAddress)
		: m_Offset(p_Offset)
	{
		m_PatchData.resize(p_Length);
		std::fill(m_PatchData.begin(), m_PatchData.end(), p_Value);

		m_OriginalData.resize(m_PatchData.size());
		memcpy(m_OriginalData.data(), (uint8_t *)p_BaseAddress + p_Offset, m_OriginalData.size());
	}

	bool Patch::NopFill(const size_t p_Offset, const size_t p_Length, void *p_BaseAddress)
	{
		if (p_Length <= 0)
		{
			WriteLog("invalid patch data.");
			return false;
		}

		// Hold our previous protections
		auto s_Protection = 0;
		auto *s_Address = (uint8_t *)p_BaseAddress + p_Offset;
		// Change the protection so we can write to the executable without crashing
		auto s_Ret = VirtualProtect(s_Address, p_Length, PAGE_EXECUTE_READWRITE, reinterpret_cast<PDWORD>(&s_Protection));
		if (!s_Ret)
		{
			WriteLog("could not change protection to patch address %p (%x).", s_Address, GetLastError());
			return false;
		}

		WriteLog("changed protection of %x with a length of %x.", s_Address, p_Length);

		// Copy our patch over
		memset(s_Address, 0x90, p_Length);

		// Change the protections back
		s_Ret = VirtualProtect(s_Address, p_Length, s_Protection, reinterpret_cast<PDWORD>(&s_Protection));
		if (!s_Ret)
		{
			WriteLog("could not change protection back to default for address %p, be warned.", s_Address);
			return false;
		}

		// Patch succcessfully applied
		return true;
	}

	bool Patch::Apply(const bool p_Reset, void *p_BaseAddress) const
	{
		if (p_Reset)
			return Reset(p_BaseAddress);

		auto s_Length = m_PatchData.size();

		if (s_Length <= 0)
		{
			WriteLog("invalid patch data.");
			return false;
		}

		// Hold our previous protections
		auto s_Protection = 0;
		auto *s_Address = (uint8_t *)p_BaseAddress + m_Offset;
		// Change the protection so we can write to the executable without crashing
		auto s_Ret = VirtualProtect(s_Address, s_Length, PAGE_EXECUTE_READWRITE, reinterpret_cast<PDWORD>(&s_Protection));
		if (!s_Ret)
		{
			WriteLog("could not change protection to patch address %p (%x).", s_Address, GetLastError());
			return false;
		}

		WriteLog("changed protection of %x with a length of %x.", s_Address, s_Length);

		// Copy our patch over
		s_Ret = memcpy_s(s_Address, s_Length, m_PatchData.data(), s_Length);
		if (s_Ret)
		{
			WriteLog("could not write patch to address %p.", s_Address);
			return false;
		}

		// Change the protections back
		s_Ret = VirtualProtect(s_Address, s_Length, s_Protection, reinterpret_cast<PDWORD>(&s_Protection));
		if (!s_Ret)
		{
			WriteLog("could not change protection back to default for address %p, be warned.", s_Address);
			return false;
		}

		// Patch succcessfully applied
		return true;
	}

	bool Patch::Reset(void *p_BaseAddress) const
	{
		auto s_Length = m_OriginalData.size();

		if (s_Length <= 0)
		{
			WriteLog("invalid patch data.");
			return false;
		}

		// Hold our previous protections
		auto s_Protection = 0;
		auto *s_Address = (uint8_t *)p_BaseAddress + m_Offset;
		// Change the protection so we can write to the executable without crashing
		auto s_Ret = VirtualProtect(s_Address, s_Length, PAGE_EXECUTE_READWRITE, reinterpret_cast<PDWORD>(&s_Protection));
		if (!s_Ret)
		{
			WriteLog("could not change protection to patch address %p (%x).", s_Address, GetLastError());
			return false;
		}

		WriteLog("changed protection of %x with a length of %x.", s_Address, s_Length);

		// Copy our patch over
		s_Ret = memcpy_s(s_Address, s_Length, m_OriginalData.data(), s_Length);
		if (s_Ret)
		{
			WriteLog("could not write patch to address %p.", s_Address);
			return false;
		}

		// Change the protections back
		s_Ret = VirtualProtect(s_Address, s_Length, s_Protection, reinterpret_cast<PDWORD>(&s_Protection));
		if (!s_Ret)
		{
			WriteLog("could not change protection back to default for address %p, be warned.", s_Address);
			return false;
		}

		// Patch succcessfully applied
		return true;
	}
}