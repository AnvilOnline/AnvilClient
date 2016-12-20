#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace AnvilEldorado
{
	enum class CommandType : int16_t
	{
		Command,
		VariableInt,
		VariableInt64,
		VariableFloat,
		VariableString
	};

	enum class CommandFlags : uint16_t
	{
		None,
		Cheat = 1 << 0, // only allow this command on cheat-enabled servers, whenever they get implemented
		Replicated = 1 << 1, // value of this variable should be output into the server info JSON, clients should update their variable to match the one in JSON
		Archived = 1 << 2, // value of this variable should be written when using WriteConfig
		DontUpdateInitial = 1 << 3, // don't call the update event when the variable is first being initialized
		Hidden = 1 << 4, // hide this command/var from the help listing
		RunOnMainMenu = 1 << 5, // if run at startup queue the command until the main menu is shown
		HostOnly = 1 << 6, // only execute the command if the user is host
		OmitValueInList = 1 << 7, // omit the variables value in help listing
		Internal = 1 << 8  // disallow the user from using this command, only internal ExecuteCommand calls can use it
	};

	bool operator&(const CommandFlags &p_A, const CommandFlags &p_B);
	CommandFlags operator|(const CommandFlags &p_A, const CommandFlags &p_B);

	typedef bool(*CommandUpdateFunc)(const std::vector<std::string> &p_Arguments, std::string &p_ReturnInfo);

	struct Command
	{
		std::string Name; // modulename is added to this too, makes looking it up easier
		std::string ModuleName;
		std::string ShortName; // because some people can't be bothered to type in module names
		std::string Description;

		CommandFlags Flags;
		CommandType Type;

		CommandUpdateFunc UpdateEvent;

		unsigned long ValueInt;
		unsigned long long ValueInt64;
		float ValueFloat;
		std::string ValueString;

		unsigned long DefaultValueInt;
		unsigned long long DefaultValueInt64;
		float DefaultValueFloat;
		std::string DefaultValueString;

		unsigned long ValueIntMin;
		unsigned long ValueIntMax;
		unsigned long long ValueInt64Min;
		unsigned long long ValueInt64Max;
		float ValueFloatMin;
		float ValueFloatMax;

		// CommandArgs is only used for help text, commands will have to parse the args themselves

		std::vector<std::string> CommandArgs; // arg names can't contain a space, since space/EOL is used to find where the command name ends and the command description begins
											  // this is for the help function, so you can specify an arg here like "parameter_1 This parameter controls the first parameter"
											  // in the help text this will be printed like "Usage: Game.Command <parameter_1>\r\nparameter_1: This parameter controls the first parameter."
											  // also don't end descriptions with a period since it'll be added in later
											  // modifiers might be added to the name later, so you can do things like "parameter_1:o" to signify the parameter is optional
		
		Command();

		std::string GenerateHelpText();
	};

}