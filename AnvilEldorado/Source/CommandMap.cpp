#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <algorithm>
#include <sstream>

#include "Engine.hpp"
#include "Blam\Network\Session.hpp"
#include "CommandMap.hpp"

namespace
{
	PCHAR *CommandLineToArgvA(PCHAR CmdLine, int* _argc);
}

namespace AnvilEldorado
{
	Command *CommandMap::FindCommand(const std::string& name)
	{
		for (auto it = Commands.begin(); it < Commands.end(); it++)
		if ((it->Name.length() > 0 && !_stricmp(it->Name.c_str(), name.c_str())) || (it->ShortName.length() > 0 && !_stricmp(it->ShortName.c_str(), name.c_str())))
			return &(*it);

		return nullptr;
	}

	Command *CommandMap::AddCommand(Command command)
	{
		if (FindCommand(command.Name) || FindCommand(command.ShortName))
			return nullptr;

		this->Commands.push_back(command);

		return &this->Commands.back();
	}

	void CommandMap::FinishAddCommands()
	{
		for (auto &s_Command : Commands)
		{
			if ((s_Command.Type != CommandType::Command) && (s_Command.Flags & CommandFlags::DontUpdateInitial))
				if (s_Command.UpdateEvent)
					s_Command.UpdateEvent(std::vector<std::string>(), std::string());
		}
	}

	std::string CommandMap::ExecuteCommand(std::vector<std::string> command, bool isUserInput)
	{
		std::string commandStr = "";
		for (auto cmd : command)
			commandStr += "\"" + cmd + "\" ";

		return ExecuteCommand(commandStr, isUserInput);
	}

	bool CommandMap::ExecuteCommandWithStatus(std::string command, bool isUserInput, std::string *output)
	{
		*output = "";

		int numArgs = 0;
		auto args = CommandLineToArgvA((PCHAR)command.c_str(), &numArgs);

		if (numArgs <= 0)
		{
			*output = "Invalid input";
			return false;
		}

		auto cmd = FindCommand(args[0]);
		if (!cmd || (isUserInput && cmd->Flags & CommandFlags::Internal))
		{
			*output = "Command/Variable not found";
			return false;
		}

		if ((cmd->Flags & CommandFlags::RunOnMainMenu) && !Engine::Instance()->HasMainMenuShown())
		{
			queuedCommands.push_back(command);
			*output = "Command queued until mainmenu shows";
			return true;
		}

		// Host-only commands
		if (cmd->Flags & CommandFlags::Cheat || cmd->Flags & CommandFlags::HostOnly)
		{
			auto session = Blam::Network::GetActiveSession();
			if (session && session->IsEstablished() && !session->IsHost())
			{
				*output = "Only a player hosting a game can use this command";
				return false;
			}
		}

		std::vector<std::string> argsVect;
		if (numArgs > 1)
			for (int i = 1; i < numArgs; i++)
				argsVect.push_back(args[i]);

		if (cmd->Type == CommandType::Command)
			return cmd->UpdateEvent(argsVect, *output); // if it's a command call it and return

		if (numArgs <= 1)
		{
			*output = cmd->ValueString;
			return true;
		}

		std::string previousValue;
		auto updateRet = SetVariable(cmd, argsVect[0], previousValue);
		switch (updateRet)
		{
		case eVariableSetReturnValueError:
			*output = "Command/variable not found";
			return false;
		case eVariableSetReturnValueInvalidArgument:
			*output = "Invalid value";
			return false;
		case eVariableSetReturnValueOutOfRange:
			if (cmd->Type == CommandType::VariableInt)
				*output = "Value " + argsVect[0] + " out of range [" + std::to_string(cmd->ValueIntMin) + ".." + std::to_string(cmd->ValueIntMax) + "]";
			else if (cmd->Type == CommandType::VariableInt64)
				*output = "Value " + argsVect[0] + " out of range [" + std::to_string(cmd->ValueInt64Min) + ".." + std::to_string(cmd->ValueInt64Max) + "]";
			else if (cmd->Type == CommandType::VariableFloat)
				*output = "Value " + argsVect[0] + " out of range [" + std::to_string(cmd->ValueFloatMin) + ".." + std::to_string(cmd->ValueFloatMax) + "]";
			else
				*output = "Value " + argsVect[0] + " out of range [this shouldn't be happening!]";
			return false;
		}

		if (!cmd->UpdateEvent)
		{
			*output = previousValue + " -> " + cmd->ValueString; // no update event, so we'll just return with what we set the value to
			return true;
		}

		auto ret = cmd->UpdateEvent(argsVect, *output);

		if (!ret) // error, revert the variable
			this->SetVariable(cmd, previousValue, std::string());

		if (output->length() <= 0)
			*output = previousValue + " -> " + cmd->ValueString;

		return ret;
	}

