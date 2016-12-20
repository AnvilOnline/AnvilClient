#pragma once
#include <string>
#include <vector>
#include <deque>
#include "Utils\Singleton.hpp"
#include "Command.hpp"

namespace AnvilEldorado
{
	enum VariableSetReturnValue
	{
		eVariableSetReturnValueSuccess,
		eVariableSetReturnValueError,
		eVariableSetReturnValueOutOfRange,
		eVariableSetReturnValueInvalidArgument,
	};

	class CommandMap : public AnvilCommon::Singleton<CommandMap>
	{
	public:
		std::deque<Command> Commands;

		Command* AddCommand(Command command);
		void FinishAddCommands();
		Command* FindCommand(const std::string& name);

		std::string ExecuteCommand(std::vector<std::string> command, bool isUserInput = false);
		std::string ExecuteCommand(std::string command, bool isUserInput = false);
		std::string ExecuteCommands(std::string& commands, bool isUserInput = false);
		bool ExecuteCommandWithStatus(std::string command, bool isUserInput, std::string *output);
		std::string ExecuteQueue();

		bool GetVariableInt(const std::string& name, unsigned long& value);
		bool GetVariableInt64(const std::string& name, unsigned long long& value);
		bool GetVariableFloat(const std::string& name, float& value);
		bool GetVariableString(const std::string& name, std::string& value);

		VariableSetReturnValue SetVariable(const std::string& name, std::string& value, std::string& previousValue);
		VariableSetReturnValue SetVariable(Command* command, std::string& value, std::string& previousValue);

		std::string GenerateHelpText(std::string moduleFilter = "");

		std::string SaveVariables();

	private:
		std::vector<std::string> queuedCommands;
	};
}