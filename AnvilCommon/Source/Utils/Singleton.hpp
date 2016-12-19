#pragma once
#include <memory>
#include <mutex>

namespace AnvilCommon
{
	template <typename T>
	class Singleton
	{
	public:
		static T *Instance()
		{
			static T s_Instance;
			return &s_Instance;
		}
	};
}