	std::string CommandMap::ExecuteCommand(std::string command, bool isUserInput)
	{
		std::string output;
		ExecuteCommandWithStatus(command, isUserInput, &output);
		return output;
	}

	bool CommandMap::GetVariableInt(const std::string& name, unsigned long& value)
	{
		auto command = FindCommand(name);
		if (!command || command->Type != CommandType::VariableInt)
			return false;

		value = command->ValueInt;
		return true;
	}

	bool CommandMap::GetVariableInt64(const std::string& name, unsigned long long& value)
	{
		auto command = FindCommand(name);
		if (!command || command->Type != CommandType::VariableInt64)
			return false;

		value = command->ValueInt64;
		return true;
	}

	bool CommandMap::GetVariableFloat(const std::string& name, float& value)
	{
		auto command = FindCommand(name);
		if (!command || command->Type != CommandType::VariableFloat)
			return false;

		value = command->ValueFloat;
		return true;
	}

	bool CommandMap::GetVariableString(const std::string& name, std::string& value)
	{
		auto command = FindCommand(name);
		if (!command || command->Type != CommandType::VariableString)
			return false;

		value = command->ValueString;
		return true;
	}

	VariableSetReturnValue CommandMap::SetVariable(const std::string& name, std::string& value, std::string& previousValue)
	{
		auto command = FindCommand(name);
		if (!command)
			return eVariableSetReturnValueError;

		return SetVariable(command, value, previousValue);
	}

	VariableSetReturnValue CommandMap::SetVariable(Command* command, std::string& value, std::string& previousValue)
	{
		// Disallow setting internal variables through the console
		if (command->Flags & CommandFlags::Internal)
			return eVariableSetReturnValueError;

		try {
			switch (command->Type)
			{
			case CommandType::VariableString:
				previousValue = command->ValueString;
				command->ValueString = value;
				break;
			case CommandType::VariableInt:
				{
					previousValue = std::to_string(command->ValueInt);
					auto newValue = std::stoul(value, 0, 0);
					if ((command->ValueIntMin || command->ValueIntMax) && (newValue < command->ValueIntMin || newValue > command->ValueIntMax))
						return eVariableSetReturnValueOutOfRange;

					command->ValueInt = newValue;
					command->ValueString = std::to_string(command->ValueInt); // set the ValueString too so we can print the value out easier
					break;
				}
			case CommandType::VariableInt64:
				{
					previousValue = std::to_string(command->ValueInt);
					auto newValue = std::stoull(value, 0, 0);
					if ((command->ValueInt64Min || command->ValueInt64Max) && (newValue < command->ValueInt64Min || newValue > command->ValueInt64Max))
						return eVariableSetReturnValueOutOfRange;

					command->ValueInt64 = newValue;
					command->ValueString = std::to_string(command->ValueInt64); // set the ValueString too so we can print the value out easier
					break;
				}
			case CommandType::VariableFloat:
				{
					previousValue = std::to_string(command->ValueFloat);
					auto newValue = std::stof(value, 0);
					if ((command->ValueFloatMin || command->ValueFloatMax) && (newValue < command->ValueFloatMin || newValue > command->ValueFloatMax))
						return eVariableSetReturnValueOutOfRange;

					command->ValueFloat = newValue;
					command->ValueString = std::to_string(command->ValueFloat); // set the ValueString too so we can print the value out easier
					break;
				}
			}
		}
		catch (std::invalid_argument)
		{
			return eVariableSetReturnValueInvalidArgument;
		}
		catch (std::out_of_range)
		{
			return eVariableSetReturnValueInvalidArgument;
		}

		return eVariableSetReturnValueSuccess;
	}

