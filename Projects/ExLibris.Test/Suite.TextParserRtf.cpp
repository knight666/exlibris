#include "ExLibris.Test.PCH.h"

#include <TextParserRtf.h>

using namespace ExLibris;

TEST(TextParserRtf, HelloWorld)
{
	TextParserRtf parser;

	std::stringstream input;
	input << "{\\rtf1\\ansi{\\fonttbl\\f0\\fswiss Helvetica;}\\f0\\pard\nHello World!\\par\n}";

	RtfDomDocument* doc = parser.ParseDocument(&input);

	ASSERT_NE(nullptr, doc);

	EXPECT_EQ(RtfFont::eFamilyType_Swiss, doc->GetFont(0).family);
	EXPECT_STREQ("Helvetica", doc->GetFont(0).name.c_str());

	RtfDomElement* root = doc->GetRootElement();

	EXPECT_EQ(eRtfCharacterSet_Ansi, root->TextFormat.character_set);
	EXPECT_STREQ("Hello World!", root->InnerText.c_str());

	EXPECT_EQ(1, root->GetChildrenCount());
}