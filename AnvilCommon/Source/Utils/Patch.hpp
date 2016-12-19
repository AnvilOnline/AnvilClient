#pragma once
#include <array>
#include <cstdint>
#include <initializer_list>
#include <vector>
#include "Globals.hpp"

namespace AnvilCommon::Utils
{
	class Patch
	{
	public:
		Patch();
		Patch(const size_t p_Offset, const std::initializer_list<uint8_t> &p_PatchData, const std::initializer_list<uint8_t> &p_OriginalData);
		Patch(const size_t p_Offset, const std::initializer_list<uint8_t> &p_PatchData, void *p_BaseAddress = Internal_GetModuleStorage());
		Patch(const size_t p_Offset, const uint8_t p_Value, const size_t p_Length = 1, void *p_BaseAddress = Internal_GetModuleStorage());

		static bool NopFill(const size_t p_Offset, const size_t p_Length = 1, void *p_BaseAddress = Internal_GetModuleStorage());

		bool Apply(const bool p_Reset = false, void *p_BaseAddress = Internal_GetModuleStorage()) const;
		bool Reset(void *p_BaseAddress = Internal_GetModuleStorage()) const;

	private:
		size_t m_Offset;
		std::vector<uint8_t> m_PatchData;
		std::vector<uint8_t> m_OriginalData;
	};
}