#include "ExLibris.Test.PCH.h"

#include <RtfParagraphFormat.h>

#include <RtfDomDocument.h>

using namespace ExLibris;

TEST(RtfParagraphFormat, Construct)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	EXPECT_FALSE(pf.IsKeepIntactEnabled());
	EXPECT_FALSE(pf.IsKeepWithNextEnabled());
	EXPECT_FLOAT_EQ(0.0f, pf.GetSpaceBefore().GetValue(RtfUnit::eType_Twips));
	EXPECT_FLOAT_EQ(0.0f, pf.GetSpaceAfter().GetValue(RtfUnit::eType_Twips));
	EXPECT_FALSE(pf.IsAutoSpacingBeforeEnabled());
	EXPECT_FALSE(pf.IsAutoSpacingAfterEnabled());
	EXPECT_TRUE(pf.IsSnapLineToGridEnabled());
	EXPECT_EQ(Rtf::eLineHeightRule_Automatic, pf.GetLineHeightRule());
	EXPECT_FLOAT_EQ(100.0f, pf.GetLineHeight().GetValue(RtfUnit::eType_Twips));
}

TEST(RtfParagraphFormat, ParseKeepIntactEnabled)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "keep";

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_TRUE(pf.IsKeepIntactEnabled());
}

TEST(RtfParagraphFormat, ParseKeepWithNextEnabled)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "keepn";

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_TRUE(pf.IsKeepWithNextEnabled());
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

TEST(RtfParagraphFormat, ParseAutoSpacingBefore)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "sbauto";
	t.parameter = 1;

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_TRUE(pf.IsAutoSpacingBeforeEnabled());
}

TEST(RtfParagraphFormat, ParseAutoSpacingBeforeInvalid)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "sbauto";
	t.parameter = -33;

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_FALSE(pf.IsAutoSpacingBeforeEnabled());
}

TEST(RtfParagraphFormat, ParseSnapToGridDisabled)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "nosnaplinegrid";

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_FALSE(pf.IsSnapLineToGridEnabled());
}

TEST(RtfParagraphFormat, ParseLineHeightPositive)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "sl";
	t.parameter = 250;

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_EQ(Rtf::eLineHeightRule_Maximum, pf.GetLineHeightRule());
	EXPECT_FLOAT_EQ(250.0f, pf.GetLineHeight().GetValue(RtfUnit::eType_Twips));
}

TEST(RtfParagraphFormat, ParseLineHeightNegative)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "sl";
	t.parameter = -333;

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_EQ(Rtf::eLineHeightRule_Absolute, pf.GetLineHeightRule());
	EXPECT_FLOAT_EQ(333.0f, pf.GetLineHeight().GetValue(RtfUnit::eType_Twips));
}

TEST(RtfParagraphFormat, ParseLineHeightZero)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;
	t.value = "sl";
	t.parameter = 0;

	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_EQ(Rtf::eLineHeightRule_Automatic, pf.GetLineHeightRule());
	EXPECT_FLOAT_EQ(100.0f, pf.GetLineHeight().GetValue(RtfUnit::eType_Twips));
}

TEST(RtfParagraphFormat, ParseLineHeightMultiple)
{
	RtfDomDocument doc(nullptr);
	RtfParagraphFormat pf(doc);

	RtfParserState s;

	RtfToken t;
	t.type = RtfToken::eParseType_Command;

	t.value = "sl";
	t.parameter = 2;
	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_FLOAT_EQ(2.0f, pf.GetLineHeight().GetValue(RtfUnit::eType_Twips));

	t.value = "slmult";
	t.parameter = 1;
	EXPECT_EQ(IRtfParseable::eResult_Handled, pf.Parse(s, t));

	EXPECT_EQ(Rtf::eLineHeightRule_Automatic, pf.GetLineHeightRule());

	EXPECT_FLOAT_EQ(200.0f, pf.GetLineHeight().GetValue(RtfUnit::eType_Twips));
}