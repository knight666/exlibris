#ifndef _COMMANDLINEARGUMENT_H_
#define _COMMANDLINEARGUMENT_H_

namespace Framework
{

	struct CommandLineArgument
	{

		enum Type
		{
			eType_ExecutablePath,
			eType_Flag,
			eType_Command,
			eType_Value
		};

	public:

		CommandLineArgument()
			: type(eType_Value)
		{
		}

		Type type;
		std::string value;

	}; // struct CommandLineArgument

}; // namespace Framework

#endif