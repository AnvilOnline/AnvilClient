#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <sstream>
#include "Logger.hpp"
#include "Hook.hpp"

namespace AnvilCommon::Utils
{
	bool operator&(const HookFlags &p_A, const HookFlags &p_B)
	{
		return (HookFlags)((int32_t)p_A & (int32_t)p_B) != HookFlags::None;
	}

	Hook::Hook() :
		m_Offset(0), m_Flags(HookFlags::None)
	{
		m_OriginalData.resize(5);
		memcpy(m_OriginalData.data(), (uint8_t *)Internal_GetModuleStorage() + m_Offset, m_OriginalData.size());
	}

	Hook::Hook(const size_t p_Offset, void *p_DestFunc, const HookFlags &p_Flags, const std::initializer_list<uint8_t> &p_Reset)
		: m_Offset(p_Offset), m_DestFunc(p_DestFunc), m_Flags(p_Flags), m_OriginalData(p_Reset)
	{
		if (m_OriginalData.size() <= 0)
		{
			m_OriginalData.resize(((m_Flags & HookFlags::IsJmpIfEqual) ? 6 : 5));
			memcpy(m_OriginalData.data(), (uint8_t *)Internal_GetModuleStorage() + m_Offset, m_OriginalData.size());
		}
	}

	bool WriteCall(void *p_Address, void *p_NewFunction)
	{
		DWORD temp;
		DWORD temp2;
		uint8_t tempJMP[5] = { 0xE8, 0x90, 0x90, 0x90, 0x90 };
		uint32_t JMPSize = ((uint32_t)p_NewFunction - (uint32_t)p_Address - 5);

		if (!VirtualProtect(p_Address, 5, PAGE_READWRITE, &temp))
		{
			std::stringstream ss;
			ss << "Failed to set protection on memory address " << std::hex << p_Address;

			WriteLog("ERROR: %s", ss.str().c_str());

			return false;
		}

		memcpy(&tempJMP[1], &JMPSize, 4);
		memcpy(p_Address, tempJMP, 5);
		VirtualProtect(p_Address, 5, temp, &temp2);

		return true;
	}

	bool WriteVirtual(void *p_Address, void *p_NewFunction)
	{
		DWORD temp;
		DWORD temp2;

		if (!VirtualProtect(p_Address, 4, PAGE_READWRITE, &temp))
		{
			std::stringstream ss;
			ss << "Failed to set protection on memory address " << std::hex << (void*)p_Address;

			WriteLog("ERROR: %s", ss.str().c_str());

			return false;
		}
		else
		{
			*(uint32_t *)p_Address = (uint32_t)p_NewFunction;
			VirtualProtect(p_Address, 4, temp, &temp2);
		}

		return true;
	}

	bool WriteJump(void *p_Address, void *p_NewFunction, const HookFlags &p_Flags)
	{
		DWORD temp;
		DWORD temp2;
		uint8_t tempJMP[5] = { 0xE9, 0x90, 0x90, 0x90, 0x90 };
		uint8_t tempJE[6] = { 0x0F, 0x84, 0x90, 0x90, 0x90, 0x90 };
		uint32_t patchSize = ((p_Flags & HookFlags::IsJmpIfEqual) || (p_Flags & HookFlags::IsJmpIfNotEqual)) ? 6 : 5;
		uint32_t JMPSize = ((uint32_t)p_NewFunction - (uint32_t)p_Address - patchSize);

		if (!VirtualProtect(p_Address, patchSize, PAGE_READWRITE, &temp))
		{
			std::stringstream ss;
			ss << "Failed to set protection on memory address " << std::hex << p_Address;
			OutputDebugString(ss.str().c_str());
			return false;
		}

		if (p_Flags & HookFlags::IsJmpIfEqual || p_Flags & HookFlags::IsJmpIfNotEqual)
		{
			if (p_Flags & HookFlags::IsJmpIfNotEqual)
				tempJE[1] = 0x85;

			memcpy(&tempJE[2], &JMPSize, 4);
			memcpy(p_Address, tempJE, 6);
		}
		else
		{
			memcpy(&tempJMP[1], &JMPSize, 4);
			memcpy(p_Address, tempJMP, 5);
		}
		
		VirtualProtect(p_Address, patchSize, temp, &temp2);

		return true;
	}

	bool Hook::Apply(const bool p_Reset, void *p_BaseAddress) const
	{
		if (p_Reset)
			return Reset(p_BaseAddress);

		if (m_Flags & HookFlags::IsCall)
			return WriteCall((uint8_t *)p_BaseAddress + m_Offset, m_DestFunc);
		else if (m_Flags & HookFlags::IsVirtual)
			return WriteVirtual((uint8_t *)p_BaseAddress + m_Offset, m_DestFunc);
		else
			return WriteJump((uint8_t *)p_BaseAddress + m_Offset, m_DestFunc, m_Flags);
	}

	bool Hook::Reset(void *p_Offset) const
	{
		if (m_OriginalData.size())
			memcpy((uint8_t *)p_Offset + m_Offset, m_OriginalData.data(), m_OriginalData.size());

		return true;
	}
}