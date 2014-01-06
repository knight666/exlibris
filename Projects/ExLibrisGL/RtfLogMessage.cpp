#include "ExLibrisGL.PCH.h"

#include "RtfLogMessage.h"

namespace ExLibris
{

	RtfLogMessage::RtfLogMessage(std::vector<RtfLogMessage>* a_List, int a_Column, int a_Line)
		: list(a_List)
		, column(a_Column)
		, line(a_Line)
	{
	}

	RtfLogMessage& RtfLogMessage::operator << (const std::string& a_Value)
	{
		message += a_Value;

		return *this;
	}

	RtfLogMessage& RtfLogMessage::operator << (const char* a_Value)
	{
		message += a_Value;

		return *this;
	}

	RtfLogMessage& RtfLogMessage::operator << (char a_Value)
	{
		char buffer[128] = { 0 };
		_snprintf(buffer, 128, "%c", a_Value);
		message += buffer;

		return *this;
	}

	RtfLogMessage& RtfLogMessage::operator << (int a_Value)
	{
		char buffer[128] = { 0 };
		_snprintf(buffer, 128, "%d", a_Value);
		message += buffer;

		return *this;
	}

	RtfLogMessage& RtfLogMessage::operator << (unsigned int a_Value)
	{
		char buffer[128] = { 0 };
		_snprintf(buffer, 128, "%u", a_Value);
		message += buffer;

		return *this;
	}

	RtfLogMessage& RtfLogMessage::operator << (long a_Value)
	{
		char buffer[128] = { 0 };
		_snprintf(buffer, 128, "%ld", a_Value);
		message += buffer;

		return *this;
	}

	RtfLogMessage& RtfLogMessage::operator << (unsigned long a_Value)
	{
		char buffer[128] = { 0 };
		_snprintf(buffer, 128, "%lu", a_Value);
		message += buffer;

		return *this;
	}

	RtfLogMessage& RtfLogMessage::operator << (float a_Value)
	{
		char buffer[128] = { 0 };
		_snprintf(buffer, 128, "%f", a_Value);
		message += buffer;

		return *this;
	}

	RtfLogMessage& RtfLogMessage::operator << (double a_Value)
	{
		char buffer[128] = { 0 };
		_snprintf(buffer, 128, "%g", a_Value);
		message += buffer;

		return *this;
	}

	void RtfLogMessage::Finish()
	{
		list->push_back(*this);
	}

}; // namespace ExLibris