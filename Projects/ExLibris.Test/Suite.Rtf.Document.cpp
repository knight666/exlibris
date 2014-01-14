#include "ExLibris.Test.PCH.h"

#include <RtfDomDocument.h>

using namespace ExLibris;

TEST(RtfDomDocument, Construct)
{
	RtfDomDocument doc(nullptr);

	EXPECT_EQ(eRtfCharacterSet_Invalid, doc.GetTextFormat().GetCharacterSet());
}

TEST(RtfDomDocument, CharacterSet)
{
	RtfDomDocument doc(nullptr);

	doc.GetTextFormat().SetCharacterSet(eRtfCharacterSet_AppleMacintoshRoman);

	EXPECT_EQ(eRtfCharacterSet_AppleMacintoshRoman, doc.GetTextFormat().GetCharacterSet());
}