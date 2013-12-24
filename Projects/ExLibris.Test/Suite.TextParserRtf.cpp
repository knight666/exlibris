#include "ExLibris.Test.PCH.h"

#include <TextParserRtf.h>

using namespace ExLibris;

TEST(TextParserRtf, HelloWorld)
{
	std::string input = "{\\rtf1\\ansi{\\fonttbl\\f0\\fswiss Helvetica;}\\f0\\pard\nHello World!\\par\n}";

	TextParserRtf parser;
	parser.SetInput(input);

	EXPECT_TRUE(parser.IsValid());
	EXPECT_EQ(TextParserRtf::eCharacterSet_ANSI, parser.GetCharacterSet());
}