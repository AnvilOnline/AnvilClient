#pragma once
#include <initializer_list>
#include <vector>
#include "Globals.hpp"

namespace AnvilCommon::Utils
{
	enum class HookFlags : int32_t
	{
		None,
		IsCall = 1 << 0,
		IsJmpIfEqual = 1 << 1
	};

	bool operator&(const HookFlags &p_A, const HookFlags &p_B);

	class Hook
	{
	public:
		Hook();
		Hook(const size_t p_Offset, void *p_DestFunc, const HookFlags &p_Flags = HookFlags::None, const std::initializer_list<uint8_t> &p_Reset = {});

		bool Apply(const bool p_Reset = false, void *p_Offset = Internal_GetModuleStorage()) const;
		bool Reset(void *p_Offset = Internal_GetModuleStorage()) const;

	private:
		size_t m_Offset;
		void* m_DestFunc;
		const HookFlags m_Flags;
		std::vector<uint8_t> m_OriginalData;
	};
}