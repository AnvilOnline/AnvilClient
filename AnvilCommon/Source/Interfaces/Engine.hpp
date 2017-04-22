#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <typeinfo>

#include "Initializable.hpp"

namespace AnvilCommon
{
	class Engine :
		public Initializable
	{
	private:
		std::map<std::string, std::shared_ptr<AnvilCommon::Initializable>> m_Subsystems;

	public:
		virtual uint8_t* ExecutableBase() = 0;
		virtual size_t ExecutableSize() = 0;

		template <class T>
		std::shared_ptr<T> GetSubsystem()
		{
			auto s_TypeName = std::string(typeid(T).name());
			auto s_Pair = m_Subsystems.find(s_TypeName);

			if (s_Pair == m_Subsystems.end())
			{
				auto s_Subsystem = std::make_shared<T>();
				m_Subsystems.insert(std::make_pair(s_TypeName, std::static_pointer_cast<AnvilCommon::Initializable>(s_Subsystem)));
				
				return s_Subsystem;
			}

			return std::static_pointer_cast<T>(s_Pair->second);
		}
	};
}