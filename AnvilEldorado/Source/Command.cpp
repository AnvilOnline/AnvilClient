#include <sstream>
#include "Command.hpp"

namespace AnvilEldorado
{
	bool operator&(const CommandFlags &p_A, const CommandFlags &p_B)
	{
		return (CommandFlags)((uint16_t)p_A & (uint16_t)p_B) != CommandFlags::None;
	}

	CommandFlags operator|(const CommandFlags &p_A, const CommandFlags &p_B)
	{
		return (CommandFlags)((uint16_t)p_A | (uint16_t)p_B);
	}

	Command::Command()
		: UpdateEvent(0), ValueInt(0), ValueFloat(0.f), DefaultValueInt(0), DefaultValueFloat(0.f), ValueIntMin(0), ValueIntMax(0), ValueFloatMin(0.f), ValueFloatMax(0.f)
	{
	}

	std::string Command::GenerateHelpText()
	{
		std::stringstream ss;
		ss << Name << " - " << Description << "." << std::endl;
		ss << "Usage: " << Name << " ";

		if (Type != CommandType::Command)
		{
			ss << "<value(";

			switch (Type)
			{
			case CommandType::VariableInt:
				ss << "int";
				break;
			case CommandType::VariableInt64:
				ss << "int64";
				break;
			case CommandType::VariableFloat:
				ss << "float";
				break;
			case CommandType::VariableString:
				ss << "string";
				break;
			}

			ss << ")>" << std::endl << "Current value: " << ValueString << std::endl << std::endl;
		}
		else
		{
			std::stringstream s_ParamStream;

			for (auto &s_Argument : CommandArgs)
			{
				auto s_ArgumentName = s_Argument;
				auto s_ArgumentNameEnd = s_Argument.find(" ");
				auto s_ArgumentDescription = std::string();

				if (s_ArgumentNameEnd != std::string::npos)
				{
					s_ArgumentName = s_Argument.substr(0, s_ArgumentNameEnd);

					if (s_Argument.length() > (s_ArgumentNameEnd + 1))
						s_ArgumentDescription = s_Argument.substr(s_ArgumentNameEnd + 1);
				}

				ss << "<" << s_ArgumentName << "> ";

				if (s_ArgumentDescription.length() > 0)
					s_ParamStream << "  " << s_ArgumentName << ": " << s_ArgumentDescription << std::endl;
			}

			ss << std::endl;

			auto s_ParamDescriptions = s_ParamStream.str();

			if (s_ParamDescriptions.length() > 0)
				ss << s_ParamDescriptions << std::endl;
			else
				ss << std::endl;
		}

		return ss.str();
	}

}