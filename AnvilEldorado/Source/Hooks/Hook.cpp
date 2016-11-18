#pragma once
#include "Hook.hpp"

using namespace AnvilEldorado::Hooks;

bool Hook::Init()
{
	Hook_DirectX();

	return false;
}