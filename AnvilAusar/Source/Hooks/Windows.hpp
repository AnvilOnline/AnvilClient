#pragma once
#include <Interfaces/Initializable.hpp>
#include <cstdint>

namespace AnvilAusar::Hooks
{
	class WindowsHooks : 
		public AnvilCommon::Initializable
	{
	private:
		typedef void* (*CreateFileW_t)(const wchar_t* p_FileName, uint32_t p_Access, void* p_SecurityAttributes, uint32_t p_Creation, uint32_t p_Attributes, void* p_Template);
		static CreateFileW_t o_CreateFileW;
		static void* c_CreateFileW(const wchar_t* p_FileName, uint32_t p_Access, void* p_SecurityAttributes, uint32_t p_Creation, uint32_t p_Attributes, void* p_Template);

	public:
		virtual bool Init();
	};
}