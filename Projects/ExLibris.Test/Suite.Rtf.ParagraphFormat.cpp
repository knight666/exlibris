#include "ExLibris.Test.PCH.h"

#include <RtfParagraphFormat.h>

#include <RtfDomDocument.h>

using namespace ExLibris;

TEST(RtfParagraphFormat, Construct)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	EXPECT_FLOAT_EQ(0.0f, pf.GetSpaceBefore().GetValue(RtfUnit::eType_Twips));
	EXPECT_FLOAT_EQ(0.0f, pf.GetSpaceAfter().GetValue(RtfUnit::eType_Twips));
}

TEST(RtfParagraphFormat, ParseSpaceBefore)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "sb";
	t.parameter = 80;

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_FLOAT_EQ(80.0f, pf.GetSpaceBefore().GetValue(RtfUnit::eType_Twips));
}

TEST(RtfParagraphFormat, ParseSpaceBeforeNegative)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "sb";
	t.parameter = -33;

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_FLOAT_EQ(-33.0f, pf.GetSpaceBefore().GetValue(RtfUnit::eType_Twips));
}

TEST(RtfParagraphFormat, ParseSpaceAfter)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "sa";
	t.parameter = 198;

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_FLOAT_EQ(198.0f, pf.GetSpaceAfter().GetValue(RtfUnit::eType_Twips));
}

TEST(RtfParagraphFormat, ParseSpaceAfterNegative)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "sa";
	t.parameter = -7;

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_FLOAT_EQ(-7.0f, pf.GetSpaceAfter().GetValue(RtfUnit::eType_Twips));
}