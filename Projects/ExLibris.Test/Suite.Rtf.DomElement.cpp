#include "ExLibris.Test.PCH.h"

#include <RtfDomElement.h>

#include <RtfDomDocument.h>
#include <RtfTextFormat.h>

using namespace ExLibris;

TEST(RtfDomElement, ParseText)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement e(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Text;
	t.value = "This is whack.";

	EXPECT_EQ(IRtfParseable::eResult_Handled, e.Parse(s, t));
	EXPECT_STREQ("This is whack.", e.InnerText.c_str());
}

TEST(RtfDomElement, ParseTextFormatProperty)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement e(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "pca";

	EXPECT_EQ(IRtfParseable::eResult_Handled, e.Parse(s, t));
	EXPECT_EQ(eRtfCharacterSet_Oem, e.GetTextFormat().GetCharacterSet());
}

TEST(RtfDomElement, ParseUnhandled)
{
	RtfDomDocument doc(nullptr);
	RtfDomElement e(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "windmills";

	EXPECT_EQ(IRtfParseable::eResult_Propagate, e.Parse(s, t));
}