#include "ExLibris.Test.PCH.h"

#include <RtfDomDocument.h>

using namespace ExLibris;

TEST(RtfDomDocument, Construct)
{
	RtfDomDocument doc;

	EXPECT_EQ(eRtfCharacterSet_Invalid, doc.GetTextFormat().character_set);
}

TEST(RtfDomDocument, CharacterSet)
{
	RtfDomDocument doc;

	doc.GetTextFormat().character_set = eRtfCharacterSet_AppleMacintoshRoman;

	EXPECT_EQ(eRtfCharacterSet_AppleMacintoshRoman, doc.GetTextFormat().character_set);
}