	bool compare_commands(const Command& lhs, const Command& rhs) {
		return lhs.Name < rhs.Name;
	}

	std::string CommandMap::GenerateHelpText(std::string moduleFilter)
	{
		std::deque<Command> tempCommands(Commands);

		std::sort(tempCommands.begin(), tempCommands.end(), compare_commands);
		std::stringstream ss;
		std::stringstream hasParent; // store commands with a parent module seperately, so they can be added to the main stringstream after the non-parent commands
		for (auto cmd : tempCommands)
		{
			if (cmd.Flags & CommandFlags::Hidden || cmd.Flags & CommandFlags::Internal)
				continue;

			if (!moduleFilter.empty() && (cmd.ModuleName.empty() || _stricmp(cmd.ModuleName.c_str(), moduleFilter.c_str())))
				continue;

			std::string helpText = cmd.Name;
			if (cmd.Type != CommandType::Command && !(cmd.Flags & CommandFlags::OmitValueInList))
				helpText += " " + cmd.ValueString;

			helpText += " - " + cmd.Description;

			if (cmd.ModuleName.length() > 0)
				hasParent << helpText << std::endl;
			else
				ss << helpText << std::endl;
		}

		ss << hasParent.str();

		return ss.str();
	}

	std::string CommandMap::SaveVariables()
	{
		std::stringstream ss;
		for (auto cmd : Commands)
		{
			if (cmd.Type == CommandType::Command || !(cmd.Flags & CommandFlags::Archived) || (cmd.Flags & CommandFlags::Internal))
				continue;

			ss << cmd.Name << " \"" << cmd.ValueString << "\"" << std::endl;
		}
		return ss.str();
	}

	std::string CommandMap::ExecuteCommands(std::string& commands, bool isUserInput)
	{
		std::istringstream stream(commands);
		std::stringstream ss;
		std::string line;
		std::string output;
		int lineIdx = 0;
		while (std::getline(stream, line))
		{
			if (!this->ExecuteCommandWithStatus(line, isUserInput, &output))
			{
				ss << "Error at line " << lineIdx << std::endl;
			}
			lineIdx++;
		}
		return ss.str();
	}

	std::string CommandMap::ExecuteQueue()
	{
		std::stringstream ss;
		for (auto cmd : queuedCommands)
		{
			ss << ExecuteCommand(cmd, true) << std::endl;
		}
		queuedCommands.clear();
		return ss.str();
	}
}

namespace
{
	PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc)
	{
		PCHAR* argv;
		PCHAR  _argv;
		ULONG   len;
		ULONG   argc;
		CHAR   a;
		ULONG   i, j;

		BOOLEAN  in_QM;
		BOOLEAN  in_TEXT;
		BOOLEAN  in_SPACE;

		len = strlen(CmdLine);
		i = ((len + 2) / 2)*sizeof(PVOID)+sizeof(PVOID);

		argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
			i + (len + 2)*sizeof(CHAR));

		if (!argv)
			return 0;

		_argv = (PCHAR)(((PUCHAR)argv) + i);

		argc = 0;
		argv[argc] = _argv;
		in_QM = FALSE;
		in_TEXT = FALSE;
		in_SPACE = TRUE;
		i = 0;
		j = 0;

		while (a = CmdLine[i]) {
			if (in_QM) {
				if (a == '\"') {
					in_QM = FALSE;
				}
				else {
					_argv[j] = a;
					j++;
				}
			}
			else {
				switch (a) {
				case '\"':
					in_QM = TRUE;
					in_TEXT = TRUE;
					if (in_SPACE) {
						argv[argc] = _argv + j;
						argc++;
					}
					in_SPACE = FALSE;
					break;
				case ' ':
				case '\t':
				case '\n':
				case '\r':
					if (in_TEXT) {
						_argv[j] = '\0';
						j++;
					}
					in_TEXT = FALSE;
					in_SPACE = TRUE;
					break;
				default:
					in_TEXT = TRUE;
					if (in_SPACE) {
						argv[argc] = _argv + j;
						argc++;
					}
					_argv[j] = a;
					j++;
					in_SPACE = FALSE;
					break;
				}
			}
			i++;
		}
		_argv[j] = '\0';
		argv[argc] = NULL;

		(*_argc) = argc;
		return argv;
	}
}