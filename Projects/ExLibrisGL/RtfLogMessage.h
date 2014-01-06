#pragma once

namespace ExLibris
{

	class RtfLogMessage
	{

	public:

		RtfLogMessage(std::vector<RtfLogMessage>* a_List, int a_Column, int a_Line);

		RtfLogMessage& operator << (const std::string& a_Value);
		RtfLogMessage& operator << (const char* a_Value);
		RtfLogMessage& operator << (char a_Value);
		RtfLogMessage& operator << (int a_Value);
		RtfLogMessage& operator << (unsigned int a_Value);
		RtfLogMessage& operator << (long a_Value);
		RtfLogMessage& operator << (unsigned long a_Value);
		RtfLogMessage& operator << (float a_Value);
		RtfLogMessage& operator << (double a_Value);

		void Finish();

	public:

		int column;
		int line;
		std::string message;

	private:

		std::vector<RtfLogMessage>* list;

	}; // class RtfLogMessage

}; // namespace ExLibris